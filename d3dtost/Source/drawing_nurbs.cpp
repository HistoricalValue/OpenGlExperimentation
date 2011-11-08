#include "stdafx.h"
#include "TheCursed.h"
//
#pragma warning( push, 0 )
#	include <Mesh.h>
#	include <MeshLoader.h>
#pragma warning( pop )
//
#include <my/algo/ShapeProducers.h>

#include <drawing_utils.h>
#include <drawing_nurbs.h>

#include <Curve.h>
#include <CurveAlgorithms.h>
#include <Knots.h>
#include <SurfaceAlgorithms.h>
#include <NurbsFacade.h>

//////////////////////////////////////////////////////////////////////////////////////

// CONTROL SWITCHES

//////////////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#	define NOTEND(I,END) _NOTEND(I,END)
#else
#	define NOTNED(I,END)
#endif

template <typename I>
static inline I& _NOTEND (I& i, I const& end) {
	DASSERT(i != end);
	return i;
}


namespace my { namespace drawing {
//////////////////////////////////////////////////////////////////////////////////////
//    LE SHIT !!!   //
namespace nurbs {
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
namespace _ {
//////////////////////////////////////////////////////////////////////////////////////

static bool VerifyBaseFunctions (size_t const m, std::vector<ankh::nurbs::Unit> const& knots) {
	using ankh::math::trig::vec4;
	using ankh::nurbs::Curve;
	using ankh::nurbs::algo::simple::N;
	using ankh::nurbs::Unit;
	using ankh::nurbs::DefaultPrecision;
	using ankh::nurbs::TesselationParameters;

	DASSERT(m >= 1);
	size_t const l(knots.size() - 1);
	size_t const n(l - m);
	size_t const numcpoints(Curve::NumberOfControlPointsFor(m, knots.size()));
	DASSERT(numcpoints == n+1);

	std::list<vec4> empty(numcpoints, vec4());
	Curve spl(empty.begin(), empty.end(), knots.begin(), knots.end());

	Curve::Domain const	domain	(spl.GetDomainOfDefinition());

	TesselationParameters tp(1e-2f);

	// Verify that N_i,m has support in [u_i, u_i+m] for m>=1 and is also >=0 for all u in domain of definition
	for (size_t i(0); i <= n; ++i) { // foreach control point
		for (Unit u(domain.first); u <= domain.last; u += tp.step) { // for all u in definition range
			float const	Nval			(N(spl,i,m,u));
			float const	u_i				(spl.GetKnot(i));
			float const	u_i_m			(spl.GetKnot(i+m));
			bool const	u_lt_u_i		(u < u_i);
			bool const	u_gt_u_i_m		(u > u_i_m);
			bool const	Nval_lt_zero	(Nval < 0.0f);
			bool const	Nval_is_zero	(tp.prec.eq(Nval));

			if (u_lt_u_i || u_gt_u_i_m) {
				bool const isok(Nval_is_zero);
				DASSERT(isok);
				if (!isok)
					return false;
			}

			if (Nval_lt_zero) {
				DASSERT(false);
				return false;
			}
		}
	}

	// Verify that SUM N_i,m(u) = 1 for i in [0,n] for all u in [u_m-1, u_n+1]
	for (Unit u(domain.first); u <= domain.last; u += tp.step) {
		Unit sum(0.0f);

		for (size_t i(0); i <= n; ++i) {
			sum += N(spl,i,m,u);
			DASSERT(tp.prec.lt(sum, 1.0f) || tp.prec.eq(sum, 1.0f));
		}

		if (!tp.prec.eq(sum, 1.0f)) {
			DASSERT(false);
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct nexter {
public:
	T const operator () (void) { return val++; }
	nexter (T const& init): val(init) {}
	nexter (nexter<T> const& other): val(other.val) {}
private:
	T val;
};
template <typename T>
nexter<T> makenexter (T const& init) { return nexter<T>(init); }

static bool VerifyMultiplicityChecker (void) {
	typedef std::vector<float>	Knots;
	using ankh::nurbs::Knots_IsMultiplicityLessThanOrEqualTo;

	Knots knots(20);

	DASSERT(Knots_IsMultiplicityLessThanOrEqualTo(knots.begin(), knots.end(), 14) == false);

	std::generate_n(knots.begin() + 5, 10, makenexter(1u));
	DASSERT(Knots_IsMultiplicityLessThanOrEqualTo(knots.begin(), knots.end(), 4) == true);

	std::fill_n(knots.begin() + 10, 4, 0.0f);
	DASSERT(Knots_IsMultiplicityLessThanOrEqualTo(knots.begin(), knots.end(), 4) == true);

	knots.assign(16, 0.0f);
	DASSERT(Knots_IsMultiplicityLessThanOrEqualTo(knots.begin(), knots.end(), 4) == false);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
} // _
//////////////////////////////////////////////////////////////////////////////////////

void Initialise (void) {
	using namespace ankh::nurbs;
	using namespace ankh::nurbs::algo;

	ankh::shapes::volumes::BoundingVolume::SingletonCreate();
	ankh::shapes::MeshLoader::SingletonCreate();

	DASSERT(_::VerifyBaseFunctions(
			FirstCrossSection(Surfaces::MoonValleyWithHorns()).m(),
			KnotsI(Surfaces::MoonValleyWithHorns())));

	DASSERT(_::VerifyMultiplicityChecker());
}

void CleanUp (void) {
	ankh::shapes::MeshLoader::SingletonDestroy();
	ankh::shapes::volumes::BoundingVolume::SingletonDestroy();
}

//////////////////////////////////////////////////////////////////////////////////////

static inline void getpathformesh (std::string& into, char const* const id, char const* const ext) {
	static const char	meshes_folder_path[] = "../meshes/";
	static const size_t	meshes_folder_path_length = countof(meshes_folder_path);

	const size_t totalLength = meshes_folder_path_length + strlen(id) + strlen(ext) + 2;
	into.reserve(totalLength);

	into = meshes_folder_path;
	into += id;
	into += ".";
	into += ext;
}

ankh::shapes::Mesh* load (char const* const id) {
	std::string loadpath;
	getpathformesh(loadpath, id, "msh");

	return DNULLCHECK(ankh::shapes::MeshLoader::GetSingleton().Load(loadpath));
}

void unload (ankh::shapes::Mesh* const mesh)
	{ ankh::shapes::MeshLoader::GetSingleton().Unload(mesh); }

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


void addastrianglesto (ankh::shapes::Mesh::Elements const& elements, my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::Vector4;

	Colour const colour	(Vector4::New(0.8f, 0.4f, 0.8f));
	timer t02("transfoming as triagnles to my::gl::shapes triangles (and adding to factory)");
	ankh::shapes::Mesh::Elements::const_iterator const end(elements.end());
	for (ankh::shapes::Mesh::Elements::const_iterator i(elements.begin()); i != end; ++i)
		f.Add(maketriangle(*i, colour));
}

void addnormalsto (ankh::shapes::Mesh::Elements const& elements, my::gl::shapes::ShapeCompositionFactory& f) {
	ankh::shapes::Mesh::Elements::const_iterator const end(elements.end());
	for (ankh::shapes::Mesh::Elements::const_iterator i(elements.begin()); i != end; ++i) {
		f.Add(makenormallineformeshelementvertex(*i, 0));
		f.Add(makenormallineformeshelementvertex(*i, 1));
		f.Add(makenormallineformeshelementvertex(*i, 2));
	}
}

void addcontrolpointsto (ankh::nurbs::Surface const& surf, my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::vec4;
	using ankh::nurbs::Curve;
	using my::algo::map_vec4_to_points;
	using my::gl::shapes::Point;
	using ankh::nurbs::Curve;
	using ankh::nurbs::Surface;
	using ankh::nurbs::Knots;
	using ankh::nurbs::algo::CrossSection;

	std::list<Point>points;
	size_t const	end	(surf.GetControlPointsWidth());
	size_t			i	(0);

	for (; i < end; ++i)
		points.clear(),
		f.AddAll(	map_vec4_to_points(
						surf.GetControlPointsJBegin(i),
						surf.GetControlPointsJEnd(i),
						points,
						Colour(vec4::New(0.5f, 0.7f, 0.7f)),
						&makevector4));
}

void addknotpointsto (ankh::nurbs::Surface const& surf, my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::Vector4;
	using ankh::math::trig::vec4;
	using ankh::nurbs::Curve;
	using my::algo::map_vec4_to_points;
	using my::gl::shapes::Point;
	using ankh::nurbs::Curve;
	using ankh::nurbs::Surface;
	using ankh::nurbs::Knots;
	using ankh::nurbs::algo::CrossSection;
	using ankh::nurbs::tesselation::IneffectiveNormalCalculator;
	using ankh::nurbs::tesselation::deboor::At;
	using ankh::nurbs::algo::optimised::p;

	std::list<vec4>	vectors;
	std::list<Point>points;
	size_t			c(surf.GetFirstKnotJInDomainIndex());
	size_t const	c_end(surf.GetLastKnotJInDomainIndex());

	for (; c != c_end; ++c) {
		vectors.clear();
		points.clear();

		Curve const curve(CrossSection(surf, c));

		size_t			i	(curve.GetFirstKnotInDomainIndex());
		size_t const	last(curve.GetLastKnotInDomainIndex());

		for (; i <= last; ++i)
			vectors.push_back(At<&p>(curve, i == last? i-1 : i, curve.GetKnot(i)));

		std::list<Point> points;
		f.AddAll(	map_vec4_to_points(
						vectors,
						points,
						Colour(Vector4::New(0.7f, 0.5f, 0.5f)),
						&makevector4));
	}
}

void addbasecurvesto (ankh::nurbs::Surface const& surf, my::gl::shapes::ShapeCompositionFactory& f) {
	using namespace							ankh::nurbs;
	using namespace							my::gl::shapes;
	using ankh::math::trig::				vec4;
	using my::gl::math::					Vector4;
	using tesselation::deboor::				ProduceAll;
	using my::algo::						map_vec4_to_linestrip;
	using ankh::nurbs::tesselation::		IneffectiveNormalCalculator;
	using ankh::nurbs::						VertexWithNormal;
	using ankh::nurbs::						TesselationParameters;
	using ankh::nurbs::tesselation::		Resolution;
	using ankh::nurbs::algo::optimised::	p;
	using ankh::nurbs::tesselation::		VertexWithNormalAppenderTesselationCallback;
	using ankh::nurbs::tesselation::		Points;

	Points												points;
	std::list<Line>										lines;
	Colour const										colour(Vector4::New(0.4f, 0.2f, 0.8f));
	IneffectiveNormalCalculator const					inc;
	TesselationParameters const							tp(1e-1f);
	VertexWithNormalAppenderTesselationCallback<Points>	tesselationCallback(&points);

	for (size_t i(0); i < surf.GetControlPointsWidth(); ++i) {
		Curve const&	base		(surf.GetAlongBase(i));
		size_t const	resolution	(Resolution(base, tp));

		points.clear();
		points.reserve(resolution);

		lines.clear();

		ProduceAll<&p>(base, tp, inc, tesselationCallback);

		f.AddAll(map_vec4_to_linestrip(points, lines, colour, &makevector4fromvertexwithnormal));
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//   as dasd a s   //
}	// nurbs
//////////////////////////////////////////////////////////////////////////////////////


}} // my::drawing

