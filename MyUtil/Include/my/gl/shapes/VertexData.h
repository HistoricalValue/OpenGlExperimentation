#ifndef __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__H__
#define __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__H__

#include <MyUtil.h>
#include <my/gl/shapes/Vertex.h>
#include <my/gl/shapes/Colour.h>

namespace my { namespace gl { namespace shapes {

    class MYUTIL_API VertexData {
    public:
                                                        VertexData (math::Vector4 const&, Colour const&, math::Vector4 const& = math::Vector4::New(), float AO = 1.0f);
                                                        VertexData (VertexData const&);
                                                        ~VertexData (void);
                                                        UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(VertexData)

        math::Vector4 const&                            GetPosition (void) const;
        Colour const&                                    GetColour (void) const;


        static size_t                                    PositionOffset (void);
        static size_t                                    ColourOffset (void);
        static size_t                                    NormalOffset (void);
        static size_t                                    AOOffset (void);
        static void*                                    PositionOffsetPointer (void);
        static void*                                    ColourOffsetPointer (void);
        static void*                                    NormalOffsetPointer (void);
        static void*                                    AOOffsetPointer (void);
        static size_t                                    Stride (void);
    private:
        math::Vector4 const                                position;
        Colour const                                    colour;
        math::Vector4 const                                normal;
        float const                                        ambientOcclusion;

        static size_t                                    Offset (size_t);
    }; // class VertexData

    class MYUTIL_API TexturedVertexData {
    public:
        static size_t                                    PositionOffset (void);
        static size_t                                    TextureCoordinatesOffset (void);
        static size_t                                    NormalOffset (void);
        static size_t                                    AOOffset (void);
        static void*                                    PositionOffsetPointer (void);
        static void*                                    TextureCoordinatesOffsetPointer (void);
        static void*                                    NormalOffsetPointer (void);
        static void*                                    AOOffsetPointer (void);
        static size_t                                    Stride (void);

                                                        TexturedVertexData (math::Vector4 const& pos, math::Vector4 const& textCoords, math::Vector4 const& normal = math::Vector4::New(), float AO = 1.0f);
                                                        ~TexturedVertexData (void);
    private:
                                                        TexturedVertexData (TexturedVertexData const&);
        void                                            operator = (TexturedVertexData const&);

        math::Vector4 const                                position;
        math::Vector4 const                                textureCoordinate;
        math::Vector4 const                                normal;
        float const                                        ambientOcclusion;
    };

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__VERTEX_DATA__H__