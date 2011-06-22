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

// Windows declarations first
#include "Private/targetver.h"
#include "PWindows.h"

// OpenGL declarations second
#include "Private/opengl.h"

// Ankh declarations third
#include "ankh/OpenGlAdapterDSO.h"

// Ankh::OpenGL::Adapter declarations fourth
#include "ankh/opengl/adapter/Context.h"
#include "ankh/opengl/adapter/ContextOptions.h"
#include "ankh/opengl/adapter/NativeWindow.h"

// Codeshare::Utilities declarations fifth
#include "PMemory.h"
#include "PPointerUtilities.h"
#include "PAssert.h"
#include "PStaticAssert.h"
#include "PWindowsUtilities.h"
#include "PCString.h"

#endif //	__STDAFX__H__INCLUDED__
#undef		__STDAFX__H__BEING_INCLUDED__

