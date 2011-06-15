#pragma once

#include "targetver.h"

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>
#define NOMINMAX						// do not define macros for min() max()
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#if defined(UNICODE) || defined(_UNICODE)
#	define _T_STR			std::wstring
#	define _T_OSTREAM		std::wostream
#	define _T_OSSTREAM		std::wostringstream
#	define _T_COUT			std::wcout
#	define _T_CERR			std::wcerr
#	define _T_OFSTREAM		std::wofstream
#else
#	define _T_STR			std::string
#	define _T_OSTREAM		std::ostream
#	define _T_OSSTREAM		std::ostringstream
#	define _T_COUT			std::cout
#	define _T_CERR			std::cerr
#	define _T_OFSTREAM		std::ofstream
#endif
