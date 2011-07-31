#ifndef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_INL__H__
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_INL__H__

#include <my/gl/textures/TextureUnit.h>
#include <my/gl/textures/TextureUnitWrapper_inl.h>

#include <PConfigurations.h>
#include <PStaticAssert.h>
#include <PAssert.h>


namespace my { namespace gl { namespace textures {

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__DEFINING)


	P_INLINE
	TextureUnit::TextureUnit (TextureUnit const& other):
		textureUnit(other.textureUnit)
	{
		P_STATIC_ASSERT(sizeof(TextureUnit) == 0 + sizeof(textureUnit))
		PASSERT(other.textureUnit != GL_TEXTURE0)
	}

	P_INLINE
	TextureUnit::~TextureUnit (void) {
	}

	P_INLINE
	bool TextureUnit::IsValid (void) const {
		return TextureUnitWrapper::IsValid(textureUnit);
	}

	P_INLINE
	bool TextureUnit::IsActive (void) const {
		return TextureUnitWrapper::IsActive(textureUnit);
	}

	P_INLINE
	void TextureUnit::Activate (void) {
		TextureUnitWrapper::Activate(textureUnit);
	}

	P_INLINE
	void TextureUnit::ActivateIfInactive (void) {
		TextureUnitWrapper::ActivateIfInactive(textureUnit);
	}

	P_INLINE
	void TextureUnit::Deactivate (void) {
		TextureUnitWrapper::Deactivate(textureUnit);
	}

	P_INLINE
	void TextureUnit::DeactivateIfActive (void) {
		TextureUnitWrapper::DeactivateIfActive(textureUnit);
	}

	P_INLINE
	TextureUnit::TextureUnit (GLenum const& _textureUnit):
		textureUnit(openglutil::ToGlConst(_textureUnit))
	{
		P_STATIC_ASSERT(sizeof(TextureUnit) == 0 + sizeof(textureUnit))
		PASSERT(_textureUnit != GL_TEXTURE0)
	}

#endif

}}} // namespace my::gl::texture

#endif // __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_INL__H__