#ifndef __MY_UTIL__MY_OPEN_GL_UTIL__H__
#define __MY_UTIL__MY_OPEN_GL_UTIL__H__

#include "MyUtil.h"
#include "MyWindows.h"

#include "PConfigurations.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#if defined( P_INLINING )
#	define __MY_OPEN_GL_UTILS__CONST_STRING_LINKING		P_INLINE
#else
#	define __MY_OPEN_GL_UTILS__CONST_STRING_LINKING		MYUTIL_API
#endif

namespace my {

// TODO remove all -- they have been moved to nmutil:: in ImageLoader
	namespace openglutil {
		enum GlConst {
			MY_GL_ALPHA_TEST		= GL_ALPHA_TEST,
			MY_GL_AUTO_NORMAL		= GL_AUTO_NORMAL,
			MY_GL_BLEND				= GL_BLEND,
			//
			MY_GL_RGB				= GL_RGB,
			MY_GL_BGR				= GL_BGR,
			MY_GL_BGRA				= GL_BGRA,
			MY_GL_LUMINANCE			= GL_LUMINANCE
		};

		P_INLINE
		MYUTIL_API enum GlConst ToGlConst (GLenum const&);

		MYUTIL_API LPCTSTR GlConstString (enum GlConst const& k);

		template <enum GlConst const K>
		__MY_OPEN_GL_UTILS__CONST_STRING_LINKING LPCTSTR GlConstString (void);


		enum GlError {
			MY_GL_INVALID_ENUM		= GL_INVALID_ENUM,		// An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag.
			MY_GL_INVALID_VALUE		= GL_INVALID_VALUE,		// A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag.
			MY_GL_INVALID_OPERATION	= GL_INVALID_OPERATION,	// The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag.
			MY_GL_NO_ERROR			= GL_NO_ERROR,			// No error has been recorded. The value of this symbolic constant is guaranteed to be zero.
			MY_GL_STACK_OVERFLOW	= GL_STACK_OVERFLOW,	// This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag.
			MY_GL_STACK_UNDERFLOW	= GL_STACK_UNDERFLOW,	// This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag.
			MY_GL_OUT_OF_MEMORY		= GL_OUT_OF_MEMORY		// There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded.
		};

		MYUTIL_API LPCTSTR GlErrorString (enum GlError const& e);

		template <enum GlError const E>
		__MY_OPEN_GL_UTILS__CONST_STRING_LINKING LPCTSTR GlErrorString (void);


		// ----------------------------
		// Nullifiers
		template <typename T>
		MYUTIL_API void nullify (T&);
		template <typename T>
		MYUTIL_API bool isnull (T const&);
		template <typename T>
		MYUTIL_API T null (void);


		// ----------------------------

		MYUTIL_API GLchar* GetProgramInfoLog (GLuint program);
		MYUTIL_API GLchar* GetShaderInfoLog (GLuint shader);

		MYUTIL_API void ReleaseShaderInfoLog (void);

		// ----------------------------

		MYUTIL_API bool GlErrorsHandled (void (*handler) (LPCTSTR));

	} // namespace openglutil
} // namespace my

#endif // __MY_UTIL__MY_OPEN_GL_UTIL__H__
