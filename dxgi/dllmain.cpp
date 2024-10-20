// dxgi proxy
#include "pch.h"

struct dxgi_dll {
	HMODULE dll;
	PFN_CREATE_DXGI_FACTORY OrignalCreateDXGIFactory;
	PFN_CREATE_DXGI_FACTORY OrignalCreateDXGIFactory1;
	PFN_CREATE_DXGI_FACTORY_2 OrignalCreateDXGIFactory2;
	PFN_DECLARE_ADAPTER_REMOVAL_SUPPORT OrignalDXGIDeclareAdapterRemovalSupport;
	PFN_GET_DEBUG_INTERFACE OrignalDXGIGetDebugInterface1;
} dxgi;

HRESULT WINAPI CreateDXGIFactory(REFIID riid, _COM_Outptr_ void** ppFactory)
{
	return dxgi.OrignalCreateDXGIFactory(riid, ppFactory);
}

HRESULT WINAPI CreateDXGIFactory1(REFIID riid, _COM_Outptr_ void** ppFactory)
{
	return dxgi.OrignalCreateDXGIFactory1(riid, ppFactory);
}

HRESULT WINAPI CreateDXGIFactory2(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory)
{
	return dxgi.OrignalCreateDXGIFactory2(Flags, riid, ppFactory);
}

HRESULT WINAPI DXGIDeclareAdapterRemovalSupport()
{
	return dxgi.OrignalDXGIDeclareAdapterRemovalSupport();
}

HRESULT WINAPI DXGIGetDebugInterface1(UINT Flags, REFIID riid, void** ppDebug)
{
	return dxgi.OrignalDXGIGetDebugInterface1(Flags, riid, ppDebug);
}

static void InitDllProxy()
{
	wchar_t path[MAX_PATH];

	GetSystemDirectory(path, MAX_PATH);
	wcscat_s(path, L"\\dxgi.dll");
	dxgi.dll = LoadLibrary(path);
	if (dxgi.dll == 0)
	{
		MessageBox(0, L"Cannot load original dxgi.dll library", L"Proxy", MB_ICONERROR);
		ExitProcess(0);
	}
	dxgi.OrignalCreateDXGIFactory = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(dxgi.dll, "CreateDXGIFactory");
	dxgi.OrignalCreateDXGIFactory1 = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(dxgi.dll, "CreateDXGIFactory1");
	dxgi.OrignalCreateDXGIFactory2 = (PFN_CREATE_DXGI_FACTORY_2)GetProcAddress(dxgi.dll, "CreateDXGIFactory2");
	dxgi.OrignalDXGIDeclareAdapterRemovalSupport = (PFN_DECLARE_ADAPTER_REMOVAL_SUPPORT)GetProcAddress(dxgi.dll, "DXGIDeclareAdapterRemovalSupport");
	dxgi.OrignalDXGIGetDebugInterface1 = (PFN_GET_DEBUG_INTERFACE)GetProcAddress(dxgi.dll, "DXGIGetDebugInterface1");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		InitDllProxy();
		if (InitHook() != 0)
		{
			MessageBox(0, L"Cannot hook hl.exe", L"Proxy", MB_ICONERROR);
			return FALSE;
		}

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		FreeLibrary(dxgi.dll);
		if (FreeHook() != 0)
		{
			MessageBox(0, L"Failed to shutdown hooks", L"Proxy", MB_ICONERROR);
			return FALSE;
		}
	}
	break;
	}
	return TRUE;
}