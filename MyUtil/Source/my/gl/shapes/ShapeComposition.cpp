#include "stdafx.h"

#include "PPointerUtilities_inl.h"

#include "PConfigurations.h"
#ifndef P_INLINING
#	define __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__DEFINING
#	include "my/gl/shapes/ShapeComposition_inl.h"
#endif

namespace my { namespace gl { namespace shapes {


	ShapeComposition::ShapeComposition (Shape** const shapesArrayMemory, size_t const shapesArrayMemoryBytesize):
		Shape(Colour(math::Vector4::New(.6f, .6f, .6f, 1.f))),
#pragma warning( push )
#pragma warning( disable: 4351 ) // elementa of "shapes" will be default initialised
		shapes(shapesArrayMemory),
		shapesBytesize(shapesArrayMemoryBytesize),
		i(0u) {
#pragma warning( pop )
		PASSERT(shapesArrayMemoryBytesize % sizeof(shapes[0]) == 0)
		P_STATIC_ASSERT(sizeof(ShapeComposition) == 0
			+ sizeof(Shape)
			+ sizeof(shapes)
			+ sizeof(shapesBytesize)
			+ sizeof(i))
	}
	
	ShapeComposition::~ShapeComposition (void) {
	}

	// Shape
	VertexData* ShapeComposition::GetVertexData (void* const memory, size_t const bytesize) const {
		using codeshare::utilities::pointer_utilities::reinterpret_assign;
		using codeshare::utilities::pointer_utilities::offset;

		size_t count				(GetNumberOfVertices());
		size_t const requiredSize	(count * sizeof(VertexData));
		VertexData* result			(NULL);

		if (bytesize >= requiredSize) {
			reinterpret_assign(result, memory);
			size_t write_offset(0u);

			for (Shape const* const* shape = &shapes[0]; shape < &shapes[i]; ++shape) {
				VertexData* const subresult((*shape)->GetVertexData(offset(result, write_offset), bytesize-write_offset));
				PASSERT(subresult != NULL)
				write_offset += (*shape)->GetNumberOfVertices() * sizeof(VertexData);
			}
			PASSERT(write_offset == requiredSize);
		}

		return result;
	}

	TexturedVertexData* ShapeComposition::GetTexturedVertexData (void* const memory, size_t const bytesize) const {
		using codeshare::utilities::pointer_utilities::reinterpret_assign;
		using codeshare::utilities::pointer_utilities::offset;

		size_t count				(GetNumberOfVertices());
		size_t const requiredSize	(count * sizeof(TexturedVertexData));
		TexturedVertexData* result	(NULL);

		if (bytesize >= requiredSize) {
			reinterpret_assign(result, memory);
			size_t write_offset(0u);

			for (Shape const* const* shape = &shapes[0]; shape < &shapes[i]; ++shape) {
				TexturedVertexData* const subresult((*shape)->GetTexturedVertexData(offset(result, write_offset), bytesize-write_offset));
				PASSERT(subresult != NULL)
				write_offset += (*shape)->GetNumberOfVertices() * sizeof(TexturedVertexData);
			}
			PASSERT(write_offset == requiredSize);
		}

		return result;
	}

	ShapeComposition* ShapeComposition::Clone (void* memory, size_t bytesize) const {
		using codeshare::utilities::pointer_utilities::reinterpret_assign;
		using codeshare::utilities::pointer_utilities::offset;

		size_t const requiredSize(GetSizeOf());

		ShapeComposition* result(NULL);

		if (bytesize >= requiredSize) {
			reinterpret_assign(result, memory);
			size_t write_offset(0U);

			for (Shape const* const* shape = &shapes[0]; shape < &shapes[i]; ++shape) {
				Shape* const subclone((*shape)->Clone(offset(result, write_offset), bytesize - write_offset));
				PASSERT(subclone != NULL)
				write_offset += (*shape)->GetSizeOf();
			}
			PASSERT(write_offset == requiredSize)
		}

		return result;
	}

	P_INLINE
	size_t ShapeComposition::GetNumberOfVertices (void) const {
		PASSERT(i <= shapesBytesize/sizeof(shapes[0]))

		size_t numberOfVertices(0u);

		for (Shape const* const* shape = &shapes[0]; shape < &shapes[i]; ++shape)
			numberOfVertices += (*shape)->GetNumberOfVertices();

		return numberOfVertices;
	}

	P_INLINE
	size_t ShapeComposition::GetSizeOf (void) const {
		PASSERT(i <= shapesBytesize/sizeof(shapes[0]))

		size_t sizeOf(0u);

		for (Shape const* const* shape = &shapes[0]; shape < &shapes[i]; ++shape)
			sizeOf += (*shape)->GetSizeOf();

		return sizeOf;
	}

}}} // namespace my::gl::shapes