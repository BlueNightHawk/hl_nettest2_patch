#include "pch.h"

typedef void (WINAPI* GETSYSTEMTIME)(LPSYSTEMTIME);
GETSYSTEMTIME ORIG_GetSystemTime = NULL;
void WINAPI HOOKED_GetSystemTime(LPSYSTEMTIME lpSystemTime)
{
	ORIG_GetSystemTime(lpSystemTime);
	lpSystemTime->wYear = 1998;
	lpSystemTime->wMonth = 10;
}

// Enable all resolutions

// Display Widths
int* pWidths;
// Display Heights
int* pHeights;
// Display Mode Count
int* pCount;

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

int HOOKED_RESOLUTIONS()
{
	if ((*pCount) > 0)
	{
		memset(pWidths, 0, *pCount);
		memset(pHeights, 0, *pCount);
		(*pCount) = 0;
	}

	if (g_DevModes.size() == 0)
		InitDisplayResolutions();

	for (const auto& f : g_DevModes)
	{
		int idx = 75 * (*pCount);
		int v6 = 60 * (*pCount);

		pWidths[idx] = f.dmPelsWidth;
		pHeights[idx] = f.dmPelsHeight;

		++(*pCount);
	}

	return 1;
}

// CD Check
int HOOKED_CDCHECK()
{
	return 1;
}

// Skip woncomm check
void __fastcall HOOKED_WONCOMM(void* This)
{
}

BOOL ChangeDisplayMode(DWORD width, DWORD height, DWORD bpp)
{
	DEVMODEA DevMode; 

	memset(&DevMode, 0, sizeof(DevMode));
	DevMode.dmSize = sizeof(DevMode);
	DevMode.dmPelsWidth = width;
	DevMode.dmPelsHeight = height;
	DevMode.dmBitsPerPel = bpp;
	DevMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY | DM_POSITION;
	return ChangeDisplaySettingsA(&DevMode, CDS_FULLSCREEN) == 0;
}

BOOL ChangeDisplayModeByIndex(int idx)
{
	DEVMODE DevMode = g_DevModes.at(idx);

	DevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	return ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN) == 0;
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

void HookExecutable()
{
	auto ofs = GetOffsetsAuto();

	if (ofs.CdCheck > 0)
		SetCompleteHook(0xE9, ofs.CdCheck, &HOOKED_CDCHECK);
	
	if (ofs.WonComm > 0)
		SetCompleteHook(0xE9, ofs.WonComm, &HOOKED_WONCOMM);

	if (ofs.Resolution > 0 && ofs.WidthPtr > 0 && ofs.HeightPtr && ofs.CountPtr > 0)
	{
		SetCompleteHook(0xE9, ofs.Resolution, &HOOKED_RESOLUTIONS);

		pWidths = (int*)ofs.WidthPtr;
		pHeights = (int*)ofs.HeightPtr;
		pCount = (int*)ofs.CountPtr;
	}

	if (ofs.ChangeDisplayMode > 0)
	{
		SetCompleteHook(0xE9, ofs.ChangeDisplayMode, &ChangeDisplayMode);
	}

	if (ofs.ChangeDisplayModeByIndex > 0)
	{
		SetCompleteHook(0xE9, ofs.ChangeDisplayModeByIndex, &ChangeDisplayModeByIndex);
	}
}

int InitHook()
{
	HookExecutable();

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