// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef __STDAFX__H__BEING_INCLUDED__
#	error "Circular inclusion of stdafx.h (how did you do that?)"
#endif
#define __STDAFX__H__BEING_INCLUDED__

#ifndef __STDAFX__H__INCLUDED__
#define __STDAFX__H__INCLUDED__

#include "Private/targetver.h"

#include "PWindows.h"

#include "miniutil/list.h"
#include "miniutil/memory.h"

#endif //	__STDAFX__H__INCLUDED__
#undef		__STDAFX__H__BEING_INCLUDED__

