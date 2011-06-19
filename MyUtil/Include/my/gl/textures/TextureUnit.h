#ifdef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__CYCLIC_INCLUSION_ERROR
#	error "Cyclic inclusion error"
#endif
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__CYCLIC_INCLUSION_ERROR

#ifndef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__

#include "my/gl/textures/TextureUnit_fwd.h"
#include "MyOpenGLUtils.h"
#include "my/gl/textures/TextureUnitWrapper.h"
#include "my/gl/textures/TextureUnitManager_fwd.h"

#include "PConfigurations.h"

namespace my { namespace gl { namespace textures {

	using TextureUnitWrapper::TextureUnitId;

	class MYUTIL_API TextureUnit {
	public:

		P_INLINE										TextureUnit (TextureUnit const&);
		P_INLINE										~TextureUnit (void);
														
		P_INLINE bool									IsValid (void) const;
		P_INLINE bool									IsActive (void) const;
		
		P_INLINE void									Activate (void);
		P_INLINE void									ActivateIfInactive (void); // TODO: refactor: make utility (uses public API) - nm 2011/06/15
		P_INLINE void									Deactivate (void);
		P_INLINE void									DeactivateIfActive (void); // TODO: refactor: make utility (uses public API) - nm 2011/06/15

		P_INLINE TextureUnitId							GetId (void) const;

	private:
		friend class TextureUnitManager;

		P_INLINE										TextureUnit (TextureUnitId const& textureUnit);
		P_INLINE void									operator = (TextureUnit const&);
		
		enum openglutil::GlConst const					textureUnit;
	}; // class TextureUnit

}}} // namespace my::gl::texture

#endif // __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__

#undef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT__H__CYCLIC_INCLUSION_ERROR