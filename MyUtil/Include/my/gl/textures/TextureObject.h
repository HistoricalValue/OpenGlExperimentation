#ifndef __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT__H__
#define __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT__H__

#include "MyUtil.h"
#include "my/gl/textures/TextureUnit.h"

namespace my { namespace gl { namespace textures {

	using TextureUnitUtil::TextureUnitId;

	class MYUTIL_API TextureObject {
	public:
		typedef GLuint									Id;

														TextureObject (Id const&, void* data);
														TextureObject (TextureObject const&);
		void											operator = (TextureObject const&);
														~TextureObject (void);

		void											Bind (TextureUnit const&);
		void											Upload (void);

		void											Release (void (*memfree) (void*));

	private:
		Id												id;
		TextureUnitId									boundTo;
		bool											isBound;
		bool											isUploaded;
		bool											isDataAllocated;
		void* const										data;
	}; // class TextureObject

}}} // namespace my::gl::textures

#endif // __MY_UTIL__MY__GL__TEXTURES__TEXTURE_OBJECT__H__
