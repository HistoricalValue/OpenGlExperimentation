#ifndef __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__
#define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__

#include "my/gl/textures/TextureUnitManager_fwd.h"
#include "MyUtil.h"
#include "PConfigurations.h"
#include "my/gl/textures/TextureUnitUtil.h"
#include "my/gl/textures/TextureUnit_fwd.h"

#define GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS		31u

namespace my { namespace gl { namespace textures {

	namespace TextureUnitManager {
		using TextureUnitUtil::TextureUnitId;

		MYUTIL_API void									Initialise (void);
		MYUTIL_API void									CleanUp (void);

		MYUTIL_API TextureUnit&							GetUnit (TextureUnitId const&);
		MYUTIL_API TextureUnit&							GetActiveUnit (void);

	} // namespace TextureUnitManager

}}} // namespace my::gl::textures

#endif // __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__
