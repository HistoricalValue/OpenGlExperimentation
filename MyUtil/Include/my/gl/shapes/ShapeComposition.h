#ifndef __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__H__
#define __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__H__


#include <MyUtil.h>
#include <my/gl/shapes/Shape.h>

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API ShapeComposition: public Shape {
	public:
														ShapeComposition (Shape** shapesArrayMemory, size_t shapesArrayMemoryBytesize);
		virtual											~ShapeComposition (void);

		bool											IsFull (void) const;
		void											Add (Shape*);


		// Shape
		VertexData*										GetVertexData (void* memory, size_t bytesize) const;
		TexturedVertexData*								GetTexturedVertexData (void* memory, size_t bytesize) const;
		ShapeComposition*								Clone (void* memory, size_t bytesize) const;
		size_t											GetNumberOfVertices (void) const;
		size_t											GetSizeOf (void) const;

		// Transformable
		void											Apply (math::Matrix4x4 const&);
		void											Adjust (math::Vector4 const&);

	protected:
		Shape**											GetShapesArray (void) const;
		size_t											GetCapacity (void) const;
		size_t											GetNumberOfAddedShapes (void) const;

		virtual void									CloneSelf (void* here) = 0;

	private:
		Shape** const									shapes;
		size_t const									shapesBytesize;
		size_t											i;

														ShapeComposition (ShapeComposition const&);
		void											operator = (ShapeComposition const&);
	}; // class ShapeComposition

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__H__
