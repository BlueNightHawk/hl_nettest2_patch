#include "pch.h"

CIniReader::CIniReader(const char* szFileName)
{
	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, szFileName, strlen(szFileName));
}
int CIniReader::ReadInteger(const char* szSection, const char* szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileIntA(szSection, szKey, iDefaultValue, m_szFileName);
	return iResult;
}
float CIniReader::ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	float fltResult;
	sprintf(szDefault, "%f", fltDefaultValue);
	GetPrivateProfileStringA(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	fltResult = atof(szResult);
	return fltResult;
}
bool CIniReader::ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	bool bolResult;
	sprintf(szDefault, "%s", bolDefaultValue ? "True" : "False");
	GetPrivateProfileStringA(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	bolResult = (strcmp(szResult, "True") == 0 ||
		strcmp(szResult, "true") == 0) ? true : false;
	return bolResult;
}
char* CIniReader::ReadString(const char* szSection, const char* szKey, const char* szDefaultValue)
{
	char* szResult = new char[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileStringA(szSection, szKey,
		szDefaultValue, szResult, 255, m_szFileName);
	return szResult;
}

ULONG CIniReader::ReadULong(const char* szSection, const char* szKey, ULONG iDefaultValue)
{
	auto str = ReadString(szSection, szKey, "0");

	return strtoul(str, NULL, 0);
}