#include <stdafx.h>

#include <my/gl/math/Vector4_inl.h>
#include <my/gl/shapes/NShapesComposition_inl.h>

namespace _ {
	using namespace my::gl::shapes;

	const size_t numberOfAxes(3u);

	const Colour DDDBLUE(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Blue()))));
	const Colour BBBBLUE(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Blue()))));

	const Colour DDDRED(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Red()))));
	const Colour BBBRED(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Red()))));

	const Colour DDDGREEN(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Dimmer(ColourFactory::Green()))));
	const Colour BBBGREEN(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Green()))));

	const float step(2.0f / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION);
} // namespace _

namespace my { namespace gl { namespace shapes {

	using _::DDDBLUE;
	using _::BBBBLUE;
	using _::DDDRED;
	using _::BBBRED;
	using _::DDDGREEN;
	using _::BBBGREEN;
	using math::Vector4;
	using _::step;

	Axes::Axes (void):
		Base(),
		lines() {

		{
			LinePlaceholder*	linep(&lines[0]);
			Colour				colx(DDDBLUE);
			math::Vector4 const	colxstep((BBBBLUE - DDDBLUE) / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION);
			Colour				coly(DDDRED);
			math::Vector4 const	colystep((BBBRED - DDDRED) / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION);
			Colour				colz(DDDGREEN);
			math::Vector4 const	colzstep((BBBGREEN - DDDGREEN) / MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION);

			for (float curr((-step * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION) / 2); curr + step < 1.0f; curr += step, ++linep, colx = colx + colxstep, coly = coly + colystep, colz = colz + colzstep) {
				P_STATIC_ASSERT(MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION <= sizeof(lines)/sizeof(lines[0]))

				PASSERT(linep < &lines[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				linep->Construct(Line(Vertex(math::Vector4::New(curr)), Vertex(math::Vector4::New(curr + step)), colx, colx+colxstep));
				Add(linep->GetInternal());

				PASSERT(&linep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION] < &lines[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				linep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].Construct(Line(Vertex(math::Vector4::New(0, curr)), Vertex(math::Vector4::New(0, curr + step)), coly, coly+colystep));
				Add(linep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].GetInternal());

				PASSERT(&linep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION] < &lines[3 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				linep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].Construct(Line(Vertex(math::Vector4::New(0, 0, curr)), Vertex(math::Vector4::New(0, 0, curr + step)), colz, colz+colzstep));
				Add(linep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].GetInternal());
			}
		}

		PASSERT(IsFull());
	}

	Axes::Axes (Axes const& other):
		Base(),
		lines() {
		{
			LinePlaceholder const*			otherlinep	(&other.lines[0]);
			LinePlaceholder const* const	end			(&lines[(sizeof(lines)/sizeof(lines[0])) / 3]);
			for (LinePlaceholder* linep(&lines[0]); linep < end; ++linep, ++otherlinep) {
				PASSERT(linep < &lines[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				PASSERT(otherlinep < &other.lines[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				linep->Construct(*otherlinep->GetInternal());
				Add(linep->GetInternal());

				PASSERT(&linep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION] < &lines[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				PASSERT(&otherlinep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION] < &other.lines[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				linep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].Construct(*otherlinep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].GetInternal());
				Add(linep[MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].GetInternal());

				PASSERT(&linep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION] < &lines[3 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				PASSERT(&otherlinep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION] < &other.lines[3 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION])
				linep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].Construct(*otherlinep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].GetInternal());
				Add(linep[2 * MY_UTIL__MY__GL__SHAPES__AXES__RESOLUTION].GetInternal());
			}
		}

		PASSERT(IsFull());
	}

	Axes::~Axes (void) {
	}

	Axes* Axes::Clone (void* const mem, size_t const bytesize) const {
		size_t const requiredSize(GetSizeOf());

		Axes* result(NULL);
		if (bytesize >= requiredSize)
			result = new(mem) Axes(*this);

		return result;
	}

	size_t Axes::GetSizeOf (void) const {
		return sizeof(Axes);
	}

}}} // namespace my::gl::shapes
