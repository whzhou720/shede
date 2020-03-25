#pragma once


// #include <winsock2.h>
// #pragma comment(lib, "Ws2_32.lib")
// 
// #include <time.h>
// 
// #include <mmsystem.h>
// #pragma comment(lib, "winmm.lib")


#include <wtypes.h>

#include <stdio.h>
#include <stdlib.h> // atol
#include <stdint.h> // int32_t ...

#include <memory.h>

#include <io.h>
#include <iostream> // std::cout, std::ios, std::wostream
#include <string>
#include <locale>
#include <codecvt>

#include <xlocbuf>
#include <vector>

#include <cctype>
#include <algorithm>

#include <fstream>
#include <sstream>

#include <map>
#include <list>

#undef assert
extern "C" _CRTIMP void __cdecl _assert(void *, void *, unsigned);
#define  assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )

#if defined(DEBUG) || defined(_DEBUG)
#define  Assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#define AssertD(exp) ((void)0)
#else
#if !defined(NDEBUG)
#error ERROR: need define NDEBUG
#endif
#define  Assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#define AssertD(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#endif

#define SAFE_DELETE(x)			{ if (NULL != x) delete x; x = NULL; }
#define SAFE_ARRAY_DELETE(x)	{ if (NULL != x) delete [] x; x = NULL; }
#define ZeroData(d)				memset((&d), 0, sizeof(d))
#define CountOf(t)				(sizeof(t) / sizeof((t)[0]))

#if (defined(_WIN32) || defined(WIN32)) && !defined(_CRT_SECURE_NO_WARNINGS)
/* This needs to come before any includes for MSVC compiler */
#define _CRT_SECURE_NO_WARNINGS
#endif