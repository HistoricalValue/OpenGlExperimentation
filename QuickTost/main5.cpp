#pragma warning( push, 0 )
#	include "BuiltinShapesBoundingVolume.h"
#	include "uinit.h"
#	include "MeshLoader.h"
#	include <stdio.h>
#pragma warning( pop )

#include "ComputeMeshAmbientOcclusion.h"

#define ECHO(LINE)	printf("%132s ... ", #LINE); LINE puts("OK");

using namespace ankh;
using shapes::Mesh;
using shapes::volumes::BoundingVolume;
using shapes::volumes::BuiltinShapes;
using shapes::MeshLoader;
using shapes::ComputeMeshAmbientOcclusion;
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
	ECHO(	Initialise();																															)

			// Action
	ECHO(	{																																		)
				// Declare
	ECHO(		ao::MeshIntersectionData	intersectionData;																						)
	ECHO(		MeshAABBTree				aabb;																									)
	ECHO(		dptr<Mesh>					mesh;																									)

				// Load mesh and recompute barycentric factors
	ECHO(		mesh = MeshLoader::GetSingleton().Load("moon_valley_2.0.msh");																		)
	ECHO(		DASSERT(mesh->GetBoundingVolume());																									)
	ECHO(		mesh->ComputeBarycentricFactors();																									)
	ECHO(		aabb(*mesh);																														)

				// Calculate AO with the traditional method, with MeshIntersectionData as a by-product
	ECHO(		mesh->SetAmbientOcclusionCreator(&ao::AmbientOcclusionCreatorProxy(ao::SamplingRate_9, &mesh->GetElements(), &intersectionData));	)
	ECHO(		mesh->SelectiveUpdate(false, false, true);																							)

				// Calculate AO with the new method, cross-checking results with the given intersection data
	ECHO(		mesh->SetAmbientOcclusionCreator(&ComputeMeshAmbientOcclusionAlt(&intersectionData, ComputeMeshAmbientOcclusion::Sampling9, &aabb));)
	ECHO(		mesh->SelectiveUpdate(false, false, true);																							)

	ECHO(		mesh.Delete(ubind1st(umemberfunctionpointer_ref(&MeshLoader::Unload), MeshLoader::GetSingleton()));									)
	ECHO(	}																																		)

			// Clean Up
	ECHO(	CleanUp();																																)

	return 0;
}
