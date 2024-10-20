#pragma once

typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY)(REFIID riid, _COM_Outptr_ void** ppFactory);
typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY_2)(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory);
typedef HRESULT(WINAPI* PFN_DECLARE_ADAPTER_REMOVAL_SUPPORT)();
typedef HRESULT(WINAPI* PFN_GET_DEBUG_INTERFACE)(UINT Flags, REFIID riid, void** ppDebug);
