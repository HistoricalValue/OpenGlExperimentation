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

	namespace TextureUnitUtil {
	
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		bool 											IsValid (GLenum const textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		bool											IsActive (GLenum const textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE 
		void											Activate (GLenum const textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		void											ActivateIfInactive (GLenum const textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		void											Deactivate (GLenum const textureUnit);
		
		__MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__LINKAGE
		void											DeactivateIfActive (GLenum const textureUnit);
		
	} // namespace TextureUnitUtil

}}} // namespace my::gl::texture

#endif // __MY_UTIL__MY__GL__TEXTURE__TEXTURE_UNIT_UTIL__H__
