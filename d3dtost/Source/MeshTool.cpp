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
#	include "MeshLoader.h"
#pragma warning( pop )
#include <drawing_nurbs.h>
#include <drawing_utils.h>

#define FORCE_REAL_TESSELATION	0
#define FAST_TESSELATION		0
#define NO_AO					1

#if FORCE_REAL_TESSELATION == 1 || !defined(_DEBUG) || NO_AO == 1
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

typedef cstring<1024>		Kilostring;

static std::list<Unit>&		ProduceStepsInto (std::list<Unit>& steps);
static Kilostring&			IdForStep (Kilostring&, char const* base, float step);
static Kilostring&			BasePathForId (Kilostring& path, char const* id);
static Kilostring&			BinPathForId (Kilostring& path, char const* id);
static Kilostring&			TextPathForId (Kilostring& path, char const* id);

static void					WriteAllMeshInfo (std::list<MeshWithInfo> const&);

static void					Initialise (void);
static void					CleanUp (void);

struct TimingNotifee: public MeshStats::TimeUpdateNotifee {
	virtual ~TimingNotifee (void);

	virtual void	TimingStarted (char const* what) const;
	virtual void	TimingEnded (char const* what, MeshStats::timing_t howMuch) const;
};

static ao::AnyAmbientOcclusionCreatorProxy
			MakeAmbientOcclusionCreator(
				ao::SamplingRate			samplingRate,
				Mesh::Elements const*		elements,
				ao::MeshIntersectionData*	intersectionData);

static void DebugAwareTesselation (Mesh::Elements&, Surface const&, TesselationParameters const&);

static void ProduceMeshFromMeshProductionRequirements (
				std::list<MeshWithInfo>&	into,
				MeshStats&					mt,
				Mesh::Elements const&		elements,
				dptr<BoundingVolume>		boundingVolume,
				char const*					meshId,
				char const*					meshPath,
				char const*					meshTextPath,
				Unit						step,
				bool						one);
static void	ProduceOrLoadMeshes (
				std::list<MeshWithInfo>&	into,
				std::list<Unit> const&		steps,
				MeshStats&					mt);
static void LoadMeshesInto (
				std::list<MeshWithInfo>&	into);

///////////////////////////////////////////////////////////

namespace my {

///////////////////////////////////////////////////////////

void MeshProcess (void) {
	::Initialise();
	{	
		std::list<MeshWithInfo>		into;
		MeshStats					timing;
		TimingNotifee				notifee;
		std::list<Unit>				steps;

		timing.SetNotifee(&notifee);

		#if 1
		ProduceOrLoadMeshes(into, ProduceStepsInto(steps), timing);
		#else
		LoadMeshesInto(meshes);
		#endif

		MeshIndex& Index(MeshIndex::GetSingleton());
		Index.ImportAllFromMeshLoader();
		Index.Store();

		{
			typedef std::map<std::string, Mesh*> Meshes;

			Meshes const&					all(MeshLoader::GetSingleton().GetAll());
			std::list<Mesh*>				toUnload, toDelete;
			Meshes::const_iterator const	meshes_end(all.end());

			for (Meshes::const_iterator meshIdWithMesh(all.begin()); meshIdWithMesh != meshes_end; ++meshIdWithMesh)
				if (MeshLoader::GetSingleton().IsLoaded(meshIdWithMesh->second))
					toUnload.push_back(meshIdWithMesh->second);
				else
					toDelete.push_back(meshIdWithMesh->second);

			foreach(toUnload, ubind1st(umemberfunctionpointer(&MeshLoader::Unload), MeshLoader::GetSingletonPtr()));
			foreach(toDelete, uptrdestructorfunctor<Mesh*>());
		}

		foreach(into, umemberfunctionpointer_ref(&MeshWithInfo::NullifyMeshPointer));
	}
	::CleanUp();
}

///////////////////////////////////////////////////////////

} // namespace my

///////////////////////////////////////////////////////////

// static
void ProduceOrLoadMeshes (std::list<MeshWithInfo>& into, std::list<Unit> const& steps, MeshStats& mt) {
	typedef dptr<Mesh>					MeshPtr;

	// reusables
	// mt
	Surface const						bob(Surfaces::MoonValleyWithHorns(0.8f));
	Mesh::Elements						elements, elementsAntisavidised;
	dptr<BoundingVolume>				volume;
	Kilostring							meshId, meshIdForStep, loadpath, textpath;

	IFOREACH (std::list<Unit>::const_iterator, steps, step) {	
		out() << "\n *** Meshes for step= " << *step << ": ";

		MeshAABBTree	aabb;
		bool			prerequisitesMade(false), one(false);

		for (size_t i(0u); i < 2u; ++i) {
			one = !one;

			BinPathForId(loadpath, IdForStep(meshIdForStep, meshId.format(one? "%s_aabbao" : "%s_aabbao_antisavidised", ucstringarg(bob.GetName())), *step));
			TextPathForId(textpath, meshIdForStep);

			out() << "\n       - " << meshId << ": " << loadpath << " ... ";

			if (MeshLoader::GetSingleton().Load(loadpath))
				out() << "loaded"; // ok
			else {
				out() << "producing ... ";

				if (!prerequisitesMade) {
					MESH_TIME(mt, Tesselation,			DebugAwareTesselation(elements, bob, TesselationParameters(*step))	);
					MESH_TIME(mt, BarycentricFactors,	ComputeBarycentricFactors(elements)									);
					MESH_TIME(mt, BoundingVolume,		volume = BuiltinShapes::Triangles(elements)							);
					MESH_TIME(mt, Savidise,				InvertNormalsAndWinding(elementsAntisavidised, elements)			);
					MESH_TIME(mt, Aabb,					aabb(elements, volume.native())										);

					prerequisitesMade = true;
				}

				PASSERT(elements.size() == elementsAntisavidised.size())

				ProduceMeshFromMeshProductionRequirements(
					into,
					mt,
					one? elements : elementsAntisavidised,
					one? tmpdptr<BoundingVolume>(volume->Clone()) : tmpdptr<BoundingVolume>(volume),
					meshIdForStep,
					loadpath,
					textpath,
					*step,
					one);
			}

			// reusables reset
			meshId.clear();
			meshIdForStep.clear();
			loadpath.clear();
		}

		// reusables reset
		mt.Reset();
		elements.clear();
		elementsAntisavidised.clear();
		volume.nullify();
	}

	out() << "\n";
}

