#include "stdafx.h"

#include <SurfacesFacade.h>
#include <AmbientOcclusion.h>

//
#include <Mesh.h>
#include <MeshLoader.h>
//
#include <my/algo/ShapeProducers.h>

#include <drawing_utils.h>
#include <drawing_nurbs.h>


//////////////////////////////////////////////////////////////////////////////////////

// CONTROL SWITCHES
#define USE_DE_BOOR								0 // TODO figure out DEBOOR + weghted control points
#define WITH_OPTIMISED_DE_BOOR					1
#define WITH_FAST_CONTROL_POINT_INTERPOLATOR	1
#define WITH_OPTIMISED_BLENDING					0

//////////////////////////////////////////////////////////////////////////////////////

// CONTROL SWITCHES RESULTS
#if WITH_FAST_CONTROL_POINT_INTERPOLATOR == 1
#	define CONTROL_POINT_INTERPOLATION	&ankh::surfaces::algo::FastControlPointInterpolation
#elif WITH_FAST_CONTROL_POINT_INTERPOLATOR == 0
#	define CONTROL_POINT_INTERPOLATION	&ankh::surfaces::algo::PreciceControlPointInterpolation
#else
#	error "wat"
#endif

template <const int WithOptimisedDeBoor> struct DeBoorOptimisationTraits;
template <> struct DeBoorOptimisationTraits<1> {
	typedef ankh::surfaces::tesselation::OptimisedInterpolatingTraits<CONTROL_POINT_INTERPOLATION>	Traits; };
template <> struct DeBoorOptimisationTraits<0> {
	typedef ankh::surfaces::tesselation::SimpleInterpolatingTraits<CONTROL_POINT_INTERPOLATION>		Traits; };

template <const int WithOptimisedBlending> struct BlendingOptimisationTraits;
template <> struct BlendingOptimisationTraits<1> { typedef ankh::surfaces::tesselation::OptimisedBlendingTraits		Traits; };
template <> struct BlendingOptimisationTraits<0> { typedef ankh::surfaces::tesselation::SimpleBlendingTraits		Traits; };

#if USE_DE_BOOR
#	define DE_BOOR_OR_NOT_DE_BOOR(FUNC) ankh::surfaces::tesselation::deboor:: FUNC <DeBoorOptimisationTraits <WITH_OPTIMISED_DE_BOOR>::Traits >
#	define NOT_USING_DE_BOOR_CHECK()	DASSERT(!"Using De-Boor's algorithm, this function shouldn't be used")
#else
#	define DE_BOOR_OR_NOT_DE_BOOR(FUNC)	ankh::surfaces::tesselation::blending:: FUNC <BlendingOptimisationTraits <WITH_OPTIMISED_BLENDING>::Traits >
#	define NOT_USING_DE_BOOR_CHECK()
#endif


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

