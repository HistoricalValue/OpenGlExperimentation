#ifndef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL_INL__H__
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL_INL__H__

#include <my/gl/textures/TextureUnitWrapper.h>
#include <my/gl/extensions/ExtensionManager.h>

#include <PConfigurations.h>
#include <PAssert.h>

namespace my { namespace gl { namespace textures {
	namespace TextureUnitWrapper {

		using extensions::ExtensionManager::glActiveTexture;

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__DEFINING)


		P_INLINE
		bool IsValid (TextureUnitId const textureUnit) {
			return false
				|| textureUnit == GL_TEXTURE0	|| textureUnit == GL_TEXTURE1
				|| textureUnit == GL_TEXTURE2	|| textureUnit == GL_TEXTURE3
				|| textureUnit == GL_TEXTURE4	|| textureUnit == GL_TEXTURE5
				|| textureUnit == GL_TEXTURE6	|| textureUnit == GL_TEXTURE7
				|| textureUnit == GL_TEXTURE8	|| textureUnit == GL_TEXTURE9
				|| textureUnit == GL_TEXTURE10	|| textureUnit == GL_TEXTURE11
				|| textureUnit == GL_TEXTURE12	|| textureUnit == GL_TEXTURE13
				|| textureUnit == GL_TEXTURE14	|| textureUnit == GL_TEXTURE15
				|| textureUnit == GL_TEXTURE16	|| textureUnit == GL_TEXTURE17
				|| textureUnit == GL_TEXTURE18	|| textureUnit == GL_TEXTURE19
				|| textureUnit == GL_TEXTURE20	|| textureUnit == GL_TEXTURE21
				|| textureUnit == GL_TEXTURE22	|| textureUnit == GL_TEXTURE23
				|| textureUnit == GL_TEXTURE24	|| textureUnit == GL_TEXTURE25
				|| textureUnit == GL_TEXTURE26	|| textureUnit == GL_TEXTURE27
				|| textureUnit == GL_TEXTURE28	|| textureUnit == GL_TEXTURE29
				|| textureUnit == GL_TEXTURE30	|| textureUnit == GL_TEXTURE31
				;
		}

		P_INLINE
		bool IsActive (TextureUnitId const textureUnit) {
			PASSERT(IsValid(textureUnit))

			return GetActive() == textureUnit;
		}

		P_INLINE
		TextureUnitId GetActive (void) {
			GLint activeTextureUnit(0xffffffff);

			glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTextureUnit);

			PASSERT(activeTextureUnit != 0xffffffff);
			PASSERT(IsValid(activeTextureUnit));

			return activeTextureUnit;
		}

		P_INLINE
		void Activate (TextureUnitId const textureUnit) {
			PASSERT(!IsActive(textureUnit)) // -> IsValid
			glActiveTexture(textureUnit);
		}

		P_INLINE
		void ActivateIfInactive (TextureUnitId const textureUnit) {
			if (!IsActive(textureUnit)) // -> IsValid
				glActiveTexture(textureUnit);
		}

		P_INLINE
		void Deactivate (TextureUnitId const textureUnit) {
			PASSERT(IsActive(textureUnit)) // -> IsValid
			glActiveTexture(GL_TEXTURE0);
		}

		P_INLINE
		void DeactivateIfActive (TextureUnitId const textureUnit) {
			if (IsActive(textureUnit)) // -> IsValid
				glActiveTexture(GL_TEXTURE0);
		}


#endif

	} // namespace TextureUnitWrapper
}}} // namespace my::gl::texture

#endif // __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL_INL__H__
