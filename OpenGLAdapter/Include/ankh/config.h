#ifdef __ANKH__OPEN_GL__ADAPTER__CONFIG__H__BEING_INCLUDED__
#	error "Circular inclusion"
#endif
#define __ANKH__OPEN_GL__ADAPTER__CONFIG__H__BEING_INCLUDED__


#ifndef __ANKH__OPEN_GL__ADAPTER__CONFIG__H__INCLUDED__
#define __ANKH__OPEN_GL__ADAPTER__CONFIG__H__INCLUDED__


#include "PCompiler.h"

#if defined(WIN32)
#	define __ANKH__WIN32__
#elif defined(LINUX)
#	define __ANKH__LINUX__
#endif

#endif //	__ANKH__OPEN_GL__ADAPTER__CONFIG__H__INCLUDED__
#undef		__ANKH__OPEN_GL__ADAPTER__CONFIG__H__BEING_INCLUDED__

