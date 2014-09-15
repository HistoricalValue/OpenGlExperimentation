#include <stdafx.h>

#include <my/gl/shapes/ShapeComposition.h>
#include <MySafeCast.h>

#define FOR_EACH_SHAPE        for (Shape* const* shape = &shapes[0]; shape < &shapes[i]; ++shape)


//////////////////////////////////////////////////////////

namespace my { namespace gl { namespace shapes {

using namespace codeshare::utilities::types;

//////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////

P_INLINE
bool ShapeComposition::IsFull (void) const {
    return GetNumberOfAddedShapes() == GetCapacity();
}

//////////////////////////////////////////////////////////

P_INLINE
void ShapeComposition::Add (Shape* const shape) {
    PASSERT(!IsFull())
    if (!IsFull())
        shapes[i++] = shape;
}

//////////////////////////////////////////////////////////

P_INLINE
size_t ShapeComposition::GetCapacity (void) const {
    return shapesBytesize/sizeof(shapes[0]);
}

//////////////////////////////////////////////////////////

P_INLINE
size_t ShapeComposition::GetNumberOfAddedShapes (void) const {
    return i;
}

//////////////////////////////////////////////////////////

P_INLINE
Shape** ShapeComposition::GetShapesArray (void) const {
    return shapes;
}

//////////////////////////////////////////////////////////
// Transformable
P_INLINE
void ShapeComposition::Apply (math::Matrix4x4 const& mat4) {
    PASSERT(i <= shapesBytesize/sizeof(shapes[0]))

    FOR_EACH_SHAPE
        (*shape)->Apply(mat4);
}

//////////////////////////////////////////////////////////

P_INLINE
void ShapeComposition::Adjust (math::Vector4 const& vec4) {
    PASSERT(i <= shapesBytesize/sizeof(shapes[0]))

    FOR_EACH_SHAPE
        (*shape)->Adjust(vec4);
}

//////////////////////////////////////////////////////////

ShapeComposition::ShapeComposition (Shape** const shapesArrayMemory, size_t const shapesArrayMemoryBytesize):
    Shape(Colour(math::Vector4::New(.6f, .6f, .6f, 1.f))),
#pragma warning( push )
#pragma warning( disable: 4351 ) // elementa of "shapes" will be default initialised
    shapes(shapesArrayMemory),
    shapesBytesize(shapesArrayMemoryBytesize),
    i(0u) {
#pragma warning( pop )
    PASSERT(shapesArrayMemoryBytesize % sizeof(shapes[0]) == 0)
    P_STATIC_ASSERT(sizeof(ShapeComposition) == 0
        + sizeof(Shape)
        + sizeof(shapes)
        + sizeof(shapesBytesize)
        + sizeof(i))
}

//////////////////////////////////////////////////////////

ShapeComposition::~ShapeComposition (void) {
}

//////////////////////////////////////////////////////////
// Shape

VertexData* ShapeComposition::GetVertexData (void* const memory, size_t const bytesize) const {
    using codeshare::utilities::pointer_utilities::reinterpret_assign;
    using codeshare::utilities::pointer_utilities::offset;

    size_t count                (GetNumberOfVertices());
    size_t const requiredSize    (count * sizeof(VertexData));
    VertexData* result            (NULL);

    if (bytesize >= requiredSize) {
        reinterpret_assign(result, memory);
        ptrdiff_t write_offset(0u);

        FOR_EACH_SHAPE {
            VertexData* const subresult((*shape)->GetVertexData(offset(result, write_offset), bytesize-write_offset));
            PASSERT(subresult != NULL)
            write_offset += (*shape)->GetNumberOfVertices() * sizeof(VertexData);
        }
        PASSERT(psafecast<size_t>(write_offset) == requiredSize);
    }

    return result;
}

//////////////////////////////////////////////////////////

TexturedVertexData* ShapeComposition::GetTexturedVertexData (void* const memory, size_t const bytesize) const {
    using codeshare::utilities::pointer_utilities::reinterpret_assign;
    using codeshare::utilities::pointer_utilities::offset;

    size_t count                (GetNumberOfVertices());
    size_t const requiredSize    (count * sizeof(TexturedVertexData));
    TexturedVertexData* result    (NULL);

    if (bytesize >= requiredSize) {
        reinterpret_assign(result, memory);
        ptrdiff_t write_offset(0u);

        FOR_EACH_SHAPE {
            TexturedVertexData* const subresult((*shape)->GetTexturedVertexData(offset(result, write_offset), bytesize-write_offset));
            PASSERT(subresult != NULL)
            write_offset += (*shape)->GetNumberOfVertices() * sizeof(TexturedVertexData);
        }
        PASSERT(psafecast<size_t>(write_offset) == requiredSize);
    }

    return result;
}

//////////////////////////////////////////////////////////

P_INLINE
size_t ShapeComposition::GetNumberOfVertices (void) const {
    PASSERT(i <= shapesBytesize/sizeof(shapes[0]))

    size_t numberOfVertices(0u);

    for (Shape const* const* shape = &shapes[0]; shape < &shapes[i]; ++shape)
        numberOfVertices += (*shape)->GetNumberOfVertices();

    return numberOfVertices;
}

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

}}} // namespace my::gl::shapes

//////////////////////////////////////////////////////////
