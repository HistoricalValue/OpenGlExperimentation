#ifndef __MY_UTIL__MY__GL__SHAPES__PLANE__H__
#define __MY_UTIL__MY__GL__SHAPES__PLANE__H__

#include <MyUtil.h>
#include <my/gl/shapes/NShapesComposition_inl.h>
#include <PTypes.h>
#include <my/gl/shapes/Triangle.h>

namespace my { namespace gl { namespace shapes {

    class MYUTIL_API Plane: public NShapesComposition<2> {
    public:

        virtual TexturedVertexData*        GetTexturedVertexData (void* memory, size_t bytesize) const;
        virtual Plane*                    Clone (void*, size_t) const;
        virtual size_t                    GetSizeOf (void) const;

                                        Plane (Colour const&);
        virtual                            ~Plane (void);

    private:
        Triangle                        down;
        Triangle                        up;

        typedef NShapesComposition<2>    Base;
        // No public copying
        void                            operator = (Plane const&);
                                        Plane (Plane const&);
    }; // class Shape

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__PLANE__H__
