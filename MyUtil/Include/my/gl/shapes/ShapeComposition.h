#ifndef __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__H__
#define __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__H__


#include <MyUtil.h>
#include <my/gl/shapes/Shape.h>

namespace my { namespace gl { namespace shapes {

	// abstract
	class MYUTIL_API ShapeComposition: public Shape {
	public:
		virtual							~ShapeComposition (void);

		virtual	void					Add (Shape*);
		// Shape
		virtual VertexData*				GetVertexData (void* memory, size_t bytesize) const;
		virtual TexturedVertexData*		GetTexturedVertexData (void* memory, size_t bytesize) const;
		virtual size_t					GetNumberOfVertices (void) const;

		Shape**							GetShapesBegin (void) const	{ return &GetShapesArray()[0]; }
		Shape**							GetShapesEnd (void) const	{ return &GetShapesArray()[GetNumberOfAddedShapes()]; }
		size_t							GetNumberOfAddedShapes (void) const;
		size_t							GetCapacity (void) const;
		bool							IsFull (void) const;

		// Transformable
		virtual void					Apply (math::Matrix4x4 const&);
		virtual void					Adjust (math::Vector4 const&);

	protected:
		void							AddShapeToComposition (Shape*);
		Shape**							GetShapesArray (void) const;
										ShapeComposition (Shape** shapesArrayMemory, size_t shapesArrayMemoryBytesize);

	private:
		// State
		Shape** const					shapes;
		size_t const					shapesBytesize;
		size_t							i;
		// ---
										ShapeComposition (ShapeComposition const&);
		void							operator = (ShapeComposition const&);
	}; // class ShapeComposition

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPE__SHAPE_COMPOSITION__H__
