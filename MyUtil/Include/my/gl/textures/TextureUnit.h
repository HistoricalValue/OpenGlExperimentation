#ifndef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__

#include "MyUtil.h"
#include "MyOpenGLUtils.h"

#include "PConfigurations.h"

namespace my { namespace gl { namespace textures {

	class MYUTIL_API TextureUnit {
	public:
		
		P_INLINE										TextureUnit (TextureUnit const&);
		P_INLINE										~TextureUnit (void);
														
		P_INLINE bool									IsValid (void) const;
		P_INLINE bool									IsActive (void) const;
		
		P_INLINE void									Activate (void);
		P_INLINE void									ActivateIfInactive (void);
		P_INLINE void									Deactivate (void);
		P_INLINE void									DeactivateIfActive (void);

		P_INLINE										TextureUnit (GLenum const& textureUnit); // TODO make private
	private:
		
		P_INLINE void									operator = (TextureUnit const&);
		
		enum openglutil::GlConst const					textureUnit;
	}; // class TextureUnit

}}} // namespace my::gl::texture

#endif // __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__