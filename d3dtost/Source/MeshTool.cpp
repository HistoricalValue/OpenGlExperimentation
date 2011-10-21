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
#	include "ustrings.h"
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

///////////////////////////////////////////////////////////

static my::Console&	out (void);

static void					IdForStep (char (*buf)[1024], char const* base, float step);
static std::list<Unit>&		ProduceStepsInto (std::list<Unit>& steps);

static std::string&			BasePathForId (std::string& path, char const* id);
static std::string&			BinPathForId (std::string& path, char const* id);
static std::string&			TextPathForId (std::string& path, char const* id);

static void					Initialise (void);
static void					CleanUp (void);

struct TimingNotifee: public MeshStats::TimeUpdateNotifee {
	virtual ~TimingNotifee (void);

	virtual void	TimingStarted (char const* what) const;
	virtual void	TimingEnded (char const* what, MeshStats::timing_t howMuch) const;
};

///////////////////////////////////////////////////////////

namespace my {


static void	ProduceMeshesInto (
				std::list<dptr<Mesh> >&		into,
				std::list<Unit> const&		steps,
				MeshStats&					mt,
				std::list<std::string>&		lines);


void MeshProcess (void) {
	::Initialise();
	{
		std::list<std::string>	lines;
		MeshStats				timing;
		TimingNotifee			notifee;
		std::list<Unit>			steps;
		std::list<dptr<Mesh> >	meshes;

		timing.notifee = &notifee;

		ProduceStepsInto(steps);
		ProduceMeshesInto(meshes, steps, timing, lines);

		{
			out() << "\nflushing stats' lines ...";
			FILE* const fout(ubinaryfileopen("../meshes/generation_stats.txt", "w"));
			FlushTo(fout, lines);
			fclose(fout);
			out() << "\n";
		}

		IFOREACH(std::list<dptr<Mesh> >::iterator, meshes, mesh)
			mesh->Delete();
	}
	::CleanUp();
}

// static
void ProduceMeshesInto (std::list<dptr<Mesh> >& into, std::list<Unit> const& steps, MeshStats& mt, std::list<std::string>& lines) {
	typedef dptr<Mesh>					MeshPtr;
	
	char								charbuf[1024];
	Surface const						bob(Surfaces::MoonValleyWithHorns());
	Mesh::Elements						elements, elementsAntisavidised;
	dptr<BoundingVolume>				volume;
	MeshPtr								mesh, meshAntisavidised;
	std::string							stepBaseId, stepBaseIdAntisavidised, storePath;
	ao::MeshIntersectionData			intersectionData;
	

	IFOREACH (std::list<Unit>::const_iterator, steps, step) {
		out() << "=== producing meshes for step = " << *step << " ===\n";
		mt.Reset();
		elements.clear();

		IdForStep(&charbuf, ucstringarg(bob.GetName()), *step);
		stepBaseId = &charbuf[0];
		IdForStep(&charbuf, format("%s_antisavidised", ucstringarg(bob.GetName())), *step);
		stepBaseIdAntisavidised = charbuf;

		MeshAABBTree aabb;

		#ifdef _DEBUG
		elements.push_back(MeshElement(Triangle(Vertex(0, 1, 2), Vertex(3, 4, 5), Vertex(6, 7, 8))));
		elements.back().MakeNormals();
		elements.back().SetNormal(0, vec3(1, 0, 0));
		elements.back().SetNormal(1, vec3(0, 1, 0));
		elements.back().SetNormal(2, vec3(0, 0, 1));
		#else
		MESH_TIME(mt, Tesselation,			GenerateSurfaceMesh(elements, bob, TesselationParameters(*step))	);
		#endif
		MESH_TIME(mt, BarycentricFactors,	ComputeBarycentricFactors(elements)									);
		MESH_TIME(mt, BoundingVolume,		volume = BuiltinShapes::Triangles(elements)							);
		MESH_TIME(mt, Savidise,				InvertNormalsAndWinding(elementsAntisavidised, elements)			);
		MESH_TIME(mt, Aabb,					aabb(elements,	volume.native())									);

		{
			intersectionData.clear();
			ao::AmbientOcclusionCreatorProxy aoc(ao::SamplingRate_9, &elements, &intersectionData);
			MESH_TIME(mt, Update1,
					mesh = DNEWCLASS(Mesh, (elements, stepBaseId, NULL, NULL, &aoc, volume->Clone())));
		}
		{
			intersectionData.clear();
			ao::AmbientOcclusionCreatorProxy aoc(ao::SamplingRate_9, &elementsAntisavidised, &intersectionData);
			MESH_TIME(mt, Update2,
					meshAntisavidised = DNEWCLASS(Mesh, (elementsAntisavidised, stepBaseIdAntisavidised, NULL, NULL, &aoc, volume.native())));
			volume.nullify();
		}

		MESH_TIME(mt, IndexBuffer1,	mesh->GetIndexBuffer()				);
		MESH_TIME(mt, IndexBuffer2, meshAntisavidised->GetIndexBuffer()	);

		{
			BinPathForId(storePath, ucstringarg(stepBaseId));
			MESH_TIME(mt, StoreBin1, mesh->StoreBin(storePath));
			MeshLoader::GetSingleton().GivePath(mesh.native(), storePath);

			TextPathForId(storePath, ucstringarg(stepBaseId));
			MESH_TIME(mt, StoreText1, mesh->StoreText(storePath));
		}

		{
			BinPathForId(storePath, ucstringarg(stepBaseIdAntisavidised));
			MESH_TIME(mt, StoreBin2, meshAntisavidised->StoreBin(storePath));
			MeshLoader::GetSingleton().GivePath(meshAntisavidised.native(), storePath);

			TextPathForId(storePath, ucstringarg(stepBaseIdAntisavidised));
			MESH_TIME(mt, StoreText2, meshAntisavidised->StoreText(storePath));
		}

		into.push_back(mesh);
		into.push_back(meshAntisavidised);

		mesh.nullify();
		meshAntisavidised.nullify();

		mt >> lines;
	}
}

} // my

