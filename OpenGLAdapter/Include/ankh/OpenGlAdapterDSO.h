// Cylcular inclusion test
#ifdef __ANKH__OPEN_GL_ADAPTER__H__BEING_INCLUDED
#	error "Circular inclusion"
#endif
#define __ANKH__OPEN_GL_ADAPTER__H__BEING_INCLUDED

// Re-inclusion test
#ifndef __ANKH__OPEN_GL_ADAPTER__H__INCLUDED__
#define __ANKH__OPEN_GL_ADAPTER__H__INCLUDED__

#include "PDSO.h"

#ifdef	__ANKH__OPEN_GL_ADAPTER__EXPORTED__
#	define ANKH__OPENGL_ADAPTER__API	CDSHR_DSO_EXPORT
#elif	__ANKH__OPEN_GL_ADAPTER__IMPORTED__
#	define ANKH__OPENGL_ADAPTER__API	CDSHR_DSO_IMPORT
#else
#	error "Ankh::OpenGlAdapter not exported or imported"
#endif

#endif //	__ANKH__OPEN_GL_ADAPTER__H__INCLUDED__
#undef		__ANKH__OPEN_GL_ADAPTER__H__BEING_INCLUDED

