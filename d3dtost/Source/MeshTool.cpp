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
#	include <fstream>
#pragma warning( pop )
#include <drawing_nurbs.h>
#include <drawing_utils.h>

#define FORCE_REAL_TESSELATION	1
#define FAST_TESSELATION		1

#if (defined(FORCE_REAL_TESSELATION) && FORCE_REAL_TESSELATION == 1) || !defined(_DEBUG)
#	define WITH_FAKE_TESSELATION 0
#else
#	define WITH_FAKE_TESSELATION 1
#endif

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

//
static const char GenerationStatsPath[] = "../meshes/generation_stats.txt";

///////////////////////////////////////////////////////////

static my::Console&	out (void);

static void					IdForStep (char (*buf)[1024], char const* base, float step);
static std::list<Unit>&		ProduceStepsInto (std::list<Unit>& steps);

static std::string&			BasePathForId (std::string& path, char const* id);
static std::string&			BinPathForId (std::string& path, char const* id);
static std::string&			TextPathForId (std::string& path, char const* id);

static void					WriteAllMeshInfo (std::list<MeshWithInfo> const&);

static void					Initialise (void);
static void					CleanUp (void);

struct TimingNotifee: public MeshStats::TimeUpdateNotifee {
	virtual ~TimingNotifee (void);

	virtual void	TimingStarted (char const* what) const;
	virtual void	TimingEnded (char const* what, MeshStats::timing_t howMuch) const;
};

///////////////////////////////////////////////////////////

