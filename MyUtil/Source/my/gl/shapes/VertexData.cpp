#include "stdafx.h"

#include "PConfigurations.h"
#include "PPointerUtilities_inl.h"

#ifndef P_INLINING
#	define __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__DEFINING
#	include "my/gl/shapes/VertexData_inl.h"
#endif


namespace my { namespace gl { namespace shapes {

	static const size_t __PositionOffset	(0u);
	static const size_t __ColourOffset		(1u);

	P_INLINE
	size_t VertexData::Offset (size_t i) {
		const size_t sVertexData	(sizeof(VertexData));
		const size_t sColour		(sizeof(Colour const));
		const size_t sVector4		(sizeof(math::Vector4 const));
		const size_t sPosition		(sVector4);
		const size_t sFloat4		(sizeof(float[4]));

		P_STATIC_ASSERT(sVertexData == sPosition + sColour)
		P_STATIC_ASSERT(sPosition == sVector4)
		P_STATIC_ASSERT(sColour == sVector4)
		P_STATIC_ASSERT(sVector4 == sFloat4)

		const size_t positionOffset	(offsetof(VertexData, position));
		const size_t colourOffset	(offsetof(VertexData, colour));

		P_STATIC_ASSERT(positionOffset != 0 || colourOffset == sPosition)
		P_STATIC_ASSERT(colourOffset != 0 || positionOffset == sColour)

		return
				i == __PositionOffset?	positionOffset:
				i == __ColourOffset?	colourOffset:
				-1;
	}

	size_t VertexData::PositionOffset (void) {
		return Offset(__PositionOffset);
	}

	size_t VertexData::ColourOffset (void) {
		return Offset(__ColourOffset);
	}



	TexturedVertexData::TexturedVertexData (math::Vector4 const& _position, math::Vector4 const& _textureCoordinate):
		position			(_position),
		textureCoordinate	(_textureCoordinate)
		{}

	void* TexturedVertexData::TextureCoordinatesOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(TextureCoordinatesOffset());
	}

	size_t TexturedVertexData::TextureCoordinatesOffset (void) {
		return offsetof(TexturedVertexData, textureCoordinate);
	}

	size_t TexturedVertexData::Stride (void) {
		return sizeof(TexturedVertexData);
	}

	void* TexturedVertexData::PositionOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(PositionOffset());
	}

	size_t TexturedVertexData::PositionOffset (void) {
		return offsetof(TexturedVertexData, position);
	}

}}} // namespace my::gl::shapes

