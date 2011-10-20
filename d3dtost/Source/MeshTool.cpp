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
using namespace ankh::shapes;
using namespace math;
using namespace trig;
using namespace types;
using namespace nurbs;
using namespace my::drawing::nurbs;
using namespace volumes;
//
using namespace ankh_x;
using namespace ankh_x::shapes;

static my::Console&	out (void);
static void			IdForStep (char (*buf)[1024], char const* base, float step);
static std::string&	BasePathForId (std::string& path, char const* id);
static std::string&	BinPathForId (std::string& path, char const* id);
static std::string&	TextPathForId (std::string& path, char const* id);
static void			Savidise (Mesh*& savidised, Mesh const& m, MeshTimingStats&);
static void			Tesselate (Mesh*& mesh, Surface const& surf, Unit const step, MeshTimingStats&);
static void			Store (Mesh const& mesh, MeshTimingStats&);

static void			Initialise (void);
static void			CleanUp (void);

namespace my {

void MeshProcess (void) {
	Initialise();
	{
		std::list<std::string>	lines;
		Unit const				steps[] = {2e-0f, 1e-0f};
		Surface const			bob(Surfaces::MoonValleyWithHorns());
		MeshTimingStats			timing;

		FOREACH(const Unit, steps, step) {
			out() << "1 ...";
			Mesh* mesh(NULL);
			Tesselate(mesh, bob, *step, timing);
			Store(*DPTR(mesh), timing);

			out() << "2 ...";
			Mesh* savidised(NULL);
			Savidise(savidised, *DPTR(mesh), timing);
			Store(*DPTR(savidised), timing);


			out() << "done ...";
			udelete(savidised);
			udelete(mesh);

			out() << "writing stats ...";
			lines.push_back(uconstructstr(" === Stats for tesselation with step %f === ", *step));
			WriteText(lines, timing);
		}

		{
			FILE* const fout(ubinaryfileopen("../meshes/generation_stats.txt"));
			FlushTo(fout, lines);
			fclose(fout);
		}
	}
	CleanUp();
}

} // my

void Tesselate (Mesh*& mesh, Surface const& surf, Unit const step, MeshTimingStats& mt) {
	char id[1024];
	IdForStep(&id, surf.GetName(), step);

	Mesh::Elements elements;
	MESH_TIME(mt, tesselation, GenerateSurfaceMesh(elements, surf, TesselationParameters(step)));
	MESH_TIME(mt, barycentricFactors, ComputeBarycentricFactors(elements));

	{
		MESH_TIME(mt, boundingVolume, BoundingVolume* const volume(BuiltinShapes::Triangles(elements)));

		MeshAABBTree aabb;
		MESH_TIME(mt, aabb, aabb(elements, volume));

		ComputeMeshAmbientOcclussion aoc(ComputeMeshAmbientOcclussion::Samples9, aabb, FLT_MAX);

		DASSERT(!mesh);
		MESH_TIME(mt, update, mesh = DNEWCLASS(Mesh, (elements, id, NULL, NULL, &aoc, volume)));

		DDELETE(volume);
	}

	MESH_TIME(mt, indexBuffer, DPTR(mesh)->GetIndexBuffer()); // generate

	std::string path;
	BinPathForId(path, id);
	MeshLoader::GetSingleton().GivePath(DPTR(mesh), path);
}

void IdForStep (char (* const buf)[1024], char const* const base, float const step)
	{ _snprintf_s(&(*buf)[0], countof(*buf), countof(*buf) - 1u, "%s_%3.1f", base, step); }

std::string& BasePathForId (std::string& path, char const* const id) {
	path.reserve(11 + 4 + strlen(id) + 1);
	path =("../meshes/");
	path += id;
	return path;
}

std::string& BinPathForId (std::string& path, char const* const id) {
	BasePathForId(path, id);
	path += ".msh";
	return path;
}

std::string& TextPathForId (std::string& path, char const* const id) {
	BasePathForId(path, id);
	path += ".txt";
	return path;
}

void Savidise (Mesh*& savidised, Mesh const& m, MeshTimingStats& mt) {
	Mesh::Elements					inversed;
	std::string						savidised_id(std::string("savidised_") + m.GetUniqueId());
	MeshTimingStats::timing_t	t_start, t_end;

	DASSERT(!savidised);
	t_start = ugettime();
	savidised = DNEWCLASS(Mesh, (	InvertNormalsAndWinding(inversed, m.GetElements()),
									savidised_id,
									NULL,
									NULL,
									NULL,
									DPTR(DNULLCHECK(m.GetBoundingVolume()))->Clone()));
	t_end = ugettime();
	mt.AddCustom("Savidising", t_end-t_start);

	t_start = ugettime();
	DPTR(savidised)->GetIndexBuffer();	// generate
	t_end = ugettime();
	mt.AddCustom("(savidised) indexBuffer", t_end-t_start);

	std::string path;
	BinPathForId(path, ucstringarg(savidised_id));
	MeshLoader::GetSingleton().GivePath(DPTR(savidised), path);
}

void Store (Mesh const& mesh, MeshTimingStats& mt) {
	std::string const path(MeshLoader::GetSingleton().GetPath(&mesh));
	std::string binpath, textpath;

	BinPathForId(binpath, ucstringarg(mesh.GetUniqueId()));
	TextPathForId(textpath, ucstringarg(mesh.GetUniqueId()));

	DASSERT(path == binpath);

	MESH_TIME(mt, storeBin, mesh.StoreBin(path));
	MESH_TIME(mt, storeText, mesh.StoreText(textpath));
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
