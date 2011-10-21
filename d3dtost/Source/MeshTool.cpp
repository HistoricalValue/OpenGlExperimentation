#include <MeshTools.h>
#include "TheCursed.h"
#include "NurbsFacade.h"
#include "AmbientOcclusion.h"
#pragma warning( push, 0 )
#	include <Mesh.h>
#	include <MeshLoader.h>
#	include <BuiltinShapesBoundingVolume.h>
#	include <ComputeMeshAmbientOcclusion.h>
#	include <cstdio>
#	include "uinit.h"
#	include "ufiles.h"
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

struct TimingNotifee: public MeshTimingStats::TimeUpdateNotifee {
	virtual ~TimingNotifee (void);

	virtual void	TimingStarted (char const* what) const;
	virtual void	TimingEnded (char const* what, MeshTimingStats::timing_t howMuch) const;
};

namespace my {

void MeshProcess (void) {
	::Initialise();
	{
		std::list<std::string>	lines;
	#ifdef _DEBUG
		Unit const				steps[] = {2e-0f, 1e-0f};
	#else
		Unit const				steps[] = {2e-0f, 1e-0f, 5e-1f}; //, 4e-1f, 3e-1f, 2e-1f, 1e-1f};
	#endif
		Surface const			bob(Surfaces::MoonValleyWithHorns());
		MeshTimingStats			timing;
		TimingNotifee			notifee;

		timing.notifee = &notifee;

		FOREACH(const Unit, steps, step) {
			timing.Reset();
			out() << "\n\n ===== step = " << *step << " ===== \n";

		#if 1
			Mesh* mesh(NULL);
			Tesselate(mesh, bob, *step, timing);
			Store(*DPTR(mesh), timing);		
		#else
			char idForStep[1024];
			IdForStep(&idForStep, bob.GetName(), *step);
			std::string loadpath;
			BinPathForId(loadpath, ucstringarg(std::string("$") + idForStep));
			Mesh* mesh = MeshLoader::GetSingleton().Load(loadpath);
			DASSERT(mesh);
		#endif

			Mesh* savidised(NULL);
			Savidise(savidised, *DPTR(mesh), timing);
			Store(*DPTR(savidised), timing);

			out() << "\nwriting stats ...";
			lines.push_back(format(" === Stats for tesselation with step %3.1f === ", *step));
			lines.push_back(format(" number of triangles: %ld", DPTR(mesh)->GetElements().size()));
			WriteText(lines, timing);
			out() << "\n";

			udelete(savidised);
			udelete(mesh);
		}

		{
			out() << "\nflushing stats' lines ...";
			FILE* const fout(ubinaryfileopen("../meshes/generation_stats.txt", "w"));
			FlushTo(fout, lines);
			fclose(fout);
			out() << "\n";
		}
	}
	::CleanUp();
}

} // my

void Tesselate (Mesh*& mesh, Surface const& surf, Unit const step, MeshTimingStats& mt) {
	char id[1024];
	IdForStep(&id, surf.GetName(), step);

	ao::MeshIntersectionData	intersectionData;
	Mesh::Elements				elements;
	MESH_TIME(mt, tesselation, GenerateSurfaceMesh(elements, surf, TesselationParameters(step)));
	MESH_TIME(mt, barycentricFactors, ComputeBarycentricFactors(elements));

	{
		MESH_TIME(mt, boundingVolume, BoundingVolume* const volume(BuiltinShapes::Triangles(elements)));

	//	MeshAABBTree aabb;
	//	MESH_TIME(mt, aabb, aabb(elements, volume));

	//	ComputeMeshAmbientOcclussion aoc(ComputeMeshAmbientOcclussion::Samples9, aabb, FLT_MAX);

		ao::AmbientOcclusionCreatorProxy aoc(ao::SamplingRate_9, &elements, &intersectionData);

		DASSERT(!mesh);
		MESH_TIME(mt, update, mesh = DNEWCLASS(Mesh, (elements, id, NULL, NULL, &aoc, volume)));
	}

	MESH_TIME(mt, indexBuffer, DPTR(mesh)->GetIndexBuffer()); // generate

	std::string path;
	BinPathForId(path, ucstringarg(DPTR(mesh)->GetUniqueId()));
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
	Mesh::Elements						inversed;
	std::string							savidised_id(std::string("savidised_") + m.GetUniqueId());
	MeshTimingStats::timing_t			t_start, t_end;
	ao::MeshIntersectionData			intersectionData;

	InvertNormalsAndWinding(inversed, m.GetElements());

	ao::AmbientOcclusionCreatorProxy	aoc(ao::SamplingRate_9, &inversed, &intersectionData);

	DASSERT(!savidised);
	MESH_TIME(mt, savidise,
	savidised = DNEWCLASS(Mesh, (	inversed,
									savidised_id,
									NULL,
									NULL,
									&aoc,
									DPTR(DNULLCHECK(m.GetBoundingVolume()))->Clone()))
		);

	t_start = ugettime();
	DPTR(savidised)->GetIndexBuffer();	// generate
	t_end = ugettime();
	mt.AddCustom("(savidised) indexBuffer", t_end-t_start);

	std::string path;
	BinPathForId(path, ucstringarg(DPTR(savidised)->GetUniqueId()));
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
	ao::AmbientOcclusionCreatorFactory::Initialise();
}
void CleanUp (void) {
	ao::AmbientOcclusionCreatorFactory::CleanUp();
	MeshLoader::SingletonDestroy();
	BoundingVolume::SingletonDestroy();
}
my::Console& out (void) { return my::global::GetConsole(); }

TimingNotifee::~TimingNotifee (void) {}
void TimingNotifee::TimingStarted (char const* const what) const { out() << what << " ... "; }
void TimingNotifee::TimingEnded (char const* const, MeshTimingStats::timing_t const howMuch) const { out() << howMuch << "ms\n"; }