///////////////////////////////////////////////////////////

// static
void IdForStep (char (* const buf)[1024], char const* const base, float const step)
	{ _snprintf_s(&(*buf)[0], countof(*buf), countof(*buf) - 1u, "%s_%3.1f", base, step); }


// static
std::list<Unit>& ProduceStepsInto (std::list<Unit>& into) {
#ifdef _DEBUG
	Unit const	steps[] = {2e-0f};//, 1e-0f};
#else			
	Unit const	steps[] = {2e-0f, 1e-0f, 5e-1f, 4e-1f, 3e-1f, 2e-1f, 1e-1f};
#endif

	into.clear();

	FOREACH(const Unit, steps, step)
		into.push_back(*step);

	return into;
}

///////////////////////////////////////////////////////////

// static
std::string& BasePathForId (std::string& path, char const* const id) {
	path.reserve(11 + 4 + strlen(id) + 1);
	path =("../meshes/");
	path += id;
	return path;
}

///////////////////////////////////////////////////////////

// static
std::string& BinPathForId (std::string& path, char const* const id) {
	BasePathForId(path, id);
	path += ".msh";
	return path;
}

///////////////////////////////////////////////////////////

// static
std::string& TextPathForId (std::string& path, char const* const id) {
	BasePathForId(path, id);
	path += ".txt";
	return path;
}

///////////////////////////////////////////////////////////

static inline void onerror (char const* const msg) { out() << msg; }
// static
void Initialise (void) {
	BoundingVolume::SingletonCreate();
	MeshLoader::SingletonCreate();
	ao::AmbientOcclusionCreatorFactory::Initialise();
	MeshIndex::SingletonCreate();
}
// static
void CleanUp (void) {
	MeshIndex::SingletonDestroy();
	ao::AmbientOcclusionCreatorFactory::CleanUp();
	MeshLoader::SingletonDestroy();
	BoundingVolume::SingletonDestroy();
}

///////////////////////////////////////////////////////////

// static
my::Console& out (void) { return my::global::GetConsole(); }

///////////////////////////////////////////////////////////

TimingNotifee::~TimingNotifee (void) {}
void TimingNotifee::TimingStarted (char const* const what) const { out() << what << " ... "; }
void TimingNotifee::TimingEnded (char const* const, MeshStats::timing_t const howMuch) const { out() << howMuch << "ms\n"; }

///////////////////////////////////////////////////////////
