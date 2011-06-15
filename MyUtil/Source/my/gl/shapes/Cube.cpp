#include "stdafx.h"

#include "PPointerUtilities_inl.h"
#include "my/gl/shapes/Line_inl.h"
#include "my/gl/shapes/VertexData_inl.h"
#include "my/gl/shapes/Colour_inl.h"

#define OFFSETS_STATIC_CHECKS																	\
		using codeshare::pointer_utilities::offset;												\
		size_t const off00(offsetof(Cube, line00));												\
		size_t const off11(offsetof(Cube, line11));												\
		size_t const minOff(off00<off11? off00 : off11);										\
		size_t const maxOff(off00>off11? off00 : off11);										\
		P_STATIC_ASSERT(offsetof(Cube, line00) >= minOff && offsetof(Cube, line00) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line01) >= minOff && offsetof(Cube, line01) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line02) >= minOff && offsetof(Cube, line02) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line03) >= minOff && offsetof(Cube, line03) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line04) >= minOff && offsetof(Cube, line04) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line05) >= minOff && offsetof(Cube, line05) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line06) >= minOff && offsetof(Cube, line06) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line07) >= minOff && offsetof(Cube, line07) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line08) >= minOff && offsetof(Cube, line08) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line09) >= minOff && offsetof(Cube, line09) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line10) >= minOff && offsetof(Cube, line10) >= minOff)	\
		P_STATIC_ASSERT(offsetof(Cube, line11) >= minOff && offsetof(Cube, line11) >= minOff)	\


namespace _ {
	static size_t const numberOfLines			(0xcu);
	static size_t const numberOfVerticesPerLine	(0x2u);
} // namespace _


namespace my { namespace gl { namespace shapes {

	Cube::Cube (Colour const& colour):
		Shape(colour),
		line00(Vertex(math::Vector4::New(-1,-1,-1,   1)), Vertex(math::Vector4::New(-1,-1, 1,   1))),
		line01(Vertex(math::Vector4::New(-1,-1, 1,   1)), Vertex(math::Vector4::New( 1,-1, 1,   1))),
		line02(Vertex(math::Vector4::New( 1,-1, 1,   1)), Vertex(math::Vector4::New( 1,-1,-1,   1))),
		line03(Vertex(math::Vector4::New( 1,-1,-1,   1)), Vertex(math::Vector4::New(-1,-1,-1,   1))),
		line04(Vertex(math::Vector4::New(-1,-1,-1,   1)), Vertex(math::Vector4::New(-1, 1,-1,   1))),
		line05(Vertex(math::Vector4::New(-1, 1,-1,   1)), Vertex(math::Vector4::New(-1, 1, 1,   1))),
		line06(Vertex(math::Vector4::New(-1, 1, 1,   1)), Vertex(math::Vector4::New( 1, 1, 1,   1))),
		line07(Vertex(math::Vector4::New( 1, 1, 1,   1)), Vertex(math::Vector4::New( 1, 1,-1,   1))),
		line08(Vertex(math::Vector4::New( 1, 1,-1,   1)), Vertex(math::Vector4::New(-1, 1,-1,   1))),
		line09(Vertex(math::Vector4::New(-1,-1, 1,   1)), Vertex(math::Vector4::New(-1, 1, 1,   1))),
		line10(Vertex(math::Vector4::New( 1,-1, 1,   1)), Vertex(math::Vector4::New( 1, 1, 1,   1))),
		line11(Vertex(math::Vector4::New( 1,-1,-1,   1)), Vertex(math::Vector4::New( 1, 1,-1,   1))) {
		
		P_STATIC_ASSERT(
				sizeof(Cube) == 0
					+ sizeof(Shape)
					+ sizeof(Line) * _::numberOfLines
			)
	}

	Cube::Cube (Cube const& cube):
		Shape(cube),
		line00(cube.line00),
		line01(cube.line01),
		line02(cube.line02),
		line03(cube.line03),
		line04(cube.line04),
		line05(cube.line05),
		line06(cube.line06),
		line07(cube.line07),
		line08(cube.line08),
		line09(cube.line09),
		line10(cube.line10),
		line11(cube.line11) {
		P_STATIC_ASSERT( sizeof(Cube) == 0
				+ sizeof(Shape)
				+ sizeof(Line) * _::numberOfLines
			)
	}

	Cube::~Cube (void) {
	}

	Line* Cube::GetLines (void) {
		OFFSETS_STATIC_CHECKS

		Line* const result(reinterpret_cast<Line* const>(offset(this, minOff)));
		PASSERT(result == &line00 || result == &line11)
		return result;
	}

	Line const* Cube::GetConstLines (void) const {
		OFFSETS_STATIC_CHECKS

		Line const* const result(reinterpret_cast<Line const* const>(offset(this, minOff)));
		PASSERT(result == &line00 || result == &line11)
		return result;
	}

	VertexData* Cube::GetVertexData (void* const memory, size_t const bytesize) const {
		PASSERT(Line::GetLineNumberOfVertices() == _::numberOfVerticesPerLine)

		size_t const numberOfVertexDataElements		(_::numberOfLines * _::numberOfVerticesPerLine);
		size_t const sizeofResult					(numberOfVertexDataElements * sizeof(VertexData));
		VertexData* result							(NULL);
		
		if (bytesize >= sizeofResult) {
			result									= reinterpret_cast<VertexData* const>(memory);
			Line const* const lines					(GetConstLines());
			Colour const colour1					(colour);
			Colour const colour2					(colour * math::Vector4::New(.8f, .8f, .8f, 1.f));

			P_STATIC_ASSERT(_::numberOfVerticesPerLine == 2)
			for (size_t i(0); i < _::numberOfLines; ++i) {
				PASSERT(2*i + 1 < numberOfVertexDataElements)
				new(&result[2*i])		VertexData(lines[i].GetBeginning().xyzw(), colour1);
				new(&result[2*i+1])		VertexData(lines[i].GetEnd().xyzw(), colour2);
			}
		}

		return result;
	}

	Cube* Cube::Clone (void* memory, size_t size) const {
		Cube* result(NULL);

		if (size >= sizeof(Cube)) {
			Cube* const self(reinterpret_cast<Cube* const>(memory));

			new(self) Cube(*this);

			result = self;
		}

		return result;
	}

	void Cube::Apply (my::gl::math::Matrix4x4 const& mat) {
		Line* lines(GetLines());

		for (size_t i(0); i < _::numberOfLines; ++i)
			lines[i].Apply(mat);
	}

	void Cube::Adjust (math::Vector4 const& vec4) {
		Line* lines(GetLines());
		for (size_t i(0); i < _::numberOfLines; ++i)
			lines[i].Adjust(vec4);
	}
	
	size_t Cube::GetCubeNumberOfVertices (void) {
		PASSERT(_::numberOfVerticesPerLine == Line::GetLineNumberOfVertices())
		return _::numberOfLines * _::numberOfVerticesPerLine;
	}


}}} // namespace my::gl::shapes


