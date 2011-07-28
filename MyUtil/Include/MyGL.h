#ifndef __MYUTIL__MY_GL__H__
#define __MYUTIL__MY_GL__H__


// OpenGL and libs/utils
// ---------------------
//#include "GL/glew.h"
#if defined( __linux__ )
//#	include "GL/glxew.h"
#	include "glxext.h"
#	include <GL/gl.h>
#	include <GL/glu.h>
#	include <GL/glx.h>
#elif defined( _MSC_VER )
#	include <gl/GL.h>
#	include <gl/GLU.h>
#	include "gl/wglext.h"
#else
#	error "Unknown platform"
#endif
//#define GL_GLEXT_PROTOTYPES
#include "gl/glext.h"


#endif // __MYUTIL__MY_GL__H__
