#ifndef __DRAWING_H__
#define __DRAWING_H__

#include "stdafx.h"

#include <my/gl/adapters/Buffer.h>
#include <my/gl/adapters/BufferManager.h>
#include <Options.h>
#include <FrameBufferObject.h>

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

struct DrawData {
	GLuint						vertexArrayIds[NUMBER_OF_VAOs];
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
	gl::prim::Id				framebuffer;
	GlobalState					gstate;
};

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////

#endif // __DRAWING_H__
