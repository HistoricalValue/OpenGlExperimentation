#include "ComputeMeshAmbientOcclusion.h"
#include "BuiltinShapesBoundingVolume.h"
#include "uinit.h"
#include "MeshLoader.h"

template <typename T>
struct dptr {
	T* ptr;

	T*		operator -> (void) const	{ return DPTR(DNULLCHECK(ptr)); }
	T&		operator * (void) const		{ return *operator ->(); }
	T*		operator ! (void)			{ udelete(ptr); return NULL; }
	T*		operator - (void)			{ unullify(ptr); return NULL; }
	T*		operator + (void)			{ return operator ->(); }

	dptr (T* const _ptr = NULL): ptr(_ptr) {}
	dptr (dptr const& p): ptr(p.ptr) {}
	~dptr (void) { DASSERT(!ptr); }

	void operator = (T* const _ptr) { DASSERT(!ptr); ptr = _ptr; }
};

using namespace ankh;
using shapes::Mesh;
using shapes::volumes::BoundingVolume;
using shapes::volumes::BuiltinShapes;
using shapes::MeshLoader;
using shapes::ComputeMeshAmbientOcclusionAlt;
using shapes::MeshAABBTree;

static void onerror (char const* const msg) { puts(msg); }

static void Initialise (void) {
	dinit(&onerror);
	UtilPackage::Initialise();
	MeshLoader::SingletonCreate();
	BoundingVolume::SingletonCreate();
	ao::AmbientOcclusionCreatorFactory::Initialise();
}
static void CleanUp (void) {
	ao::AmbientOcclusionCreatorFactory::CleanUp();
	BoundingVolume::SingletonDestroy();
	MeshLoader::SingletonDestroy();
	UtilPackage::CleanUp();
	dclose();
}

int main5 (int argc, char** argv) {
	// Init
	Initialise();

	// Declare
	ao::MeshIntersectionData				intersectionData;
	MeshAABBTree							aabb;
	dptr<Mesh::AmbientOcclusionCreator>		aocTrad;
	dptr<Mesh>								mesh;

	// Prepare
	{
		mesh = MeshLoader::GetSingleton().Load("moon_valley_2.0.msh");
		DASSERT(mesh->GetBoundingVolume());

		aocTrad = ao::AmbientOcclusionCreatorFactory::New(ao::SamplingRate_9, &mesh->GetElements(), &intersectionData);

		aabb(*mesh);
	}

	// AO testing
	{
		mesh->SetAmbientOcclusionCreator(+aocTrad);
		mesh->SelectiveUpdate(false, false, true);

		ComputeMeshAmbientOcclusionAlt aocAlt(&intersectionData, ComputeMeshAmbientOcclusionAlt::Sampling9, &aabb);
		mesh->SetAmbientOcclusionCreator(&aocAlt);

		// Compute AO with AABB AO creator, while cross-checking with previous results
		mesh->SelectiveUpdate(false, false, true);
	}

	// Clean Up
	{
		ao::AmbientOcclusionCreatorFactory::Delete(+aocTrad); -aocTrad;
		MeshLoader::GetSingleton().Unload(+mesh); -mesh;
		CleanUp();
	}

	return 0;
}
