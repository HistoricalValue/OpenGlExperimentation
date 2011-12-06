#ifndef __DRAWING_H__
#define __DRAWING_H__

#include "stdafx.h"

#include <my/gl/adapters/Buffer.h>
#include <my/gl/adapters/BufferManager.h>
#include <Options.h>
#include <OpenGlPrimitiveAdapters.h>
#include <OpenGlAdapters.h>
#include <MyUtils.h>

///////////////////////////////////////////////////////////

namespace _	{

///////////////////////////////////////////////////////////

typedef ankh::images::Image*		ImagesArray[NUMBER_OF_IMAGES];
typedef ankh::textures::Texture*	TexturesArray[NUMBER_OF_TEXTURES];

///////////////////////////////////////////////////////

struct GlobalState {
	ankh::shapes::Mesh*	mesh;
};

///////////////////////////////////////////////////////

struct ColourChannels {
	static const size_t		RED		= 0u;
	static const size_t		GREEN	= 1u;
	static const size_t		BLUE	= 2u;
	static const size_t		ALL		= 3u;

	gl::adapt::Renderbuffer*	Red (void)
									{ return r; }
	gl::adapt::Renderbuffer*	Green (void)
									{ return g; }
	gl::adapt::Renderbuffer*	Blue (void)
									{ return b; }
	gl::adapt::Renderbuffer*	All (void)
									{ return a; }

	gl::adapt::Renderbuffer*	operator [] (size_t const i)
									{ DASSERT(i < 4); return AsArray()[i]; }

	void	Create (void)
				{ gl::adapt::RenderbufferManager::GetSingleton().Create(AsArray()); }

	void	Release (void)
				{ gl::adapt::RenderbufferManager::GetSingleton().Release(AsArray()); }

	void	Allocate (GLsizei const width, GLsizei const height)
				{ foreach(AsArray(), ubind2nd(ubind2nd(umemberfunctionpointer(&gl::adapt::Renderbuffer::AllocateColour), width), height)); }

	void	Map (void) {

			}

private:
	gl::adapt::Renderbuffer*	r;
	gl::adapt::Renderbuffer*	g;
	gl::adapt::Renderbuffer*	b;
	gl::adapt::Renderbuffer*	a;

	gl::adapt::Renderbuffer*	(& AsArray (void))[4] {
									gl::adapt::Renderbuffer* (&result)[4](*reinterpret_cast<gl::adapt::Renderbuffer* (*)[4]>(this));
									DASSERT(result[RED] == r && result[GREEN] == g && result[BLUE] == b && result[ALL] == a);
									return result;
								}
};

///////////////////////////////////////////////////////

struct DrawData {
	gl::adapt::VertexArray*		vertexArrayIds[NUMBER_OF_VAOs];
	my::gl::adapters::Buffer*	buffers[NUMBER_OF_VBOs];
	GLuint						texturesIds[NUMBER_OF_TEXTURES];
	GLuint						numberOfPoints;
	GLuint						numberOfWorldCubeLineSegments;
	GLuint						numberOfPointPoints;
	unsigned long int			startingTime;
	unsigned long int			prevtime;
	GLuint						sampler_location;
	TexturesArray				textures;
	ImagesArray					images;
	GLuint						numberOfTexturedSegments;
	size_t						previousTextureIndex;
	GlobalState					gstate;
	gl::adapt::Framebuffer*		framebuffer;
	ColourChannels				colourbuffers;
	gl::adapt::Renderbuffer*	depthbuffer;
};

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////

#endif // __DRAWING_H__
