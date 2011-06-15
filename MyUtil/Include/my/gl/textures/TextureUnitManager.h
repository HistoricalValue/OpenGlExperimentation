#ifndef __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__
#define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__

#include "my/gl/textures/TextureUnitManager_fwd.h"
#include "MyUtil.h"
#include "PConfigurations.h"
#include "my/gl/textures/TextureUnitUtil.h"
#include "my/gl/textures/TextureUnit_fwd.h"

#ifdef P_INLINING
#	define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__LINKAGE	P_INLINE
#else
#	define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__LINKAGE	MYUTIL_API
#endif

#define GL__TEXUTRE_UNIT_MANAGER__MAX_TEXTURE_UNITS		31u

namespace my { namespace gl { namespace textures {

	namespace TextureUnitManager {
		using TextureUnitUtil::TextureUnitId;

		__MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__LINKAGE
		void											Initialise (void);
		__MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__LINKAGE
		void											CleanUp (void);

		__MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__LINKAGE
		TextureUnit&									GetUnit (TextureUnitId const&);

		__MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__LINKAGE
		TextureUnit&									GetActiveUnit (void);

	} // namespace TextureUnitManager

}}} // namespace my::gl::textures

#endif // __MY_UTIL__MY__GL__TEXTURES__TEXTURE_UNIT_MANAGER__H__
