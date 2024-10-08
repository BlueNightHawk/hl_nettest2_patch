// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

struct dxgi_dll {
	HMODULE dll;
	FARPROC OrignalApplyCompatResolutionQuirking;
	FARPROC OrignalCompatString;
	FARPROC OrignalCompatValue;
	FARPROC OrignalCreateDXGIFactory;
	FARPROC OrignalCreateDXGIFactory1;
	FARPROC OrignalCreateDXGIFactory2;
	FARPROC OrignalDXGID3D10CreateDevice;
	FARPROC OrignalDXGID3D10CreateLayeredDevice;
	FARPROC OrignalDXGID3D10GetLayeredDeviceSize;
	FARPROC OrignalDXGID3D10RegisterLayers;
	FARPROC OrignalDXGIDeclareAdapterRemovalSupport;
	FARPROC OrignalDXGIDisableVBlankVirtualization;
	FARPROC OrignalDXGIDumpJournal;
	FARPROC OrignalDXGIGetDebugInterface1;
	FARPROC OrignalDXGIReportAdapterConfiguration;
	FARPROC OrignalPIXBeginCapture;
	FARPROC OrignalPIXEndCapture;
	FARPROC OrignalPIXGetCaptureState;
	FARPROC OrignalSetAppCompatStringPointer;
	FARPROC OrignalUpdateHMDEmulationStatus;
} dxgi;

extern "C"
{
	void DXGI_API ApplyCompatResolutionQuirking() { _asm { jmp[dxgi.OrignalApplyCompatResolutionQuirking] } }
	void DXGI_API CompatString() { _asm { jmp[dxgi.OrignalCompatString] } }
	void DXGI_API CompatValue() { _asm { jmp[dxgi.OrignalCompatValue] } }
	void DXGI_API CreateDXGIFactory() { _asm { jmp[dxgi.OrignalCreateDXGIFactory] } }
	void DXGI_API CreateDXGIFactory1() { _asm { jmp[dxgi.OrignalCreateDXGIFactory1] } }
	void DXGI_API CreateDXGIFactory2() { _asm { jmp[dxgi.OrignalCreateDXGIFactory2] } }
	void DXGI_API DXGID3D10CreateDevice() { _asm { jmp[dxgi.OrignalDXGID3D10CreateDevice] } }
	void DXGI_API DXGID3D10CreateLayeredDevice() { _asm { jmp[dxgi.OrignalDXGID3D10CreateLayeredDevice] } }
	void DXGI_API DXGID3D10GetLayeredDeviceSize() { _asm { jmp[dxgi.OrignalDXGID3D10GetLayeredDeviceSize] } }
	void DXGI_API DXGID3D10RegisterLayers() { _asm { jmp[dxgi.OrignalDXGID3D10RegisterLayers] } }
	void DXGI_API DXGIDeclareAdapterRemovalSupport() { _asm { jmp[dxgi.OrignalDXGIDeclareAdapterRemovalSupport] } }
	void DXGI_API DXGIDisableVBlankVirtualization() { _asm { jmp[dxgi.OrignalDXGIDisableVBlankVirtualization] } }
	void DXGI_API DXGIDumpJournal() { _asm { jmp[dxgi.OrignalDXGIDumpJournal] } }
	void DXGI_API DXGIGetDebugInterface1() { _asm { jmp[dxgi.OrignalDXGIGetDebugInterface1] } }
	void DXGI_API DXGIReportAdapterConfiguration() { _asm { jmp[dxgi.OrignalDXGIReportAdapterConfiguration] } }
	void DXGI_API PIXBeginCapture() { _asm { jmp[dxgi.OrignalPIXBeginCapture] } }
	void DXGI_API PIXEndCapture() { _asm { jmp[dxgi.OrignalPIXEndCapture] } }
	void DXGI_API PIXGetCaptureState() { _asm { jmp[dxgi.OrignalPIXGetCaptureState] } }
	void DXGI_API SetAppCompatStringPointer() { _asm { jmp[dxgi.OrignalSetAppCompatStringPointer] } }
	void DXGI_API UpdateHMDEmulationStatus() { _asm { jmp[dxgi.OrignalUpdateHMDEmulationStatus] } }
}

