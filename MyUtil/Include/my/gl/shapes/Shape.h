#ifndef __MY_UTIL__MY__GL__SHAPES__SHAPE__H__
#define __MY_UTIL__MY__GL__SHAPES__SHAPE__H__

#include <MyUtil.h>
#include <my/gl/shapes/Colour.h>
#include <my/gl/shapes/Transformable.h>
#include <my/gl/shapes/VertexData.h>

namespace my { namespace gl { namespace shapes {

    class MYUTIL_API Shape: public Transformable {
    public:
                                        // memory should be at least as much as NumberOfVertices * sizeof(VertexData)
        virtual VertexData*                GetVertexData (void* memory, size_t bytesize) const = 0;
        virtual TexturedVertexData*        GetTexturedVertexData (void* memory, size_t bytesize) const;
                                        // memory should be at least GetSizeOf() bytes
        virtual Shape*                    Clone (void* memory, size_t bytesize) const = 0;

        virtual                            ~Shape (void);

        virtual size_t                    GetSizeOf (void) const = 0;
        virtual size_t                    GetNumberOfVertices (void) const = 0;

        void                            SetColour (Colour const&);
        Colour const&                    GetColour (void) const;
        Shape const&                    AdjustColour (float f)        { SetColour(GetColour() * f); return *this; }

    protected:
                                        Shape (Colour const& colour);
                                        Shape (Shape const&);

        Colour                            colour;
    private:
        void    operator = (Shape const&);
    }; // class Shape


    inline TexturedVertexData* Shape::GetTexturedVertexData (void* const, size_t const) const
        { return NULL; }

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__SHAPE__H__
