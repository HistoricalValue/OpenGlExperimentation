#ifndef __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA_INL__H__

#include <my/gl/shapes/VertexData.h>
#include <PConfigurations.h>
#include <PPointerUtilities.h>
#include <MySafeCast.h>

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__DEFINING)

namespace my { namespace gl { namespace shapes {

	P_INLINE
	VertexData::VertexData (math::Vector4 const& _position, Colour const& _colour, math::Vector4 const& _normal, float const ao):
		position(_position),
		colour(_colour),
		normal(_normal),
		ambientOcclusion(ao)
		{ P_STATIC_ASSERT(sizeof(VertexData) == sizeof(position) + sizeof(colour) + sizeof(normal) + sizeof(ambientOcclusion)) }

	P_INLINE
	VertexData::VertexData (VertexData const& other):
		position(other.position),
		colour(other.colour),
		normal(other.normal),
		ambientOcclusion(other.ambientOcclusion)
		{ P_STATIC_ASSERT(sizeof(VertexData) == sizeof(position) + sizeof(colour) + sizeof(normal) + sizeof(ambientOcclusion)) }

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
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(PositionOffset()));
	}

	P_INLINE
	void* VertexData::ColourOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(ColourOffset()));
	}

	P_INLINE
	void* VertexData::NormalOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(NormalOffset()));
	}

	P_INLINE
	void* VertexData::AOOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(AOOffset()));
	}

}}} // namespace my::gl::shapes

#endif

#endif // __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA_INL__H__