void InitDllProxy()
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
	dxgi.OrignalApplyCompatResolutionQuirking = GetProcAddress(dxgi.dll, "ApplyCompatResolutionQuirking");
	dxgi.OrignalCompatString = GetProcAddress(dxgi.dll, "CompatString");
	dxgi.OrignalCompatValue = GetProcAddress(dxgi.dll, "CompatValue");
	dxgi.OrignalCreateDXGIFactory = GetProcAddress(dxgi.dll, "CreateDXGIFactory");
	dxgi.OrignalCreateDXGIFactory1 = GetProcAddress(dxgi.dll, "CreateDXGIFactory1");
	dxgi.OrignalCreateDXGIFactory2 = GetProcAddress(dxgi.dll, "CreateDXGIFactory2");
	dxgi.OrignalDXGID3D10CreateDevice = GetProcAddress(dxgi.dll, "DXGID3D10CreateDevice");
	dxgi.OrignalDXGID3D10CreateLayeredDevice = GetProcAddress(dxgi.dll, "DXGID3D10CreateLayeredDevice");
	dxgi.OrignalDXGID3D10GetLayeredDeviceSize = GetProcAddress(dxgi.dll, "DXGID3D10GetLayeredDeviceSize");
	dxgi.OrignalDXGID3D10RegisterLayers = GetProcAddress(dxgi.dll, "DXGID3D10RegisterLayers");
	dxgi.OrignalDXGIDeclareAdapterRemovalSupport = GetProcAddress(dxgi.dll, "DXGIDeclareAdapterRemovalSupport");
	dxgi.OrignalDXGIDisableVBlankVirtualization = GetProcAddress(dxgi.dll, "DXGIDisableVBlankVirtualization");
	dxgi.OrignalDXGIDumpJournal = GetProcAddress(dxgi.dll, "DXGIDumpJournal");
	dxgi.OrignalDXGIGetDebugInterface1 = GetProcAddress(dxgi.dll, "DXGIGetDebugInterface1");
	dxgi.OrignalDXGIReportAdapterConfiguration = GetProcAddress(dxgi.dll, "DXGIReportAdapterConfiguration");
	dxgi.OrignalPIXBeginCapture = GetProcAddress(dxgi.dll, "PIXBeginCapture");
	dxgi.OrignalPIXEndCapture = GetProcAddress(dxgi.dll, "PIXEndCapture");
	dxgi.OrignalPIXGetCaptureState = GetProcAddress(dxgi.dll, "PIXGetCaptureState");
	dxgi.OrignalSetAppCompatStringPointer = GetProcAddress(dxgi.dll, "SetAppCompatStringPointer");
	dxgi.OrignalUpdateHMDEmulationStatus = GetProcAddress(dxgi.dll, "UpdateHMDEmulationStatus");
}

typedef void (WINAPI* GETSYSTEMTIME)(LPSYSTEMTIME);
GETSYSTEMTIME ORIG_GetSystemTime = NULL;
void WINAPI HOOKED_GetSystemTime(LPSYSTEMTIME lpSystemTime)
{
	ORIG_GetSystemTime(lpSystemTime);
	lpSystemTime->wYear = 1998;
	lpSystemTime->wMonth = 10;
}

// Enable all resolutions
// unknown
int* dword_4F11D0;
int* dword_4F11DC;

// Display Widths
int* dword_4F11D4;
// Display Heights
int* dword_4F11D8;
// Display Mode Buffer
char* byte_4F11FC;
// Display Mode Count
int* dword_4F111C;

std::vector<DEVMODE> g_DevModes;

void InitDisplayResolutions()
{
	DEVMODE		devmode = {};
	int 	iMode = 0;

	bool bContinue = false;

	while (EnumDisplaySettings(NULL, iMode, &devmode))
	{
		++iMode;
		for (size_t i = 0; i < g_DevModes.size(); i++)
		{
			if (g_DevModes.at(i).dmPelsWidth == devmode.dmPelsWidth && g_DevModes.at(i).dmPelsHeight == devmode.dmPelsHeight)
			{
				bContinue = true;
				break;
			}
		}
		if (bContinue)
		{
			bContinue = false;
			continue;
		}
		g_DevModes.push_back(devmode);
	}
}

int sub_427410()
{
	int* pWidths = dword_4F11D4;
	int* pHeights = dword_4F11D8;
	int* pCount = dword_4F111C;

	if ((*pCount) > 0)
	{
		memset(pWidths, 0, *pCount);
		memset(pHeights, 0, *pCount);
		(*pCount) = 0;
	}

	if (g_DevModes.size() == 0)
		InitDisplayResolutions();

	for (const auto &f : g_DevModes)
	{
		int idx = 75 * (*pCount);
		int v6 = 60 * (*pCount);

		pWidths[idx] = f.dmPelsWidth;
		pHeights[idx] = f.dmPelsHeight;

		dword_4F11D0[idx] = (*pCount);
		dword_4F11DC[5 * v6 / 4u] = (*pCount);

		++(*pCount);
	}

	return 1;
}

// CD Check
int sub_4431A0()
{
	//((int(*)())0x004431A0)();
	return 1;
}

// Skip woncomm check
void __fastcall sub_4769B0(void* This)
{
}

void SetCompleteHook(BYTE head, DWORD offset, ...)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 5, PAGE_EXECUTE_READWRITE, &OldProtect);

	if (head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset + 1;

	*(DWORD*)(offset + 1) = (*function) - (offset + 5);

	VirtualProtect((void*)offset, 5, OldProtect, &OldProtect);
}

void HookCD()
{
	SetCompleteHook(0xE9, 0x004431A0, &sub_4431A0);
	SetCompleteHook(0xE9, 0x004769B0, &sub_4769B0);
	SetCompleteHook(0xE9, 0x00427410, &sub_427410);
	
	dword_4F11D4 = (int*)0x004F11D4;
	dword_4F11D8 = (int*)0x004F11D8;
	dword_4F111C = (int*)0x004F111C;
	byte_4F11FC = (char*)(0x004F11FC);

	dword_4F11D0 = (int*)(0x004F11D0);
	dword_4F11DC = (int*)(0x004F11DC);
}

int InitHook()
{
	HookCD();

	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	if (MH_CreateHook(&GetSystemTime, &HOOKED_GetSystemTime,
		reinterpret_cast<LPVOID*>(&ORIG_GetSystemTime)) != MH_OK)
	{
		return 1;
	}

	if (MH_EnableHook(&GetSystemTime) != MH_OK)
	{
		return 1;
	}

	return 0;
}

int FreeHook()
{
	if (MH_Uninitialize() != MH_OK)
	{
		return 1;
	}
	return 0;
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
		FreeHook();
	}
	break;
	}
	return TRUE;
}