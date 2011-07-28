#ifndef __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA_INL__H__

#include "my/gl/shapes/VertexData.h"
#include "PConfigurations.h"
#include "PPointerUtilities.h"

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__DEFINING)

namespace my { namespace gl { namespace shapes {

	P_INLINE
	VertexData::VertexData (math::Vector4 const& _position, Colour const& _colour):
		position(_position),
		colour(_colour)
		{ P_STATIC_ASSERT(sizeof(VertexData) == sizeof(position) + sizeof(colour)) }

	P_INLINE
	VertexData::VertexData (VertexData const& other):
		position(other.position),
		colour(other.colour)
		{ P_STATIC_ASSERT(sizeof(VertexData) == sizeof(position) + sizeof(colour)) }

	P_INLINE
	VertexData::~VertexData (void) {
	}

	P_INLINE
	math::Vector4 const& VertexData::GetPosition (void) const {
		return position;
	}

	P_INLINE
	Colour const& VertexData::GetColour (void) const {
		return colour;
	}

	P_INLINE
	size_t VertexData::Stride (void) {
		return sizeof(VertexData);
	}

	P_INLINE
	void* VertexData::PositionOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(PositionOffset());
	}

	P_INLINE
	void* VertexData::ColourOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(ColourOffset());
	}

}}} // namespace my::gl::shapes

#endif

#endif // __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA_INL__H__
