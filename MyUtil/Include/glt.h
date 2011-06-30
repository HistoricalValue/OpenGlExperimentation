#ifndef __MY_UTIL__GLT__H__
#define __MY_UTIL__GLT__H__

#include "MyUtil.h"
#include "MyGL.h"
#include "ImageDecoder.h"

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



	class MYUTIL_API TGADecoder: public ankh::images::ImageDecoder {
	public:

		virtual ankh::images::Image* Decode (
				ankh::images::GenericReader&			r,
				ankh::images::ImageId const&			imgid,
				ankh::images::ImageFormatId const&		fmt,
				ankh::images::ImageSourceType const&	src);
		virtual ankh::images::ImageFormatId const& GetFormatId (void) const;
		virtual bool CanHandleFormat (ankh::images::ImageFormatId const& fmt) const;
		TGADecoder (void);
		virtual ~TGADecoder (void);
	};

}

#endif // __MY_UTIL__GLT__H__
