#include <stdafx.h>

#include <MySafeCast.h>
#include <PConfigurations.h>
#include <PPointerUtilities_inl.h>
#include <my/gl/Shapes/VertexData.h>

#ifndef P_INLINING
#	define __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__DEFINING
#	include <my/gl/shapes/VertexData_inl.h>
#endif


namespace my { namespace gl { namespace shapes {

	static const size_t __PositionOffset	(0u);
	static const size_t __ColourOffset		(1u);
	static const size_t __NormalOffset		(2u);
	static const size_t __AOOffset			(3u);

	P_INLINE
	size_t VertexData::Offset (size_t i) {
		const size_t sVertexData	(sizeof(VertexData));
		const size_t sColour		(sizeof(Colour const));
		const size_t sVector4		(sizeof(math::Vector4 const));
		const size_t sPosition		(sVector4);
		const size_t sFloat4		(sizeof(float[4]));
		const size_t sFloat			(sizeof(float));
		const size_t sNormal		(sizeof(math::Vector4 const));
		const size_t sAO			(sizeof(float const));

		P_STATIC_ASSERT(sVertexData == sPosition + sColour + sNormal + sAO)
		P_STATIC_ASSERT(sPosition == sVector4)
		P_STATIC_ASSERT(sColour == sVector4)
		P_STATIC_ASSERT(sNormal == sVector4)
		P_STATIC_ASSERT(sVector4 == sFloat4)
		P_STATIC_ASSERT(sAO == sFloat)

		const size_t positionOffset	(offsetof(VertexData, position));
		const size_t colourOffset	(offsetof(VertexData, colour));
		const size_t normalOffset	(offsetof(VertexData, normal));
		const size_t aoOffset		(offsetof(VertexData, ambientOcclusion));

		P_STATIC_ASSERT(positionOffset != 0 || colourOffset == sPosition)
		P_STATIC_ASSERT(colourOffset != 0 || positionOffset == sColour)
		P_STATIC_ASSERT(normalOffset != 0 || normalOffset == sNormal)
		P_STATIC_ASSERT(aoOffset != 0 || aoOffset == sAO)

		return
				i == __PositionOffset?	positionOffset:
				i == __ColourOffset?	colourOffset:
				i == __NormalOffset?	normalOffset:
				i == __AOOffset?		aoOffset:
				-1;
	}

	size_t VertexData::PositionOffset (void) {
		return Offset(__PositionOffset);
	}

	size_t VertexData::ColourOffset (void) {
		return Offset(__ColourOffset);
	}

	size_t VertexData::NormalOffset (void) {
		return Offset(__NormalOffset);
	}

	size_t VertexData::AOOffset (void) {
		return Offset(__AOOffset);
	}


	TexturedVertexData::TexturedVertexData (math::Vector4 const& _position, math::Vector4 const& _textureCoordinate, math::Vector4 const& _normal, float const ao):
		position			(_position),
		textureCoordinate	(_textureCoordinate),
		normal				(_normal),
		ambientOcclusion	(ao)
		{}

	TexturedVertexData::TexturedVertexData (TexturedVertexData const& d):
		position			(d.position			),
		textureCoordinate	(d.textureCoordinate),
		normal				(d.normal			),
		ambientOcclusion	(d.ambientOcclusion	)
		{}

	void* TexturedVertexData::TextureCoordinatesOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(TextureCoordinatesOffset()));
	}

	size_t TexturedVertexData::TextureCoordinatesOffset (void) {
		return offsetof(TexturedVertexData, textureCoordinate);
	}

	size_t TexturedVertexData::NormalOffset (void) {
		return offsetof(TexturedVertexData, normal);
	}

	void* TexturedVertexData::NormalOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(NormalOffset()));
	}

	size_t TexturedVertexData::Stride (void) {
		return sizeof(TexturedVertexData);
	}

	void* TexturedVertexData::PositionOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(PositionOffset()));
	}

	size_t TexturedVertexData::PositionOffset (void) {
		return offsetof(TexturedVertexData, position);
	}

	void* TexturedVertexData::AOOffsetPointer (void) {
		return codeshare::utilities::pointer_utilities::offset(psafecast<ptrdiff_t>(AOOffset()));
	}

	size_t TexturedVertexData::AOOffset (void) {
		return offsetof(TexturedVertexData, ambientOcclusion);
	}

}}} // namespace my::gl::shapes

