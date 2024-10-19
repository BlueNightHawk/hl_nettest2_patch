#pragma once

class CIniReader
{
public:
	CIniReader(const char* szFileName);
	int ReadInteger(const char* szSection, const char* szKey, int iDefaultValue);
	float ReadFloat(const char* szSection, const  char* szKey, float fltDefaultValue);
	bool ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue);
	char* ReadString(const char* szSection, const char* szKey, const char* szDefaultValue);
	ULONG ReadULong(const char* szSection, const char* szKey, ULONG iDefaultValue);
private:
	char m_szFileName[255];
};
