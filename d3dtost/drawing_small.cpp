#include "stdafx.h"

#define __NE()	DASSERT(!::nmutil::gl::ErrorsHandled(&::_::__gl_error_handler));


using namespace gl::ext;
using codeshare::utilities::pointer_utilities::offset;

///////////////////////////////////////////////////////////

namespace _			{

///////////////////////////////////////////////////////////

struct DD {
	ankh::textures::Texture*	texture;
	ankh::images::Image*		image;
	GLuint						textureid;
	ankh::images::ImageDecoder*	dec;
};

static inline
DD* newdd (
		ankh::textures::Texture* const		texture		= NULL,
		ankh::images::Image* const			image		= NULL,
		GLuint const						textureid	= -1,
		ankh::images::ImageDecoder* const	dec			= NULL) {
	DD* const dd(DNEW(DD));

	dd->texture		= texture;
	dd->image		= image;
	dd->textureid	= textureid;
	dd->dec			= dec;

	return dd;
}

///////////////////////////////////////////////////////////

struct vec4 {
	float a, b, c, d;
};

static inline
vec4 makevec4 (float const a, float const b, float const c) {
	vec4 v;
	v.a = a/10.f;
	v.b = b/10.f;
	v.c = c/10.f;
	v.d = 1;
	return v;
}

///////////////////////////////////////////////////////////

static inline
void __gl_error_handler (char const* const msg) {
	std::string _msg(msg);
	DASSERT(false);
}

///////////////////////////////////////////////////////////

}	// _

///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////

namespace my		{
namespace drawing	{

///////////////////////////////////////////////////////////

#define VC4(X,Y,Z)	_::makevec4(X,Y,Z)
#define POS(X,Y,Z)	VC4(X,Y,Z)
#define TXR(S,T)	VC4(S,T,0)
#define COL(R,G,B)	VC4(R,G,B)

void* setup (void) {

	glEnable(GL_TEXTURE_2D); __NE()
	glEnable(GL_CULL_FACE); __NE()
	glEnable(GL_DEPTH_TEST); __NE()

	GLuint buffer(-1);
	glGenBuffers(1, &buffer); __NE()
	glBindBuffer(GL_ARRAY_BUFFER, buffer); __NE()
	_::vec4 const vertexData[] = {
		POS(-6, -6, -6),		POS(6, -6, -6),		POS(0, 6, -6),
		TXR(0, 0),				TXR(10, 0),			TXR(5, 10),
		COL(8, 0, 0),			COL(0, 8, 0),		COL(0, 0, 8)
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData[0], GL_STATIC_DRAW); __NE()
	//GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	glVertexAttribPointer(OpenGL::VAI_POSITION, 4, GL_FLOAT, GL_FALSE, 0, offset(0)); __NE()
	glVertexAttribPointer(OpenGL::VAI_TEXCOORD, 4, GL_FLOAT, GL_FALSE, 0, offset(3 * sizeof(vertexData[0]))); __NE()
	glVertexAttribPointer(OpenGL::VAI_COLOUR  , 4, GL_FLOAT, GL_FALSE, 0, offset(6 * sizeof(vertexData[0]))); __NE()

	glEnableVertexAttribArray(OpenGL::VAI_POSITION); __NE()
	glEnableVertexAttribArray(OpenGL::VAI_TEXCOORD); __NE()
	glEnableVertexAttribArray(OpenGL::VAI_COLOUR); __NE()


	using namespace ankh::images;
	using namespace ankh::textures;

//	ankh::images::Initialise();
//	ankh::textures::Initialise();

//	my::image_decoders::DevilImageDecoder* devil(NULL);
//	UNEW(devil);
//	ankh::images::ImageLoader::GetSingleton().InstallDecoder(devil);

	Image* const image(NULL); //ankh::images::ImageLoader::GetSingleton().LoadFromPath("../textures/CoolTexture.tga"));

	PixelFormat pxlfmt;
	GLint w(-1);
	GLint h(-1);
//	GLbyte* bytes(glt::ReadTGABits("../textures/CoolTexture.tga", &malloc, &free, (GLenum*)&pxlfmt, &w, &h));
	GLubyte bytes[] = { GLubyte(0xff), GLubyte(0x00), GLubyte(0x00),	GLubyte(0x00), GLubyte(0xff), GLubyte(0x00),
						GLubyte(0x00), GLubyte(0x00), GLubyte(0xff),	GLubyte(0xff), GLubyte(0x00), GLubyte(0xff)
					};
	w = h = 2;
	pxlfmt = PixelFormats::RGB;

	GLuint gltxr(-1);
	glGenTextures(1, &gltxr); __NE()
	glActiveTexture(GL_TEXTURE0); __NE()
	glBindTexture(GL_TEXTURE_2D, gltxr); __NE()
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); __NE()
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); __NE()
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); __NE()
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0); __NE()
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, pxlfmt, GL_UNSIGNED_BYTE, bytes); __NE()
//	free(bytes);


	Texture* const texture(NULL);//TextureManager::GetSingleton().New("../textures/CoolTexture.tga"));

	return _::newdd(texture, image, gltxr, NULL);
}

///////////////////////////////////////////////////////////

void cleanup (void*& drawData) {
	_::DD*& dd = reinterpret_cast<_::DD*&>(drawData);  // stupid microsoft -- cannot parenth-init refs to ptrs

	if (dd->textureid >= 0)
		{ glDeleteTextures(1, &dd->textureid); __NE() }

	if (dd->image)
		ankh::images::ImageLoader::GetSingleton().Unload(dd->image);

	if (dd->texture)
		ankh::textures::TextureManager::GetSingleton().Delete(dd->texture);

	if (dd->dec)
		udelete(dd->dec);

//	ankh::textures::CleanUp();
//	ankh::images::CleanUp();

	udelete(dd);
}

///////////////////////////////////////////////////////////

void draw (void* drawData, void (*bufferSwapper) (void*), void* bufferSwapperClosure);
void draw (void* const drawingData, void (* const bufferSwapper) (void*), void* const bufferSwapperClosure) {
	_::DD* const dd(reinterpret_cast<_::DD* const>(drawingData));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniform1i(OpenGL::VUL_SAMPLER4, 4); __NE()
	glUniform1i(OpenGL::VUL_SAMPLER0, 0); __NE()
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, dd->textureid); __NE()
	glDrawArrays(GL_TRIANGLES, 0, 3); __NE()
		
	(*bufferSwapper)(bufferSwapperClosure);
}

///////////////////////////////////////////////////////////

}	// drawing
}	// my

///////////////////////////////////////////////////////////