#ifndef __MY_UTIL__MY__ALGO__SHAPE_PRODUCERS__H__
#define __MY_UTIL__MY__ALGO__SHAPE_PRODUCERS__H__

#include <my/gl/shapes/Colour.h>
#include <my/gl/shapes/ColourFactory.h>
#include <my/gl/math/Vector4.h>
#include <my/gl/shapes/Line.h>
#include <my/gl/shapes/Vertex.h>

namespace my { namespace algo {

template <typename IterInType, typename Cout, typename VectorMakerType>
static Cout& map_vec4_to_linestrip (
        IterInType const&                vec_begin,
        IterInType const&                vec_end,
        Cout&                            lines,
        my::gl::shapes::Colour const&    col,
        VectorMakerType const&            makevector4) {
    using my::gl::math::Vector4;
    using my::gl::shapes::Line;
    using my::gl::shapes::Vertex;
    using my::gl::shapes::Colour;
    using my::gl::shapes::ColourFactory;

    Colour const bright(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(col)))));

    typename IterInType i(vec_begin);
    DASSERT(i != vec_end);
    typename IterInType prev(i++);

    for (; i != vec_end; prev = i++)
        lines.push_back(Line(Vertex(makevector4(*prev)), Vertex(makevector4(*i)), col, bright));

    return lines;
}
template <typename Cin, typename Cout, typename VectorMakerType>
static inline Cout& map_vec4_to_linestrip (
        Cin const&                        c,
        Cout&                            lines,
        my::gl::shapes::Colour const&    col,
        VectorMakerType const&            makevector4)
    { return map_vec4_to_linestrip(c.begin(), c.end(), lines, col, makevector4); }

template <typename IterInType, typename Cout, typename VectorMakerType>
static Cout& map_vec4_to_points (
        IterInType const&                vec_begin,
        IterInType const&                vec_end,
        Cout&                            points,
        my::gl::shapes::Colour const&    col,
        VectorMakerType const&            makevector4) {
    using my::gl::math::Vector4;
    using my::gl::shapes::Point;
    using my::gl::shapes::Vertex;
    using my::gl::shapes::Colour;

    typename IterInType i(vec_begin);

    for (; i != vec_end; i++)
        points.push_back(Point(Vertex(makevector4(*i)), col));

    return points;
}
template <typename Cin, typename Cout, typename VectorMakerType>
static inline Cout& map_vec4_to_points (
        Cin const&                        c,
        Cout&                            points,
        my::gl::shapes::Colour const&    col,
        VectorMakerType const&            makevector4)
    { return map_vec4_to_points(c.begin(), c.end(), points, col, makevector4); }



}} // namespace my::algo

#endif // __MY_UTIL__MY__ALGO__SHAPE_PRODUCERS__H__
