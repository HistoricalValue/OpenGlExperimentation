// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef STDAFX_H
#define STDAFX_H

#ifdef _MSC_VER
//#	pragma once
#	pragma warning( push, 0 )
#endif

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include <string>
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <functional>
#include <sstream>

#include <ufunctors.h>
#include <usig.h>
#include <utypes.h>
#include <uptr.h>
#include <usystem.h>

#include <PStaticAssert.h>
#include <PProtectedClass_inl.h>
#include <PAllocators_inl.h>
#include <PFunctors_inl.h>
#include <PError.h>
#include <PSingleAllocationBuffer.h>
#include <PGlobalSingleAllocationBuffer.h>
#include <PCString.h>
#include <PStaticCString_inl.h>
#include <PPointerUtilities_inl.h>
#include <PConfigurations.h>

#ifdef _MSC_VER
#	pragma warning( pop )
#endif

#endif // STDAFX_H