static bool VerifyBaseFunctions (short const m, std::vector<ankh::surfaces::Unit> const& knots) {
	using ankh::math::trig::vec4;
	using ankh::surfaces::Curve;
	using ankh::surfaces::algo::simple::N;
	using ankh::surfaces::Unit;
	using ankh::surfaces::DefaultPrecision;
	using ankh::surfaces::TesselationParameters;

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
	using ankh::surfaces::Knots_IsMultiplicityLessThanOrEqualTo;

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

typedef ankh::shapes::Mesh::Elements			MeshElements;

static ankh::surfaces::Surface*	_surf(NULL);
static ankh::shapes::Mesh*		_mesh(NULL);

static inline ankh::surfaces::Surface const& getsurf (void)
	{ return *DPTR(_DNOTNULL(_surf)); }

static inline void unew_mesh (void)
	{ unew(_mesh); }
static inline void udelete_mesh (void)
	{ udelete(_mesh); }
static inline ankh::shapes::Mesh& getmesh (void)
	{ return *DPTR(DNULLCHECK(_mesh)); }
static inline MeshElements const& meshElements (void)
	{ return _::getmesh().GetElements(); }

static float	minx(-0.025f);
static float	maxx( 0.025f);
static float	miny(-0.025f);
static float	maxy( 0.025f);
static float	minz(-0.025f);
static float	maxz( 0.025f);

//////////////////////////////////////////////////////////////////////////////////////

static inline void printmeshinfo (void) {
	using ankh::shapes::Mesh;
	using ankh::math::types::Vertex;

	Mesh& m(getmesh());
	MeshElements const& els(m.GetElements());

	typedef MeshElements::const_iterator  ite_t;
	
	float minx(FLT_MAX), maxx(-FLT_MAX), miny(FLT_MAX), maxy(-FLT_MAX), minz(FLT_MAX), maxz(-FLT_MAX);

	ite_t const els_end(els.end());
	for (ite_t el(els.begin()); el != els_end; ++el)
		for (unsigned int i(0); i < 3; ++i) {
			const Vertex& v(el->GetVertex(i));
			if (v.x < minx)
				minx = v.x;
			if (v.x > maxx)
				maxx = v.x;
			if (v.y < miny)
				miny = v.y;
			if (v.y > maxy)
				maxy = v.y;
			if (v.z < minz)
				minz = v.z;
			if (v.z > maxz)
				maxz = v.z;
		}

	{
		TCHAR buf[1024];
		_sntprintf_s(&buf[0], _countof(buf), _countof(buf)-1, _T("max<%f %f %f>    min<%f %f %f>\n"),
				maxx, maxy, maxz, minx, miny, minz);
		my::global::log::info(&buf[0]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
} // _
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void Initialise (void) {

	{
		DASSERT((_::minx < 0 && _::miny < 0 && _::maxx > 0 && _::maxy > 0));

		using my::gl::math::		Vector4;
		using ankh::math::trig::	vec4;
		using ankh::math::trig::	vec3;
		using ankh::surfaces::		Curve;
		using ankh::surfaces::		ControlPoints;
		using ankh::surfaces::		Knots;
		using ankh::surfaces::		FillRandomly;
		using ankh::surfaces::		FillGridUniformly;
		using ankh::surfaces::		VaryGrid;
		using ankh::surfaces::		FillUniformly;
		using ankh::surfaces::		Surface;
		using ankh::surfaces::		Unit;
		using ankh::surfaces::algo::FirstCrossSection;
		using ankh::surfaces::		ControlPointsGrid;

		size_t const	width_units	(16);
		size_t const	height_units(16);
		size_t const	depth_units	(16);

		const bool		inverseX(false);
		const bool		inverseZ(false);
		
		size_t const	order_j			(0x04u)
					,	order_i			(0x05u)
					,	numcpoints_j	(0x10u)
					,	numcpoints_i	(0x15u)
					,	numknots_j		(Curve::NumberOfKnotsFor(order_j, numcpoints_j))
					,	numknots_i		(Curve::NumberOfKnotsFor(order_i, numcpoints_i))
					;
	//	Unit const		variation	(0.0f);
		Unit const		variation	(0.00625f);
	//	Unit const		variation	(0.0125f);
	//	Unit const		variation	(0.05f);

		long seed;
		{
			std::ifstream fin("./seed.txt", std::ios::in);
			DASSERT(!fin.bad());

			std::string str;
			std::getline(fin, str);
			seed = atol(str.c_str());
		}

		ControlPoints				cpoints;
		Knots						knots_j, knots_i;
		ControlPointsGrid			cpoints_i;

		cpoints_i.reserve(numcpoints_i);
		cpoints.reserve(numcpoints_j);
		knots_j.reserve(numknots_j);
		knots_i.reserve(numknots_i);

		FillUniformly(knots_j, numknots_j, 0.0f, 1.0f);
		FillUniformly(knots_i, numknots_i, 0.0f, 1.0f);
		
		//for (size_t curve_i(0u); curve_i < numcurves; ++curve_i)
		//	cpoints.clear(),
		//	
		//	cpoints_j.push_back(
		//			ControlPoints_FillRandomly(
		//				cpoints, numcpoints,
		//				minx, maxx, miny, maxy, minz, maxz,
		//				width_units, height_units, depth_units, seed + curve_i));
		
		VaryGrid(
			FillGridUniformly(	cpoints_i,
								numcpoints_i,
								numcpoints_j,
								makeinverser<inverseX>(_::minx, _::maxx).a, makeinverser<inverseX>(_::minx, _::maxx).b,
								makeinverser<inverseZ>(_::minz, _::maxz).a, makeinverser<inverseZ>(_::minz, _::maxz).b,
								(_::miny + _::maxy)/2.0f,
								ankh::surfaces::Precision(5e-9f))
			,variation, variation, variation, 1.5f, false, seed)
		;
		cpoints_i.at(3).at(3) = vec4(_::maxx, _::maxy, _::maxz, 1.0f);
		cpoints_i.at(3).at(3) *= 2.0f;
	
		cpoints_i.at(9).at(3).y = -_::maxy;
		cpoints_i.at(9).at(3) *= 4.0f;
	
		cpoints_i.at(15).at(5).y = _::maxy;
		cpoints_i.at(15).at(5) *= 0.5f;

		_::_surf = DNEWCLASS(Surface, (knots_j.begin(), knots_j.end(), knots_i.begin(), knots_i.end(), cpoints_i.begin(), cpoints_i.end(), "BOB ROSS"));
		_::unew_mesh();
		ankh::shapes::MeshLoader::SingletonCreate();

		DASSERT(_::VerifyBaseFunctions(FirstCrossSection(*_::_surf).m(), knots_i));

		DASSERT(_::VerifyMultiplicityChecker());
	}

}

void CleanUp (void) {
	ankh::shapes::MeshLoader::SingletonDestroy();
	_::udelete_mesh();
	udelete(_::_surf);
}

void tesselate (ankh::surfaces::TesselationParameters const* const _tp) {
	using ankh::surfaces::tesselation::	SimpleBlendingTraits;
	using ankh::surfaces::tesselation::	SimpleInterpolatingTraits;
	using ankh::surfaces::tesselation::	OptimisedBlendingTraits;
	using ankh::surfaces::tesselation::	OptimisedInterpolatingTraits;
	using ankh::surfaces::algo::		FastControlPointInterpolation;
	using ankh::surfaces::algo::		PreciceControlPointInterpolation;
#if 0
	using ankh::surfaces::tesselation::blending::	ProduceAllFromAcrossSections;
	using ankh::surfaces::tesselation::blending::	ProduceAllFromAlongSections;
	typedef SimpleBlendingTraits							t;
#else
	using ankh::surfaces::tesselation::deboor::	ProduceAllFromAcrossSections;
	using ankh::surfaces::tesselation::deboor::	ProduceAllFromAlongSections;
//	typedef OptimisedInterpolatingTraits<FastControlPointInterpolation>		t;
	typedef SimpleInterpolatingTraits<PreciceControlPointInterpolation>		t;
#endif
	using ankh::math::types::		Triangle;
	typedef std::vector<Triangle>	Triangles;
	using ankh::shapes::			MeshElement;
	using ankh::shapes::			MeshAdjacencyElement;
	using ankh::surfaces::			Surface;
	using my::gl::shapes::			Colour;
	using my::gl::math::			Vector4;
	using my::gl::shapes::			Line;
	using my::gl::shapes::			Vertex;
	using ankh::shapes::			Mesh;
	using ankh::surfaces::			TesselationParameters;

	TesselationParameters const defaulttp(9e-1f, false, ankh::surfaces::DefaultPrecision());
	TesselationParameters const& tp(_tp == NULL? defaulttp : *_tp);

	Surface const&			surf	(_::getsurf());

	size_t const			meshElementsMinCapacity((surf.GetResolutionI(tp) - 1) * 2 + 1 + 1),	// +1 security
							adjacenciesMinCapacity(meshElementsMinCapacity * 3);	// generally will by the number of mesh elements times 3

	// if they were vectors...
//	_::meshElements().reserve(meshElementsMinCapacity);
//	_::adjacencies().reserve(adjacenciesMinCapacity);

	{
		_::MeshElements			elements;

		{	timer t03("surface tesselation");
			ProduceAllFromAlongSections
		//	ProduceAllFromAcrossSections
			<t>(surf, tp, elements);
		}

		{	timer t03("compute and update AO for tesselated mesh");
			ankh::ao::InsertAmbientOcclusionFactors(elements);
		}

		{	timer t03("write AO factors to file");
			_::MeshElements::const_iterator end(elements.end()), i(elements.begin());
			std::ofstream fout("./AOS.txt");
			DASSERT(fout.good());
			for (; i != end; ++i)
				fout << i->GetAmbientOcclusion(0) << ", " << i->GetAmbientOcclusion(1) << ", " << i->GetAmbientOcclusion(2) << std::endl;
		}

		_::getmesh().Update(elements);
	}

	{
		timer t02("creating mesh, storing binary, and cleaning mesh up");
		_::getmesh().StoreBin("./surface_bin.msh");
		_::getmesh().StoreText("./surface_txt.msh");
	}

	_::printmeshinfo();
}

void load (char const* const id) {
	using namespace ankh::shapes;
	static const char mesh_bin_id[] = "surface_bin";
	static const size_t mesh_bin_id_length = sizeof(mesh_bin_id)/sizeof(mesh_bin_id[0]);

	std::string loadpath((
					strncmp(id, mesh_bin_id, mesh_bin_id_length) == 0?
						std::string(id) :
						std::string("../meshes/") + id
				) + ".msh");

	Mesh* m(MeshLoader::GetSingleton().Load(loadpath));

	DASSERT(m);
	_::getmesh() =(*DPTR(m));
	MeshLoader::GetSingleton().Unload(m);
	_::printmeshinfo();
}

void addastrianglesto (my::gl::shapes::ShapeCompositionFactory&	f)
{
	using my::gl::shapes::Colour;
	using my::gl::math::Vector4;

	Colour const			colour	(Vector4::New(0.8f, 0.4f, 0.8f));
	timer t02("transfoming as triagnles to my::gl::shapes triangles (and adding to factory)");
	_::MeshElements::const_iterator const end(_::meshElements().end());
	for (_::MeshElements::const_iterator i(_::meshElements().begin()); i != end; ++i)
		f.Add(maketriangle(*i, colour));
}

void addnormalsto (my::gl::shapes::ShapeCompositionFactory& f) {
	_::MeshElements::const_iterator const end(_::meshElements().end());
	for (_::MeshElements::const_iterator i(_::meshElements().begin()); i != end; ++i) {
		f.Add(makenormallineformeshelementvertex(*i, 0));
		f.Add(makenormallineformeshelementvertex(*i, 1));
		f.Add(makenormallineformeshelementvertex(*i, 2));
	}
}

void addcontrolpointsto (my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::vec4;
	using ankh::surfaces::Curve;
	using my::algo::map_vec4_to_points;
	using my::gl::shapes::Point;
	using ankh::surfaces::Curve;
	using ankh::surfaces::Surface;
	using ankh::surfaces::Knots;
	using ankh::surfaces::algo::CrossSection;

	Surface const&	surf(_::getsurf());
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

void addknotpointsto (my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::Vector4;
	using ankh::math::trig::vec4;
	using ankh::surfaces::Curve;
	using my::algo::map_vec4_to_points;
	using my::gl::shapes::Point;
	using ankh::surfaces::Curve;
	using ankh::surfaces::Surface;
	using ankh::surfaces::Knots;
	using ankh::surfaces::algo::CrossSection;
	using ankh::surfaces::tesselation::IneffectiveNormalCalculator;

	Surface const&	surf(_::getsurf());
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
			vectors.push_back(DE_BOOR_OR_NOT_DE_BOOR(At)(curve, i == last? i-1 : i, curve.GetKnot(i)));

		std::list<Point> points;
		f.AddAll(	map_vec4_to_points(
						vectors,
						points,
						Colour(Vector4::New(0.7f, 0.5f, 0.5f)),
						&makevector4));
	}
}

void addbasecurvesto (my::gl::shapes::ShapeCompositionFactory& f) {
	using namespace						ankh::surfaces;
	using namespace						my::gl::shapes;
	using ankh::math::trig::			vec4;
	using my::gl::math::				Vector4;
	using tesselation::deboor::			ProduceAll;
	using tesselation::					OptimisedInterpolatingTraits;
	using ankh::surfaces::algo::		PreciceControlPointInterpolation;
	using my::algo::					map_vec4_to_linestrip;
	using ankh::surfaces::tesselation::	IneffectiveNormalCalculator;
	using ankh::surfaces::				VertexWithNormal;
	using ankh::surfaces::				TesselationParameters;
	using ankh::surfaces::tesselation::	Resolution;

	typedef OptimisedInterpolatingTraits<&PreciceControlPointInterpolation> t;

	Surface const&						surf(_::getsurf());
	std::vector<VertexWithNormal>		points;
	std::list<Line>						lines;
	Colour const						colour(Vector4::New(0.4f, 0.2f, 0.8f));
	IneffectiveNormalCalculator const	inc;
	TesselationParameters const			tp(1e-1f);

	for (size_t i(0); i < surf.GetControlPointsWidth(); ++i) {
		Curve const&	base		(surf.GetAlongBase(i));
		size_t const	resolution	(Resolution(base, tp));

		points.clear();
		points.reserve(resolution);

		lines.clear();

		f.AddAll(map_vec4_to_linestrip(ProduceAll<t>(base, tp, inc, points), lines, colour, &makevector4fromvertexwithnormal));
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//   as dasd a s   //
}	// nurbs
//////////////////////////////////////////////////////////////////////////////////////


}} // my::drawing

