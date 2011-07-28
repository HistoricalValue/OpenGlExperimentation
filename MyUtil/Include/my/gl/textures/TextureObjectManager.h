#ifndef __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT_MANAGER__H__
#define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT_MANAGER__H__

#include "MyUtil.h"
#include "my/gl/textures/TextureObject.h"

namespace my { namespace gl { namespace textures {

	namespace TextureObjectManager {
		MYUTIL_API void									Initialise (void);
		MYUTIL_API void									CleanUp (void);


		MYUTIL_API void									Create (TextureObject* placeIn, size_t bytesize);

	} // namespace TextureObjectManager

}}} // namespace my::gl::textures

#endif // __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT_MANAGER__H__

