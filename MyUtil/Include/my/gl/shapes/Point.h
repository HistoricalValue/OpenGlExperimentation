#ifndef __MY_UTIL__MY__GL__SHAPES__POINT__H__
#define __MY_UTIL__MY__GL__SHAPES__POINT__H__

#include <MyUtil.h>
#include <my/gl/shapes/Shape.h>

namespace my { namespace gl { namespace shapes {

class MYUTIL_API Point: public Shape {
public:
	// Shape
	virtual Point*		Clone (void*, size_t) const;
	virtual size_t		GetSizeOf (void) const;
	virtual VertexData*	GetVertexData (void*, size_t) const;
	virtual size_t		GetNumberOfVertices (void) const;

	// Transformable
	virtual void		Apply (math::Matrix4x4 const &);
	virtual void		Adjust (math::Vector4 const&);

	static size_t		GetSizeOfPoint (void);
	static size_t		GetPointNumberOfVertices (void);

						Point (Vertex const&, Colour const& = Colour(math::Vector4::New(0.6f, 0.6f, 0.6f)));
						Point (Point const&);
	virtual				~Point (void);

private:
	Vertex				point;
}; // class Point

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__POINT__H__
