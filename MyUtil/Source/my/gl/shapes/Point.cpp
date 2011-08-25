#include <stdafx.h>

namespace my { namespace gl { namespace shapes {

//////////////////////////////////////////////////////////

Point::Point (Vertex const& v, Colour const& colour):
	Shape(colour),
	point(v)
	{ }

//////////////////////////////////////////////////////////

Point::Point (Point const& other):
	Shape(other),
	point(other.point)
	{ }

//////////////////////////////////////////////////////////

Point::~Point (void) {
}

//////////////////////////////////////////////////////////

inline
void Point::Apply (math::Matrix4x4 const& mat) {
	point.Apply(mat);
}

//////////////////////////////////////////////////////////

inline
void Point::Adjust (math::Vector4 const& vec) {
	point.Adjust(vec);
}

//////////////////////////////////////////////////////////

inline
// static
size_t Point::GetPointNumberOfVertices (void) {
	return 1u;
}

//////////////////////////////////////////////////////////

inline
size_t Point::GetNumberOfVertices (void) const {
	return GetPointNumberOfVertices();
}

//////////////////////////////////////////////////////////

VertexData* Point::GetVertexData (void* const mem, size_t const bytesize) const {
	size_t const requiredSize(GetNumberOfVertices() * sizeof(VertexData));

	VertexData* result(NULL);
	if (bytesize >= requiredSize)
		new(ucastassign(result, mem)) VertexData(point.xyzw(), colour);

	return result;
}

//////////////////////////////////////////////////////////

Point* Point::Clone (void* const mem, size_t const bytesize) const {
	size_t const requiredSize(GetSizeOf());

	Point* result(NULL);
	if (bytesize >= requiredSize) {
		ucastassign(result, mem);
		new(result) Point(*this);
	}
	return result;
}

//////////////////////////////////////////////////////////

inline
size_t Point::GetSizeOf (void) const {
	return GetSizeOfPoint();
}

inline
// static
size_t Point::GetSizeOfPoint (void) {
	return sizeof(Point);
}

//////////////////////////////////////////////////////////

}}} // namespace my::gl::shape

