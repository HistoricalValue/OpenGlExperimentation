#include "stdafx.h"

#include "my/gl/textures/TextureObject_inl.h"

#define __NGLE()	PASSERT(!my::openglutil::GlErrorsHandled(::my::global::errorHandler::Get()))

namespace my { namespace gl { namespace textures {

	P_INLINE void TextureObject_Create (TextureObject* const placeIn, TextureObject::Id const& id) {
		new(placeIn) TextureObject(id);
	}

	namespace TextureObjectManager {

		static bool initialised(false);

		void Initialise (void) {
			PASSERT(!initialised)
			initialised = true;
		}

		void CleanUp (void) {
			PASSERT(initialised)
			initialised = false;
		}

		void Create (TextureObject* const placeIn, size_t const bytesize) {

			TextureObject::Id textureId(0xffffffff); {
				glGenTextures(1, &textureId); __NGLE()
			}

			size_t const requiredBytesize(sizeof(*placeIn));
			PASSERT(bytesize >= requiredBytesize)
			if (bytesize >= requiredBytesize) {
				TextureObject_Create(placeIn, textureId);
			}
		}

	} // namespace TextureObjectManager

}}} // namespace my::gl::textures