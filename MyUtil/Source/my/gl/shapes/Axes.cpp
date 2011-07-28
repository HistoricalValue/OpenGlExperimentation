#include "stdafx.h"

#include "my/gl/math/Vector4_inl.h"

namespace _ {
	using namespace my::gl::shapes;

	const size_t numberOfAxes(3u);

	const Colour DDDBLUE(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Blue()))));
	const Colour BBBBLUE(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Blue()))));

	const Colour DDDRED(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Red()))));
	const Colour BBBRED(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Red()))));

	const Colour DDDGREEN(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Green()))));
	const Colour BBBGREEN(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Green()))));
} // namespace _

namespace my { namespace gl { namespace shapes {

	using _::DDDBLUE;
	using _::BBBBLUE;
	using _::DDDRED;
	using _::BBBRED;
	using _::DDDGREEN;
	using _::BBBGREEN;
	using math::Vector4;

	Axes::Axes (void):
		ShapeComposition(&shapesArray[0], sizeof(shapesArray)),
		lines(),
#pragma warning( push )
#pragma warning( disable: 4351 ) // elements of "shapesArray" will be default initialised
		shapesArray() {
#pragma warning( pop )
		P_STATIC_ASSERT(sizeof(Axes) == sizeof(ShapeComposition) + sizeof(lines) + sizeof(shapesArray))
		PASSERT(DDDBLUE < BBBBLUE)

		Vertex const	x_step			(math::Vector4::New(1.f / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION, 0.f, 0.f, 0.f));
		Vertex			x_vertex		(math::Vector4::New(-1.f, 0.f, 0.f, 1.f));
		Colour			x_colour1		(DDDBLUE);
		Colour			x_colour2		(BBBBLUE);
		Vector4 const	x_colourStep	((BBBBLUE - DDDBLUE) / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION);

		Vertex const	y_step			(math::Vector4::New(0.f, 1.f / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION, 0.f, 0.f));
		Vertex			y_vertex		(math::Vector4::New(0.f,-1.f, 0.f, 1.f));
		Colour			y_colour1		(DDDRED);
		Colour			y_colour2		(BBBRED);
		Vector4 const	y_colourStep	((BBBRED - DDDRED) / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION);

		Vertex const	z_step			(math::Vector4::New(0.f, 0.f, 1.f / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION, 0.f));
		Vertex			z_vertex		(math::Vector4::New(0.f, 0.f,-1.f, 1.f));
		Colour			z_colour1		(DDDGREEN);
		Colour			z_colour2		(BBBGREEN);
		Vector4 const	z_colourStep	((BBBGREEN - DDDGREEN) / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION);

		for (LinePlaceholder* placeholder(&lines[0]); placeholder < &lines[(sizeof(lines)/sizeof(lines[0]))/3]; ++placeholder) {
			Vertex		x_end(x_vertex + x_step);
			Add(placeholder[0 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].Construct(Line(x_vertex, x_end, x_colour1, x_colour2)));;
			x_vertex	= x_end;
			x_colour1	= x_colour1 + x_colourStep;
			x_colour2	= x_colour2 + x_colourStep;


			Vertex		y_end(y_vertex + y_step);
			Add(placeholder[1 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].Construct(Line(y_vertex, y_end, y_colour1, y_colour2)));
			y_vertex	= y_end;
			y_colour1	= y_colour1 + y_colourStep;
			y_colour2	= y_colour2 + y_colourStep;

			Vertex		z_end(z_vertex + z_step);
			Add(placeholder[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].Construct(Line(z_vertex, z_end, z_colour1, z_colour2)));
			z_vertex	= z_end;
			z_colour1	= z_colour1 + z_colourStep;
			z_colour2	= z_colour2 + z_colourStep;
		}
	}

	Axes::~Axes (void) {
	}

}}} // namespace my::gl::shapes
