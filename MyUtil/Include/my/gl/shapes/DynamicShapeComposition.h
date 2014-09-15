#ifndef __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION__H__
#define __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION__H__

#include <MyUtil.h>
#include <my/gl/shapes/SpecificNumberOfShapesComposition.h>

//////////////////////////////////////////////////////////

namespace my { namespace gl { namespace shapes {

//////////////////////////////////////////////////////////

#pragma warning( push )
#pragma warning( disable: 4263 4264 )    // apparently, Add(Shape const&) does not overload the Add() function
                                        // (because it's virtual?) but instead (since it also does not override
                                        // it), hides it.

/// Dynamic Shape Composition keeps a Clone()ed value of each
/// added shape.
// concrete
class MYUTIL_API DynamicShapeComposition: public SpecificNumberOfShapesComposition {
public:
    void                                Add (Shape const&);            // Shape is cloned
    bool                                IsFull (void) const;

                                        DynamicShapeComposition (size_t numberOfShapes);
    virtual                                ~DynamicShapeComposition (void);

    virtual DynamicShapeComposition*    Clone (void*, size_t) const;
    virtual size_t                        GetSizeOf (void) const;

private:
    // No public copying
    void                                operator = (DynamicShapeComposition const&);
                                        DynamicShapeComposition (DynamicShapeComposition const&);
}; // class DynamicShapeComposition

#pragma warning( pop )
//////////////////////////////////////////////////////////

}}} // namespace my::gl::shapes

//////////////////////////////////////////////////////////

#endif // __MY_UTIL__MY__GL__SHAPES__DYNAMIC_SHAPE_COMPOSITION__H__
