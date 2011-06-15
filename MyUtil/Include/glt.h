#ifndef __MY_UTIL__GLT__H__
#define __MY_UTIL__GLT__H__

#include "MyUtil.h"
#include "MyGL.h"

namespace glt {
	extern MYUTIL_API GLbyte *ReadTGABits(
			const char*	szFileName,
			void*		(*memalloc) (size_t),
			void		(*memfree) (void*),
			GLenum*		eFormat,
			GLint*		iWidth,
			GLint*		iHeight,
			GLshort*	pDepth						= NULL,
			GLuint*		pImageSize					= NULL,
			GLint*		iComponents					= NULL,
			GLbyte*		pData						= NULL);
}

#endif // __MY_UTIL__GLT__H__
