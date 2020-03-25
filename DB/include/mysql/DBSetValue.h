#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifdef _UNICODE
#define DBSetValue DBSetValueA
#else
#define DBSetValue DBSetValueA
#endif


//////////////////////////////////////////////////////////////////////////
// mbcs data

inline void DBSetValueA(char & des, const char * value, char def = 0)
	{ des = (value) ? (char)atoi(value) : def; }

inline void DBSetValueA(short & des, const char * value, short def = 0)
	{ des = (value) ? (short)atoi(value) : def; }

inline void DBSetValueA(int & des, const char * value, int def = 0)
	{ des = (value) ? atoi(value) : def; }

inline void DBSetValueA(long & des, const char * value, long def = 0)
	{ des = (value) ? atol(value) : def; }

inline void DBSetValueA(__int64 & des, const char * value, __int64 def = 0)
	{ des = (value) ? _atoi64(value) : def; }

inline void DBSetValueA(unsigned char & des, const char * value, unsigned char def = 0)
	{ des = (value) ? (unsigned char)atoi(value) : def; }

inline void DBSetValueA(unsigned short & des, const char * value, unsigned short def = 0)
	{ des = (value) ? (unsigned short)atoi(value) : def; }

inline void DBSetValueA(unsigned int & des, const char * value, unsigned int def = 0)
	{ des = (value) ? (unsigned int)_atoi64(value) : def; }

inline void DBSetValueA(unsigned long & des, const char * value, unsigned long def = 0)
	{ des = (value) ? (unsigned long)_atoi64(value) : def; }

inline void DBSetValueA(unsigned __int64 & des, const char * value, unsigned __int64 def = 0)
	{ des = (value) ? (unsigned __int64)_strtoui64(value, NULL, 10) : def; }

inline void DBSetValueA(float & des, const char * value, float def = 0)
	{ des = (value) ? (float)atof(value) : def; }

inline void DBSetValueA(double & des, const char * value, double def = 0)
	{ des = (value) ? atof(value) : def; }

inline void DBSetValueA(std::string & des, const char * value,  const char * def = "")
{
	des = (value) ? std::string(value) : std::string(def);
}


//////////////////////////////////////////////////////////////////////////
// unicode data

inline void DBSetValueW(char & des, const wchar_t * value, char def = 0)
	{ des = (value) ? (char)_wtoi(value) : def; }

inline void DBSetValueW(short & des, const wchar_t * value, short def = 0)
	{ des = (value) ? (short)_wtoi(value) : def; }

inline void DBSetValueW(int & des, const wchar_t * value, int def = 0)
	{ des = (value) ? _wtoi(value) : def; }

inline void DBSetValueW(long & des, const wchar_t * value, long def = 0)
	{ des = (value) ? _wtol(value) : def; }

inline void DBSetValueW(__int64 & des, const wchar_t * value, __int64 def = 0)
	{ des = (value) ? _wtoi64(value) : def; }

inline void DBSetValueW(unsigned char & des, const wchar_t * value, unsigned char def = 0)
	{ des = (value) ? (unsigned char)_wtoi(value) : def; }

inline void DBSetValueW(unsigned short & des, const wchar_t * value, unsigned short def = 0)
	{ des = (value) ? (unsigned short)_wtoi(value) : def; }

inline void DBSetValueW(unsigned int & des, const wchar_t * value, unsigned int def = 0)
	{ des = (value) ? (unsigned int)_wtoi64(value) : def; }

inline void DBSetValueW(unsigned long & des, const wchar_t * value, unsigned long def = 0)
	{ des = (value) ? (unsigned long)_wtoi64(value) : def; }

inline void DBSetValueW(unsigned __int64 & des, const wchar_t * value, unsigned __int64 def = 0)
	{ des = (value) ? (unsigned __int64)_wcstoui64(value, NULL, 10) : def; }

inline void DBSetValueW(float & des, const wchar_t * value, float def = 0)
{
	// (value) ? swscanf(value, L"&f", des) : des = def;
	des = (value) ? (float)_wtof(value) : def;
}

inline void DBSetValueW(double & des, const wchar_t * value, double def = 0)
{
	des = (value) ? _wtof(value) : def;
}

inline void DBSetValueW(std::string & des, const wchar_t * value, const wchar_t * def = L"")
	{ /*des = (value) ? value : def;*/ }

// wchar_t to string
// void Wchar_tToString(std::string& szDst, wchar_t *wchar)
// {
// 	wchar_t * wText = wchar;
// 	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
// 	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
// 	psText = new char[dwNum];
// 	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
// 	szDst = psText;// std::string赋值
// 	delete[]psText;// psText的清除
// }

//////////////////////////////////////////////////////////////////////////
// utf8 data

inline void DBSetValueU(char & des, const char * value, char def = 0)
	{ des = (value) ? (char)atoi(value) : def; }

inline void DBSetValueU(short & des, const char * value, short def = 0)
	{ des = (value) ? (short)atoi(value) : def; }

inline void DBSetValueU(int & des, const char * value, int def = 0)
	{ des = (value) ? atoi(value) : def; }

inline void DBSetValueU(long & des, const char * value, long def = 0)
	{ des = (value) ? atol(value) : def; }

inline void DBSetValueU(__int64 & des, const char * value, __int64 def = 0)
	{ des = (value) ? _atoi64(value) : def; }

inline void DBSetValueU(unsigned char & des, const char * value, unsigned char def = 0)
	{ des = (value) ? (unsigned char)atoi(value) : def; }

inline void DBSetValueU(unsigned short & des, const char * value, unsigned short def = 0)
	{ des = (value) ? (unsigned short)atoi(value) : def; }

inline void DBSetValueU(unsigned int & des, const char * value, unsigned int def = 0)
	{ des = (value) ? (unsigned int)_atoi64(value) : def; }

inline void DBSetValueU(unsigned long & des, const char * value, unsigned long def = 0)
	{ des = (value) ? (unsigned long)_atoi64(value) : def; }

inline void DBSetValueU(unsigned __int64 & des, const char * value, unsigned __int64 def = 0)
	{ des = (value) ? (unsigned __int64)_strtoui64(value, NULL, 10) : def; }

inline void DBSetValueU(float & des, const char * value, float def = 0)
	{ des = (value) ? (float)atof(value) : def; }

inline void DBSetValueU(double & des, const char * value, double def = 0)
	{ des = (value) ? atof(value) : def; }

inline void DBSetValueU(std::string & des, const char * value, const char * def = "")
{
	des = (value) ? std::string(value) : std::string(def);
}