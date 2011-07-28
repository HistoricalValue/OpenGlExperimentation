#include "stdafx.h"

#include "PPointerUtilities_inl.h"

#define OFFSETS_STATIC_CHECKS																					\
		using codeshare::utilities::pointer_utilities::offset;													\
		size_t const off00(offsetof(SolidCube, triangle00));													\
		size_t const off11(offsetof(SolidCube, triangle11));													\
		size_t const minOff(off00<off11? off00 : off11);														\
		size_t const maxOff(off00>off11? off00 : off11);														\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle00) >= minOff && offsetof(SolidCube, triangle00) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle01) >= minOff && offsetof(SolidCube, triangle01) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle02) >= minOff && offsetof(SolidCube, triangle02) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle03) >= minOff && offsetof(SolidCube, triangle03) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle04) >= minOff && offsetof(SolidCube, triangle04) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle05) >= minOff && offsetof(SolidCube, triangle05) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle06) >= minOff && offsetof(SolidCube, triangle06) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle07) >= minOff && offsetof(SolidCube, triangle07) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle08) >= minOff && offsetof(SolidCube, triangle08) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle09) >= minOff && offsetof(SolidCube, triangle09) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle10) >= minOff && offsetof(SolidCube, triangle10) >= minOff)	\
		P_STATIC_ASSERT(offsetof(SolidCube, triangle11) >= minOff && offsetof(SolidCube, triangle11) >= minOff)


namespace _ {
	static const size_t numberOfTriangles			(0xcu);
	static const size_t numberOfVerticesPerTriangle	(0x3u);
}

namespace my { namespace gl { namespace shapes {


	Triangle* SolidCube::GetTriangles (void) {
		OFFSETS_STATIC_CHECKS

		Triangle* const result(reinterpret_cast<Triangle* const>(offset(this, minOff)));
		PASSERT(result == &triangle00 || result == &triangle11)
		return result;
	}

	Triangle const* SolidCube::GetConstTriangles (void) const {
		OFFSETS_STATIC_CHECKS

		Triangle const* const result(reinterpret_cast<Triangle const* const>(offset(this, minOff)));
		PASSERT(result == &triangle00 || result == &triangle11)
		return result;
	}

	size_t SolidCube::GetSolidCubeNumberOfVertices (void) {
		PASSERT(_::numberOfVerticesPerTriangle == Triangle::GetTriangleNumberOfVertices())
		return _::numberOfTriangles * _::numberOfVerticesPerTriangle;
	}

	SolidCube::SolidCube (Colour const& colour):
		Shape(colour),
		triangle00(colour),
		triangle01(colour),
		triangle02(colour),
		triangle03(colour),
		triangle04(colour),
		triangle05(colour),
		triangle06(colour),
		triangle07(colour),
		triangle08(colour),
		triangle09(colour),
		triangle10(colour),
		triangle11(colour)
	{
		P_STATIC_ASSERT(sizeof(SolidCube) == sizeof(Shape) + 12*sizeof(Triangle))

		// build cube
		triangle01.RotateZ(M_PI);


		triangle02.RotateX(-M_PI / 2.f);
		//
		triangle03.RotateZ(M_PI);
		triangle03.RotateX(-M_PI / 2.f);

		triangle04.RotateX(-M_PI);
		//
		triangle05.RotateZ(M_PI);
		triangle05.RotateX(-M_PI);

		triangle06.RotateX(-(3.f*M_PI) / 2.f);
		//
		triangle07.RotateZ(M_PI);
		triangle07.RotateX(-(3.f*M_PI) / 2.f);

		triangle08.RotateY(M_PI / 2.f);
		//
		triangle09.RotateZ(M_PI);
		triangle09.RotateY(M_PI / 2.f);

		triangle10.RotateY( (3.f*M_PI) / 2.f);
		//
		triangle11.RotateZ(M_PI);
		triangle11.RotateY( (3.f*M_PI) / 2.f);
	}

	SolidCube::SolidCube (SolidCube const& other):
		Shape(other),
		triangle00(other.triangle00),
		triangle01(other.triangle01),
		triangle02(other.triangle02),
		triangle03(other.triangle03),
		triangle04(other.triangle04),
		triangle05(other.triangle05),
		triangle06(other.triangle06),
		triangle07(other.triangle07),
		triangle08(other.triangle08),
		triangle09(other.triangle09),
		triangle10(other.triangle10),
		triangle11(other.triangle11)
	{
		P_STATIC_ASSERT(sizeof(SolidCube) == sizeof(Shape) + 12*sizeof(Triangle))
	}

	SolidCube::~SolidCube (void) {
	}