namespace my {

static void DebugAwareTesselation (Mesh::Elements&, Surface const&, TesselationParameters const&);

static void ProduceMeshFromMeshProductionRequirementsInto (
				std::list<MeshWithInfo>&	into,
				MeshStats&					mt,
				Mesh::Elements const&		elements,
				dptr<BoundingVolume>		boundingVolume,
				char const*					meshStemId,
				Unit						step,
				bool						one);
static void	ProduceMeshesInto (
				std::list<MeshWithInfo>&	into,
				std::list<Unit> const&		steps,
				MeshStats&					mt);
static void LoadMeshesInto (
				std::list<MeshWithInfo>&	into);

void MeshProcess (void) {
	::Initialise();
	{
		MeshStats				timing;
		TimingNotifee			notifee;
		std::list<Unit>			steps;
		std::list<MeshWithInfo>	meshes;

		timing.SetNotifee(&notifee);

		#if 1
		ProduceMeshesInto(meshes, ProduceStepsInto(steps), timing);
		#else
		LoadMeshesInto(meshes);
		#endif

		MeshIndex& Index(MeshIndex::GetSingleton());
		Index.ImportAllFromMeshLoader();
		Index.Store();

		IFOREACH(std::list<MeshWithInfo>::iterator, meshes, meshWithInfo)
			meshWithInfo->mesh.Delete();
	}
	::CleanUp();
}

// static
void ProduceMeshesInto (std::list<MeshWithInfo>& into, std::list<Unit> const& steps, MeshStats& mt) {
	typedef dptr<Mesh>					MeshPtr;

	Surface const						bob(Surfaces::MoonValleyWithHorns(0.8f));
	Mesh::Elements						elements, elementsAntisavidised;
	dptr<BoundingVolume>				volume;


	IFOREACH (std::list<Unit>::const_iterator, steps, step) {
		out() << "\n\n=== producing meshes for step = " << *step << " ===\n";
		mt.Reset();
		elements.clear();
		elementsAntisavidised.clear();

		MeshAABBTree aabb;

		MESH_TIME(mt, Tesselation,			DebugAwareTesselation(elements, bob, TesselationParameters(*step))	);
		MESH_TIME(mt, BarycentricFactors,	ComputeBarycentricFactors(elements)									);
		MESH_TIME(mt, BoundingVolume,		volume = BuiltinShapes::Triangles(elements)							);
		MESH_TIME(mt, Savidise,				InvertNormalsAndWinding(elementsAntisavidised, elements)			);
		MESH_TIME(mt, Aabb,					aabb(elements, volume.native())										);

		PASSERT(elements.size() == elementsAntisavidised.size())

		ProduceMeshFromMeshProductionRequirementsInto(
			into,
			mt,
			elements, 
			tmpdptr<BoundingVolume>(volume->Clone()),
			ucstringarg(std::string(format("%s_aabbao", ucstringarg(bob.GetName())))),
			*step,
			true); 
		ProduceMeshFromMeshProductionRequirementsInto(
			into,
			mt,
			elementsAntisavidised, 
			volume,
			ucstringarg(std::string(format("%s_aabbao_antisavidised", ucstringarg(bob.GetName())))),
			*step,
			false); 

		volume.nullify();
	}
}

// static
void ProduceMeshFromMeshProductionRequirementsInto (
		std::list<MeshWithInfo>&	into,
		MeshStats&					mt,
		Mesh::Elements const&		elements,
		dptr<BoundingVolume>		boundingVolume,
		char const* const			meshStemId,
		Unit const					step,
		bool const					one) {
	dptr<Mesh>				mesh;
	MeshStats::Timing const	Update		(one? MeshStats::Update1		: MeshStats::Update2		),
							IndexBuffer	(one? MeshStats::IndexBuffer1	: MeshStats::IndexBuffer2	),
							StoreBin	(one? MeshStats::StoreBin1		: MeshStats::StoreBin2		),
							StoreText	(one? MeshStats::StoreText1		: MeshStats::StoreText2		);
	{
		char meshBaseId[1024];
		IdForStep(&meshBaseId, ucstringarg(meshStemId), step);

		ao::MeshIntersectionData intersectionData;
		ao::AmbientOcclusionCreatorProxy aoc(ao::SamplingRate_9, &elements, &intersectionData);

		MESH_TIME_VAR(mt, Update, mesh = DNEWCLASS(Mesh, (elements, meshBaseId, NULL, NULL, &aoc, boundingVolume.discard())));
	}

	MESH_TIME_VAR(mt, IndexBuffer, mesh->GetIndexBuffer());

	{	
		std::string storePath;

		BinPathForId(storePath, ucstringarg(mesh->GetUniqueId()));
		MESH_TIME_VAR(mt, StoreBin, mesh->StoreBin(storePath));
		MeshLoader::GetSingleton().GivePath(mesh.native(), storePath);

		TextPathForId(storePath, ucstringarg(mesh->GetUniqueId()));
		MESH_TIME_VAR(mt, StoreText, mesh->StoreText(storePath));
	}


	{
		MeshWithInfo info(mesh, step, mt);

		into.push_back(info);

		info.mesh.nullify();
	}

	mesh.nullify();

	WriteAllMeshInfo(into);
}

// static
void LoadMeshesInto (std::list<MeshWithInfo >& into) {
	MeshIndex& Index(MeshIndex::GetSingleton());

	out() << " - Loading mesh index...";
	Index.Load();
	out() << "\n - Loading all meshes with MeshLoader...";
	Index.ExportAllToMeshLoader();

	MeshLoader& Loader(MeshLoader::GetSingleton());
	IFOREACH(MeshLoader::Meshes::const_iterator, Loader.GetAll(), meshPair) {
		dptr<Mesh> mesh(meshPair->second);
		PASSERT(meshPair->first == mesh->GetUniqueId());

		out() << "\n - Loaded " << mesh->GetUniqueId() << " from " << Loader.GetPath(mesh.native());

		{
			MeshWithInfo info(mesh, -1.0f, MeshStats());
			into.push_back(info);
			info.mesh.nullify();
		}

		mesh.nullify();
	}

	out() << "\n";
}

// static
void DebugAwareTesselation (Mesh::Elements& elements, Surface const& bob, TesselationParameters const& tp) {
#if WITH_FAKE_TESSELATION == 1
	(void) bob;
	(void) tp;

	elements.push_back(MeshElement(Triangle(Vertex(0, 1, 2), Vertex(3, 4, 5), Vertex(6, 7, 8))));
	elements.back().MakeNormals();
	elements.back().SetNormal(0, vec3(1, 0, 0));
	elements.back().SetNormal(1, vec3(0, 1, 0));
	elements.back().SetNormal(2, vec3(0, 0, 1));
#else
	GenerateSurfaceMesh(elements, bob, tp);
#endif
}

} // my

///////////////////////////////////////////////////////////

// static
void IdForStep (char (* const buf)[1024], char const* const base, float const step)
	{ format((*buf), "%s_%3.1f", base, step); }

// static
std::list<Unit>& ProduceStepsInto (std::list<Unit>& into) {
#if WITH_FAKE_TESSELATION == 0 && (defined(_DEBUG) || FAST_TESSELATION == 1)
	Unit const	steps[] = {2e-0f};
#else
	Unit const	steps[] = {2e-0f, 1e-0f, 5e-1f, 4e-1f, 3e-1f, 2e-1f};//, 1e-1f};
#endif

	into.clear();

	FOREACH(const Unit, steps, step)
		into.push_back(*step);

	return into;
}

///////////////////////////////////////////////////////////

// static
void WriteAllMeshInfo (std::list<MeshWithInfo> const& meshesWithInfos) {
	std::ofstream fout("../meshes/generation_stats.txt", std::ios::out | std::ios::trunc | std::ios::binary);
	PASSERT(fout.good())

	IFOREACH(std::list<MeshWithInfo>::const_iterator, meshesWithInfos, meshWithInfo)
		WriteToStream(fout, meshWithInfo->info);
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
