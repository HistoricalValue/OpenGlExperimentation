#ifndef __DRAWING__SETUP_SHAPES___H__
#define __DRAWING__SETUP_SHAPES___H__

#include <drawing.h>
#include <drawing_nurbs.h>

///////////////////////////////////////////////////////

namespace	_	{

///////////////////////////////////////////////////////

static const float POOP_LIGHT[4] = { 6.0f, 2.0f, 0.0f, 1.0f };
static GLboolean const POINTS_NORMALISED(GL_TRUE);

///////////////////////////////////////////////////////

extern
void SetUpShapes (
		DrawData&						dd,
		//
		gl::adapt::VertexArray* const	point_vertexArrayId,
		gl::adapt::Vertexbuffer* const	point_buffer0,
		gl::adapt::Vertexbuffer* const	point_buffer1,
		GLuint&							numberOfPointPoints,
		//
		gl::adapt::VertexArray* const	line_vertexArrayId,
		gl::adapt::Vertexbuffer* const	line_buffer0,
		gl::adapt::Vertexbuffer* const	line_buffer1,
		GLuint&							numberOfPoints,
		//
		gl::adapt::VertexArray* const	tria_vertexArrayId,
		gl::adapt::Vertexbuffer* const	tria_buffer0,
		gl::adapt::Vertexbuffer* const	tria_buffer1,
		GLuint&							numberOfWorldCubeLineSegments,
		//
		gl::adapt::VertexArray* const	text_vertexArrayId,
		gl::adapt::Vertexbuffer* const	text_buffer0,
		gl::adapt::Vertexbuffer* const	text_buffer1,
		GLuint&							numberOfTexturedSegment);

///////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////

#endif	// __DRAWING__SETUP_SHAPES___H__
