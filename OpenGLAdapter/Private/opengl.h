#ifdef __ANKH__OPEN_GL__H__BEING_INCLUDED__
#	error "Circular inclusion"
#endif
#define __ANKH__OPEN_GL__H__BEING_INCLUDED__

#ifndef __ANKH__OPEN_GL__H__INCLUDED__
#define __ANKH__OPEN_GL__H__INCLUDED__

#include "ankh/config.h"

// OpenGL and libs/utils
// ---------------------
//#include "GL/glew.h"
#if defined( __ANKH__LINUX__ )
//#	include "GL/glxew.h"
#	include "Private/glxext.h"
#	include <GL/gl.h>
#	include <GL/glu.h>
#	include <GL/glx.h>
#elif defined( __ANKH__WIN32__ )
#	include <gl/GL.h>
#	include <gl/GLU.h>
#	include "Private/wglext.h"
#else
#	error "Unknown platform"
#endif
//#define GL_GLEXT_PROTOTYPES
#include "Private//glext.h"



#endif //	__ANKH__OPEN_GL__H__INCLUDED__
#undef		__ANKH__OPEN_GL__H__BEING_INCLUDED__