	// Shape
	VertexData* SolidCube::GetVertexData (void* const memory, size_t bytesize) const {
		PASSERT(Triangle::GetTriangleNumberOfVertices() == _::numberOfVerticesPerTriangle)
		size_t const numberOfElements	(_::numberOfTriangles * _::numberOfVerticesPerTriangle);
		size_t const requiredSize		(numberOfElements * sizeof(VertexData));

		VertexData* result				(NULL);
		Triangle const* const triangles	(GetConstTriangles());

		P_STATIC_ASSERT(_::numberOfVerticesPerTriangle == 3)
		if (bytesize >= requiredSize) {
			result = reinterpret_cast<VertexData* const>(memory);

			Colour const	colour1(colour);
			Colour const	colour2(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(colour1))));
			Colour const	colour3(ColourFactory::Brighter(colour1));

			for (size_t i(0u); i < _::numberOfTriangles; ++i) {
				PASSERT(&result[3*i+2] < codeshare::utilities::pointer_utilities::offset(result, bytesize))

				Triangle const&	triangle(triangles[i]);

				new(&result[3*i]  ) VertexData(triangle.GetA().xyzw(), colour1);
				new(&result[3*i+1]) VertexData(triangle.GetB().xyzw(), colour2);
				new(&result[3*i+2]) VertexData(triangle.GetC().xyzw(), colour3);
			}
		}

		return result;
	}

	TexturedVertexData* SolidCube::GetTexturedVertexData (void* const memory, size_t const bytesize) const {
		PASSERT(Triangle::GetTriangleNumberOfVertices() == _::numberOfVerticesPerTriangle)
		size_t const numberOfElements	(_::numberOfTriangles * _::numberOfVerticesPerTriangle);
		size_t const requiredSize		(numberOfElements * sizeof(TexturedVertexData));

		TexturedVertexData* result		(NULL);
		Triangle const* const triangles	(GetConstTriangles());

		P_STATIC_ASSERT(_::numberOfVerticesPerTriangle == 3)
		if (bytesize >= requiredSize) {
			codeshare::utilities::pointer_utilities::reinterpret_assign(result, memory);

			// downside triangle A B C textcoords
			math::Vector4 const downA(math::Vector4::New( 0,  0, 0, 1));
			math::Vector4 const downB(math::Vector4::New( 1,  0, 0, 1));
			math::Vector4 const downC(math::Vector4::New( 0,  1, 0, 1));

			math::Vector4 const upA(math::Vector4::New( 1,  1, 0, 1));
			math::Vector4 const upB(math::Vector4::New( 0,  1, 0, 1));
			math::Vector4 const upC(math::Vector4::New( 1,  0, 0, 1));

			int triangleParity(0);

			for (size_t i(0u); i < _::numberOfTriangles; ++i) {
				PASSERT(&result[3*i+2] < codeshare::utilities::pointer_utilities::offset(result, bytesize))

				Triangle const&	triangle(triangles[i]);

				new(&result[3*i]  ) TexturedVertexData(triangle.GetA().xyzw(), triangleParity%2? upA : downA);
				new(&result[3*i+1]) TexturedVertexData(triangle.GetB().xyzw(), triangleParity%2? upB : downB);
				new(&result[3*i+2]) TexturedVertexData(triangle.GetC().xyzw(), triangleParity%2? upC : downC);

				++triangleParity;
			}
		}

		return result;
	}

	SolidCube* SolidCube::Clone (void* memory, size_t size) const {
		SolidCube* result(NULL);

		if (size >= sizeof(SolidCube)) {
			SolidCube* const cube(reinterpret_cast<SolidCube* const>(memory));

			new(cube) SolidCube(*this);

			result = cube;
		}

		return result;
	}

	void SolidCube::Apply (math::Matrix4x4 const& mat4) {
		triangle00.Apply(mat4);
		triangle01.Apply(mat4);
		triangle02.Apply(mat4);
		triangle03.Apply(mat4);
		triangle04.Apply(mat4);
		triangle05.Apply(mat4);
		triangle06.Apply(mat4);
		triangle07.Apply(mat4);
		triangle08.Apply(mat4);
		triangle09.Apply(mat4);
		triangle10.Apply(mat4);
		triangle11.Apply(mat4);
	}

	void SolidCube::Adjust (math::Vector4 const& vec4) {
		triangle00.Adjust(vec4);
		triangle01.Adjust(vec4);
		triangle02.Adjust(vec4);
		triangle03.Adjust(vec4);
		triangle04.Adjust(vec4);
		triangle05.Adjust(vec4);
		triangle06.Adjust(vec4);
		triangle07.Adjust(vec4);
		triangle08.Adjust(vec4);
		triangle09.Adjust(vec4);
		triangle10.Adjust(vec4);
		triangle11.Adjust(vec4);
	}


}}} // namespace my::gl::shapes