// static
void ProduceMeshFromMeshProductionRequirements (
		std::list<MeshWithInfo>&	into,
		MeshStats&					mt,
		Mesh::Elements const&		elements,
		dptr<BoundingVolume>		boundingVolume,
		char const* const			meshId,
		char const* const			meshPath,
		char const* const			meshTextPath,
		Unit const					step,
		bool const					one) {
	dptr<Mesh>				mesh;
	MeshStats::Timing const	Update		(one? MeshStats::Update1		: MeshStats::Update2		),
							IndexBuffer	(one? MeshStats::IndexBuffer1	: MeshStats::IndexBuffer2	),
							StoreBin	(one? MeshStats::StoreBin1		: MeshStats::StoreBin2		),
							StoreText	(one? MeshStats::StoreText1		: MeshStats::StoreText2		);
	{
		ao::MeshIntersectionData intersectionData;

		MESH_TIME_VAR(
			mt,
			Update,
			mesh = DNEWCLASS(
				Mesh, (
					elements,
					meshId,
					NULL,
					NULL,
					uaddress_of(MakeAmbientOcclusionCreator(ao::SamplingRate_9, &elements, &intersectionData)), 
					boundingVolume.discard())));
	}

	MESH_TIME_VAR(mt, IndexBuffer, mesh->GetIndexBuffer());

	{
		MESH_TIME_VAR(mt, StoreBin, mesh->StoreBin(meshPath));
		MESH_TIME_VAR(mt, StoreText, mesh->StoreText(meshTextPath));

		MeshLoader::GetSingleton().GivePath(mesh.native(), meshPath);
	}

	{
		MeshWithInfo info(mesh, step, mt);

		into.push_back(info);

		info.NullifyMeshPointer();
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

// static
ao::AnyAmbientOcclusionCreatorProxy MakeAmbientOcclusionCreator(
		ao::SamplingRate const			samplingRate,
		Mesh::Elements const* const		elements,
		ao::MeshIntersectionData* const	intersectionData) {
#if NO_AO == 1
	USE(samplingRate), USE(elements), USE(intersectionData);
	return DNEW(ao::IneffectiveAmbientOcclusionCreator);
#else
	return DNEWCLASS(ao::AmbientOcclusionCreatorProxy, (samplingRate, elements, intersectionData));
#endif
}

///////////////////////////////////////////////////////////

// static
Kilostring& IdForStep (Kilostring& kilostring, char const* base, float step)
	{ return kilostring.format("%s_%3.1f", base, step); }

// static
std::list<Unit>& ProduceStepsInto (std::list<Unit>& into) {
#if WITH_FAKE_TESSELATION == 0 && (FAST_TESSELATION == 1 || (defined(_DEBUG) && NO_AO == 0))
	Unit const	steps[] = {2e-0f};
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
void WriteAllMeshInfo (std::list<MeshWithInfo> const& meshesWithInfos) {
	std::ofstream fout("../meshes/generation_stats.txt", std::ios::out | std::ios::trunc | std::ios::binary);
	PASSERT(fout.good())

	IFOREACH(std::list<MeshWithInfo>::const_iterator, meshesWithInfos, meshWithInfo)
		WriteToStream(fout, meshWithInfo->info);
}

///////////////////////////////////////////////////////////

// static
Kilostring& BasePathForId (Kilostring& path, char const* const id) {
	path = "../meshes/";
	return path += id;
}

///////////////////////////////////////////////////////////

// static
Kilostring& BinPathForId (Kilostring& path, char const* id)
	{ return BasePathForId(path, id) += ".msh"; }

///////////////////////////////////////////////////////////

// static
Kilostring& TextPathForId (Kilostring& path, char const* const id)
	{ return BasePathForId(path, id) += ".txt"; }

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
void TimingNotifee::TimingStarted (char const* const what) const { out() << "\n         - " << what << " ... "; }
void TimingNotifee::TimingEnded (char const* const, MeshStats::timing_t const howMuch) const { out() << howMuch << "ms"; }

///////////////////////////////////////////////////////////
