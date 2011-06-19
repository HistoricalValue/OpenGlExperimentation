#ifndef __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT__H__
#define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT__H__

#include "MyUtil.h"
#include "my/gl/textures/TextureUnitWrapper.h"
#include "my/gl/textures/TextureUnit.h"
#include "my/gl/textures/TextureData.h"

namespace my { namespace gl { namespace textures {

	using TextureUnitWrapper::TextureUnitId;

	class MYUTIL_API TextureObject {
	public:
		typedef GLuint									Id;

		P_INLINE										TextureObject (TextureObject const&);
		P_INLINE void									operator = (TextureObject const&);
		P_INLINE 										~TextureObject (void);

		P_INLINE void									Upload (TextureUnit const& to, TextureData const&);

		P_INLINE bool									IsUploaded (void) const;

	private:
		P_INLINE 										TextureObject (Id const&);
		friend void										TextureObject_Create (TextureObject* placeIn, Id const&);

		Id												id;
		bool											isUploaded;
	}; // class TextureObject

}}} // namespace my::gl::textures

#endif // __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT__H__
