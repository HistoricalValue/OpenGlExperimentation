#ifndef __DRAWING_H__
#define __DRAWING_H__

#include "stdafx.h"

#include <my/gl/adapters/Buffer.h>
#include <my/gl/adapters/BufferManager.h>
#include <Options.h>
#include <OpenGlPrimitiveAdapters.h>
#include <OpenGlAdapters.h>

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

	gl::adapt::RenderBuffer*	Red (void)
									{ return Channel(RED); }
	gl::adapt::RenderBuffer*	Green (void)
									{ return Channel(GREEN); }
	gl::adapt::RenderBuffer*	Blue (void)
									{ return Channel(BLUE); }
	gl::adapt::RenderBuffer*	All (void)
									{ return Channel(ALL); }

	ColourChannels (void)
		{ gl::adapt::RenderBufferManager::GetSingleton().Create(channels); }
	~ColourChannels (void)
		{ gl::adapt::RenderBufferManager::GetSingleton().Release(channels); }

private:
	gl::adapt::RenderBuffer*	channels[4];

	gl::adapt::RenderBuffer*	Channel (size_t const index)
									{ DASSERT(index < _countof(channels)); return DNULLCHECK(channels[index]); }
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
	gl::adapt::FrameBuffer*		framebuffer;
	ColourChannels				colourbuffers;
	gl::adapt::RenderBuffer*	depthbuffer;
};

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////

#endif // __DRAWING_H__
