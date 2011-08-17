#pragma once

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MYUTIL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MYUTIL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MYUTIL_EXPORTS
#define MYUTIL_API __declspec(dllexport)
#else
#define MYUTIL_API __declspec(dllimport)
#endif

#pragma warning( push, 0 )
#	include <stdlib.h>
#pragma warning( pop )

// This class is exported from the MyUtil.dll
class MYUTIL_API CMyUtil {
public:
	CMyUtil(void);
	// TODO: add your methods here.
};

extern MYUTIL_API int nMyUtil;

MYUTIL_API int fnMyUtil(void);

/////////////////////////////

namespace my {
	extern MYUTIL_API void Initialise (void);
	extern MYUTIL_API void CleanUp (void);
}
