#include "pch.h"

static offset_s g_Offsets;

static GETSYSTEMTIME ORIG_GetSystemTime = NULL;

// Enable all resolutions
struct
{
	// Display Width
	int* pWidth;
	// Display Height
	int* pHeight;
	// Display Mode Count
	int* pCount;
} displaymode;

static std::vector<DEVMODE> g_DevModes;

static void InitDisplayResolutions()
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

// Fills the list of game resolutions
static int HOOKED_InitResolutionList()
{
	if ((*displaymode.pCount) > 0)
	{
		memset(displaymode.pWidth, 0, *displaymode.pCount);
		memset(displaymode.pHeight, 0, *displaymode.pCount);
		(*displaymode.pCount) = 0;
	}

	if (g_DevModes.size() == 0)
		InitDisplayResolutions();

	for (const auto& f : g_DevModes)
	{
		int idx = 75 * (*displaymode.pCount);
		int v6 = 60 * (*displaymode.pCount);

		displaymode.pWidth[idx] = f.dmPelsWidth;
		displaymode.pHeight[idx] = f.dmPelsHeight;

		++(*displaymode.pCount);
	}

	return 1;
}

// CD Check
static int HOOKED_CDCHECK()
{
	return 1;
}

// Skip woncomm check
static void __fastcall HOOKED_WONCOMM(void* This)
{
}

// Called when switching to game from main menu
static BOOL HOOKED_ChangeDisplayModeByIndex(int idx)
{
	HOOKED_InitResolutionList();
	DEVMODE DevMode = g_DevModes.at(idx);

	DevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	return ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN) == 0;
}

static void WINAPI HOOKED_GetSystemTime(LPSYSTEMTIME lpSystemTime)
{
	ORIG_GetSystemTime(lpSystemTime);
	lpSystemTime->wYear = (WORD)g_Offsets.Bomb_Year;
	lpSystemTime->wMonth = (WORD)g_Offsets.Bomb_Month;
}

static void SetCompleteHook(BYTE head, DWORD offset, ...)
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

static void HookExecutable()
{
	g_Offsets = GetOffsets();

	if (g_Offsets.CdCheck > 0)
		SetCompleteHook(0xE9, g_Offsets.CdCheck, &HOOKED_CDCHECK);
	
	if (g_Offsets.WonComm > 0)
		SetCompleteHook(0xE9, g_Offsets.WonComm, &HOOKED_WONCOMM);

	if (g_Offsets.Resolution > 0 && g_Offsets.WidthPtr > 0 && g_Offsets.HeightPtr && g_Offsets.CountPtr > 0)
	{
		SetCompleteHook(0xE9, g_Offsets.Resolution, &HOOKED_InitResolutionList);

		displaymode.pWidth = (int*)g_Offsets.WidthPtr;
		displaymode.pHeight = (int*)g_Offsets.HeightPtr;
		displaymode.pCount = (int*)g_Offsets.CountPtr;
	}

	if (g_Offsets.ChangeDisplayModeByIndex > 0)
	{
		SetCompleteHook(0xE9, g_Offsets.ChangeDisplayModeByIndex, &HOOKED_ChangeDisplayModeByIndex);
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