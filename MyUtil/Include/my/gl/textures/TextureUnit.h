#ifndef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__

#include "my/gl/textures/TextureUnit_fwd.h"
#include "MyOpenGLUtils.h"
#include "my/gl/textures/TextureUnitUtil.h"
#include "my/gl/textures/TextureUnitManager.h"

#include "PConfigurations.h"

namespace my { namespace gl { namespace textures {

	using TextureUnitUtil::TextureUnitId;

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

		P_INLINE TextureUnitId							GetId (void) const;

	private:
		friend TextureUnit& TextureUnitManager::GetUnit (TextureUnitId const&);

		P_INLINE										TextureUnit (TextureUnitId const& textureUnit);
		P_INLINE void									operator = (TextureUnit const&);
		
		enum openglutil::GlConst const					textureUnit;
	}; // class TextureUnit

}}} // namespace my::gl::texture

#endif // __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__