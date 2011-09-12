#ifndef __MY_UTIL__MY__ALGO__SHAPE_PRODUCERS__H__
#define __MY_UTIL__MY__ALGO__SHAPE_PRODUCERS__H__

#include <my/gl/shapes/Colour.h>
#include <my/gl/shapes/ColourFactory.h>
#include <my/gl/math/Vector4.h>
#include <my/gl/shapes/Line.h>
#include <my/gl/shapes/Vertex.h>

namespace my { namespace algo {

template <typename Cin, typename Cout, typename VectorMakerType>
static Cout& map_vec4_to_linestrip (Cin const& points, Cout& dest, my::gl::shapes::Colour const& col, VectorMakerType const& makevector4) {
	using my::gl::math::Vector4;
	using my::gl::shapes::Line;
	using my::gl::shapes::Vertex;
	using my::gl::shapes::Colour;
	using my::gl::shapes::ColourFactory;

	typename Cin::const_iterator points_end(points.end());
	typename Cin::const_iterator i(points.begin());
	DASSERT(i != points_end);
	typename Cin::const_iterator prev(i++);

	Colour const bright(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(col)))));

	for (; i != points_end; prev = i++)
		dest.push_back(Line(Vertex(makevector4(*prev)), Vertex(makevector4(*i)), col, bright));

	return dest;
}

}} // namespace my::algo 

#endif // __MY_UTIL__MY__ALGO__SHAPE_PRODUCERS__H__
