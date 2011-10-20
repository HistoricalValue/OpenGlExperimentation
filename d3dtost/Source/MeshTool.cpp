#include <MeshTools.h>
#include "TheCursed.h"
#include "NurbsFacade.h"
#pragma warning( push, 0 )
#	include <Mesh.h>
#	include <MeshLoader.h>
#	include <BuiltinShapesBoundingVolume.h>
#	include <ComputeMeshAmbientOcclusion.h>
#	include <cstdio>
#	include "uinit.h"
#pragma warning( pop )
#include <drawing_nurbs.h>
#include <drawing_utils.h>

using namespace ankh;
using namespace shapes;
using namespace math;
using namespace trig;
using namespace types;
using namespace nurbs;
using namespace my::drawing::nurbs;
using namespace volumes;

static my::Console&	out (void);
static void			IdForStep (char (*buf)[1024], char const* base, float step);
static void			PathForId (std::string& path, char const* id);
static void			Savidise (Mesh*& savidised, Mesh const& m);
static void			Tesselate (Mesh*& mesh, Surface const& surf, Unit const step);
static void			Store (Mesh const& mesh);

static void			Initialise (void);
static void			CleanUp (void);
namespace my {

void MeshProcess (void) {
	Initialise();
	{
		Unit steps[] = {2e-0f, 1e-0f};
		Surface const bob(Surfaces::MoonValleyWithHorns());

		FOREACH(Unit, steps, step) {
			out() << "1 ...";
			Mesh* mesh(NULL);
			Tesselate(mesh, bob, *step);
			Store(*DPTR(mesh));

			out() << "2 ...";
			Mesh* savidised(NULL);
			Savidise(savidised, *DPTR(mesh));
			Store(*DPTR(savidised));

			out() << "done ...";
			udelete(savidised);
			udelete(mesh);
		}
	}
	CleanUp();
}

} // my

void Tesselate (Mesh*& mesh, Surface const& surf, Unit const step) {
	char id[1024];
	IdForStep(&id, surf.GetName(), step);

	Mesh::Elements elements;
	GenerateSurfaceMesh(elements, surf, TesselationParameters(step));
	x::ComputeBarycentricFactors(elements);
	
	{
		BoundingVolume* const volume(BuiltinShapes::Triangles(elements));

		MeshAABBTree aabb;
		aabb(elements, volume);

		ComputeMeshAmbientOcclussion aoc(ComputeMeshAmbientOcclussion::Samples9, aabb, FLT_MAX);

		DASSERT(!mesh);
		mesh = DNEWCLASS(Mesh, (elements, id, NULL, NULL, &aoc, volume));

		DDELETE(volume);
	}

	DPTR(mesh)->GetIndexBuffer(); // generate

	std::string path;
	PathForId(path, id);
	MeshLoader::GetSingleton().GivePath(DPTR(mesh), path);
}

void IdForStep (char (* const buf)[1024], char const* const base, float const step)
	{ _snprintf_s(&(*buf)[0], countof(*buf), countof(*buf) - 1u, "%s_%3.1f", base, step); }

void PathForId (std::string& path, char const* const id) {
	path.reserve(11 + 4 + strlen(id) + 1);
	path =("../meshes/");
	path += id;
	path += ".msh";
}

void Savidise (Mesh*& savidised, Mesh const& m) {
	Mesh::Elements inversed;
	std::string savidised_id(std::string("savidised_") + m.GetUniqueId());

	DASSERT(!savidised);
	savidised = DNEWCLASS(Mesh, (	x::InvertNormalsAndWinding(inversed, m.GetElements()),
									savidised_id,
									NULL,
									NULL,
									NULL,
									DPTR(DNULLCHECK(m.GetBoundingVolume()))->Clone()));
	DPTR(savidised)->GetIndexBuffer();	// generate

	std::string path;
	PathForId(path, ucstringarg(savidised_id));
	MeshLoader::GetSingleton().GivePath(DPTR(savidised), path);
}

void Store (Mesh const& mesh) {
	std::string const path(MeshLoader::GetSingleton().GetPath(&mesh));
	mesh.StoreBin(path);
}

static inline void onerror (char const* const msg) { out() << msg; }
void Initialise (void) {
	BoundingVolume::SingletonCreate();
	MeshLoader::SingletonCreate();
}
void CleanUp (void) {
	MeshLoader::SingletonDestroy();
	BoundingVolume::SingletonDestroy();
}
my::Console& out (void) { return my::global::GetConsole(); }
