#define NURBS__STATIC_LIB
#define IMAGE_LOADER__STATIC_LIB
#include "../../NURBS/_trash/AmbientOcclusion.h"
#include "NurbsFacade.h"
#include "BuiltinShapesBoundingVolume.h"
#include "uinit.h"
#include "MeshLoader.h"
#include "ComputeMeshAmbientOcclusion.h"

using namespace ankh;
using nurbs::Unit;
using nurbs::Surface;
using nurbs::Curve;
using nurbs::Knots;
using nurbs::ControlPointsGrid;
using nurbs::FillUniformly;
using nurbs::VaryGrid;
using nurbs::FillGridUniformly;
using shapes::Mesh;
using shapes::volumes::BoundingVolume;
using shapes::volumes::BuiltinShapes;
using shapes::MeshLoader;
using shapes::ComputeMeshAmbientOcclusionAlt;
using shapes::MeshAABBTree;

template <const bool inv, typename T> struct inverser;
template <typename T> struct inverser<false, T>
	{ T const& a, b; inverser (T const& _a, T const& _b): a(_a), b(_b) {} };
template <typename T> struct inverser<true, T>
	{ T const& a, b; inverser (T const& _a, T const& _b): a(_b), b(_a) {} };
template <const bool inv, typename T> inverser<inv, T> const makeinverser (T const& a, T const& b)
	{ return inverser<inv,T>(a, b); }

static Surface MakeSurface (void) {
	const Unit		variation		(0.00625f),
					minx			(-0.025f),
					maxx			(-minx),
					miny			(minx),
					maxy			(maxx),
					minz			(miny),
					maxz			(maxy);
	const size_t	width_units		(16u),
					height_units	(16u),
					depth_units		(16u),
					order_j			(0x04u),
					order_i			(0x05u),
					numcpoints_j	(0x10u),
					numcpoints_i	(0x15u),
					numknots_j		(Curve::NumberOfKnotsFor(order_j, numcpoints_j)),
					numknots_i		(Curve::NumberOfKnotsFor(order_i, numcpoints_i));
	const bool		inverseX		(false),
					inverseZ		(true);
	const long		seed			(230l);
	Knots							knots_j, knots_i;
	ControlPointsGrid				cpoints_i;

	FillUniformly(knots_j, numknots_j, 0.0f, 1.0f);
	FillUniformly(knots_i, numknots_i, 0.0f, 1.0f);
		
	VaryGrid(
		FillGridUniformly(
			cpoints_i,
			numcpoints_i,
			numcpoints_j,
			makeinverser<inverseX>(minx, maxx).a, makeinverser<inverseX>(minx, maxx).b,
			makeinverser<inverseZ>(minz, maxz).a, makeinverser<inverseZ>(minz, maxz).b,
			(miny + maxy)/2.0f,
			ankh::nurbs::Precision(5e-9f)),
		variation,
		variation,
		variation,
		1.5f,
		false,
		seed)
	;
	cpoints_i.at(3).at(3) = math::trig::vec4(maxx, maxy, maxz, 1.0f);
	cpoints_i.at(3).at(3) *= 2.0f;
	
	cpoints_i.at(9).at(3).y = -maxy;
	cpoints_i.at(9).at(3) *= 4.0f;
	
	cpoints_i.at(15).at(5).y = maxy;
	cpoints_i.at(15).at(5) *= 0.5f;

	return	Surface(
				knots_j.begin(),
				knots_j.end(),
				knots_i.begin(),
				knots_i.end(),
				cpoints_i.begin(),
				cpoints_i.end(),
				"BOB ROSS");
}

static void onerror (char const* const msg) { puts(msg); }

static void Initialise (void) {
	dinit(&onerror);
	UtilPackage::Initialise();
	MeshLoader::SingletonCreate();
	ao::AmbientOcclusionCreatorFactory::Initialise();
}
static void CleanUp (void) {
	ao::AmbientOcclusionCreatorFactory::CleanUp();
	MeshLoader::SingletonDestroy();
	UtilPackage::CleanUp();
	dclose();
}

int main5 (int argc, char** argv) {
	// Init
	Initialise();

	// Declare
	Mesh::Elements						elements;
	ao::ProduceIntersectionData_Into	intersectionData;
	BoundingVolume*						volume				(NULL);
	Mesh::AmbientOcclusionCreator*		aocTrad 			(NULL);

	// Prepare
	{
		nurbs::GenerateSurfaceMesh(elements, MakeSurface(), nurbs::TesselationParameters(2e0f, true, nurbs::Precision(5e-9f)));
		volume	= BuiltinShapes::Triangles(elements);
		aocTrad	= ao::AmbientOcclusionCreatorFactory::New(ao::SamplingRate_9, &elements, &intersectionData);
	}

	// AO testing
	{
		Mesh m(elements, "", NULL, NULL, aocTrad, volume);
		
		MeshAABBTree aabb;
		aabb(m);

		ComputeMeshAmbientOcclusionAlt aocAlt(&intersectionData, ComputeMeshAmbientOcclusionAlt::Sampling9, &aabb);
		m.ResetCreators();
		m.SetAmbientOcclusionCreator(&aocAlt);

		// Compute AO with AABB AO creator, while cross-checking with previous results
		m.SelectiveUpdate(false, false, true);
	}

	// Clean Up 
	{
		ao::AmbientOcclusionCreatorFactory::Delete(aocTrad);
		DDELETE(volume);
		CleanUp();
	}
	return 0;
}
