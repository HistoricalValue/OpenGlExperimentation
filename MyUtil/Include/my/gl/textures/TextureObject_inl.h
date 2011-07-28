#ifndef __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT_INL__H__
#define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT_INL__H__

#include "my/gl/textures/TextureObject.h"
#include "PConfigurations.h"
#include "PStaticAssert.h"

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT__DEFINING)


namespace my { namespace gl { namespace textures {

	P_INLINE
	TextureObject::TextureObject (TextureObject const& other):
		id(other.id),
		isUploaded(other.isUploaded)
	{
		P_STATIC_ASSERT(sizeof(TextureObject) == 3 + sizeof(id) + sizeof(isUploaded))
	}

	P_INLINE
	void TextureObject::operator = (TextureObject const& other) {
		this->~TextureObject();
		new(this) TextureObject(other);
	}

	P_INLINE
	TextureObject::~TextureObject (void) {
	}

	P_INLINE
	bool TextureObject::IsUploaded (void) const {
		return isUploaded;
	}

	P_INLINE
	TextureObject::TextureObject (Id const& _id):
		id(_id),
		isUploaded(false)
	{
		P_STATIC_ASSERT(sizeof(TextureObject) == 3 + sizeof(id) + sizeof(isUploaded))
	}

}}} // namespace my::gl::textures


#endif

#endif // __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT_INL__H__
