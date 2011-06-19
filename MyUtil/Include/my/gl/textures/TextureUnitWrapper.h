#ifndef __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__H__
#define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__H__

#include "MyUtil.h"
#include "PConfigurations.h"

#ifdef P_INLINING
#	define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE	P_INLINE
#else
#	define __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE	MYUTIL_API
#endif

namespace my { namespace gl { namespace textures {

	namespace TextureUnitWrapper {

		typedef GLenum									TextureUnitId;
	
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		bool 											IsValid (TextureUnitId textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		bool											IsActive (TextureUnitId textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		TextureUnitId									GetActive (void);

		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE 
		void											Activate (TextureUnitId textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		void											ActivateIfInactive (TextureUnitId textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		void											Deactivate (TextureUnitId textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		void											DeactivateIfActive (TextureUnitId textureUnit);
		
	} // namespace TextureUnitWrapper

}}} // namespace my::gl::texture

#endif // __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__H__
