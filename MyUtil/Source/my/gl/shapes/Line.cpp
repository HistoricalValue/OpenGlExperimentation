#include <stdafx.h>

#include <PConfigurations.h>
#include <my/gl/Shapes/Line.h>

#ifndef P_INLINING
#    define __MY_UTIL__MY__GL__SHAPES__LINE__DEFINING
#    include <my/gl/shapes/Line_inl.h>
#endif

namespace _ {
    static size_t const numberOfVertices    (2u);
}

namespace my { namespace gl { namespace shapes {

    size_t Line::GetLineNumberOfVertices (void) {
        return _::numberOfVertices;
    }

    // Shape
    // -=---
    VertexData* Line::GetVertexData (void* const memory, size_t bytesize) const {

        size_t const N                (_::numberOfVertices);
        size_t const requiredSize    (sizeof(VertexData) * N);
        VertexData* result            (NULL);

        if (bytesize >= requiredSize) {
            result                = reinterpret_cast<VertexData* const>(memory);
            Colour const dim    (ColourFactory::Dimmer(colour));
            Colour const light    (ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(colour))));

            new(&result[0]) VertexData(beginning.xyzw(), dim);
            new(&result[1]) VertexData(end.xyzw(), light);
        }

        return result;
    }

    Line* Line::Clone (void* memory, size_t size) const {
        Line* result(NULL);
        if (size >= sizeof(Line)) {
            Line* const line(reinterpret_cast<Line* const>(memory));

            new(line) Line(*this);

            result = line;
        }

        return result;
    }

}}} // namespace my::gl::shapes