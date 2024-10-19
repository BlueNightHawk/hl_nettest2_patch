#include "pch.h"

offset_s GetOffsets(const char* version)
{
	static offset_s s;
	s = {};
	CIniReader ini(".\\dxgi.ini");

	s.CdCheck = ini.ReadULong(version, "CdCheck", 0);
	s.WonComm = ini.ReadULong(version, "WonCommCheck", 0);
	s.Resolution = ini.ReadULong(version, "ResolutionList", 0);

	s.WidthPtr = ini.ReadULong(version, "WidthPtr", 0);
	s.HeightPtr = ini.ReadULong(version, "HeightPtr", 0);
	s.CountPtr = ini.ReadULong(version, "CountPtr", 0);
	return s;
}

const char* GetFileVersionOfApplication(LPCSTR lpszFilePath)
{
	static char ver[30];
	//give your application full path
	DWORD dwDummy;
	DWORD dwFVISize = GetFileVersionInfoSizeA(lpszFilePath, &dwDummy);

	LPBYTE lpVersionInfo = new BYTE[dwFVISize];

	GetFileVersionInfoA(lpszFilePath, 0, dwFVISize, lpVersionInfo);

	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi;

	VerQueryValue(lpVersionInfo, L"\\", (LPVOID*)&lpFfi, &uLen);

	DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
	DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;

	delete[] lpVersionInfo;

	printf("Higher: %x\n", dwFileVersionMS);

	printf("Lower: %x\n", dwFileVersionLS);

	DWORD dwLeftMost = HIWORD(dwFileVersionMS);
	DWORD dwSecondLeft = LOWORD(dwFileVersionMS);
	DWORD dwSecondRight = HIWORD(dwFileVersionLS);
	DWORD dwRightMost = LOWORD(dwFileVersionLS);

	sprintf(ver, "%d.%d.%d.%d", dwLeftMost, dwSecondLeft,
		dwSecondRight, dwRightMost);

	return ver;
}

offset_s GetOffsetsAuto()
{
	static offset_s s;
	s = {};
	CIniReader ini(".\\dxgi.ini");

	auto detection = ini.ReadString("SETTINGS", "Detection", "AUTO");
	const char* version = nullptr;
	if (!strcmp(detection, "MANUAL"))
	{
		version = ini.ReadString("SETTINGS", "Version", "NETTEST2");
	}
	else
	{
		const char *executable = ini.ReadString("SETTINGS", "Executable", "hl.exe");
		char exepath[MAX_PATH];
		sprintf(exepath, ".\\%s", executable);

		version = GetFileVersionOfApplication(exepath);
	}

	s.CdCheck = ini.ReadULong(version, "CdCheck", 0);
	s.WonComm = ini.ReadULong(version, "WonCommCheck", 0);
	s.Resolution = ini.ReadULong(version, "ResolutionList", 0);
	s.ChangeDisplayMode = ini.ReadULong(version, "ChangeDisplayMode", 0);
	s.ChangeDisplayModeByIndex = ini.ReadULong(version, "CDMByIndex", 0);

	s.WidthPtr = ini.ReadULong(version, "WidthPtr", 0);
	s.HeightPtr = ini.ReadULong(version, "HeightPtr", 0);
	s.CountPtr = ini.ReadULong(version, "CountPtr", 0);

	s.Bomb_Year = ini.ReadULong(version, "TimeBomb_Year", 0);
	s.Bomb_Month = ini.ReadULong(version, "TimeBomb_Month", 0);
	return s;
}