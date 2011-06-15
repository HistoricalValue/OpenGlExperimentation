#ifndef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_INL__H__
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_INL__H__

#include "my/gl/textures/TextureUnit.h"
#include "my/gl/textures/TextureUnitUtil.h"

#include "PConfigurations.h"
#include "PStaticAssert.h"
#include "PAssert.h"


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
		DeactivateIfActive();
	}
	
	P_INLINE
	bool TextureUnit::IsValid (void) const {
		return TextureUnitUtil::IsValid(textureUnit);
	}
	
	P_INLINE
	bool TextureUnit::IsActive (void) const {
		return TextureUnitUtil::IsActive(textureUnit);
	}
	
	P_INLINE
	void TextureUnit::Activate (void) {
		TextureUnitUtil::Activate(textureUnit);
	}
	
	P_INLINE
	void TextureUnit::ActivateIfInactive (void) {
		TextureUnitUtil::ActivateIfInactive(textureUnit);
	}
	
	P_INLINE
	void TextureUnit::Deactivate (void) {
		TextureUnitUtil::Deactivate(textureUnit);
	}
	
	P_INLINE
	void TextureUnit::DeactivateIfActive (void) {
		TextureUnitUtil::DeactivateIfActive(textureUnit);
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