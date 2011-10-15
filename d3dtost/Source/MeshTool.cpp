#include <stdafx.h>

#include "TheCursed.h"

#include <Mesh.h>
#include <MeshLoader.h>
#include <drawing_nurbs.h>
#include <drawing_utils.h>
#include <BuiltinShapesBoundingVolume.h>
#include <ComputeMeshAmbientOcclusion.h>

using namespace ankh;
using namespace shapes;
using namespace math;
using namespace trig;
using namespace types;
using namespace nurbs;
using namespace my::drawing::nurbs;
using namespace volumes;

static char const* const meshesnames[] = {
	"moon_vallye_bumps_prec2e1_triangles480_ao9_aotime397",
	"moon_vallye_bumps_prec1e1_triangles442_ao9_aotime464",
	"moon_vallye_bumps_prec5e-1_triangles1768_ao9_aotime7052",
	"moon_vallye_bumps_prec4e-1_triangles3876_ao9_aotime33140",
	"moon_vallye_bumps_prec3e-1_triangles6939_ao9_aotime104954",
	"moon_vallye_bumps_prec2e-1_triangles12610_ao9_aotime343817"
};

#define FOREACH_MESHNAME(VARNAME)	\
	for (char const* const* VARNAME = &meshesnames[0]; VARNAME < &meshesnames[_countof(meshesnames)]; ++ VARNAME)

static void Savidise (void) {
	MeshLoader::SingletonCreate();

	FOREACH_MESHNAME(meshname) {
		std::string const path		(std::string("../meshes/") + *meshname + ".msh");
		std::string const outpath	(std::string("../meshes/") + "savidised_" + *meshname + ".msh");

		my::global::GetConsole() << "Savidising " << *meshname << " . . . ";

		my::global::GetConsole() << "loading . . . ";
		Mesh* const m = MeshLoader::GetSingleton().Load(path);
		DASSERT(m);

		my::global::GetConsole() << "savidising . . . ";
		{
			typedef Mesh::Elements::const_iterator ite_t;
			Mesh::Elements inverted;

			ite_t const end(m->GetElements().end());
			for (ite_t el(m->GetElements().begin()); el != end; ++el) {
				inverted.push_back(MeshElement(Triangle(el->a, el->c, el->b)));
				if (el->HasAmbientOcclusion()) {
					inverted.back().MakeAmbientOcclusion();
					inverted.back().SetAmbientOcclusion(0, el->GetAmbientOcclusion(0));
					inverted.back().SetAmbientOcclusion(1, el->GetAmbientOcclusion(1));
					inverted.back().SetAmbientOcclusion(2, el->GetAmbientOcclusion(2));
				}
				if (el->HasNormals()) {
					inverted.back().MakeNormals();
					inverted.back().SetNormal(0, -el->GetNormal(0));
					inverted.back().SetNormal(1, -el->GetNormal(1));
					inverted.back().SetNormal(2, -el->GetNormal(2));
				}
			}

			m->Update(inverted);
		}

		my::global::GetConsole() << "storing . . . ";
		m->StoreBin(outpath);

		my::global::GetConsole() << "unloading . . . ";
		MeshLoader::GetSingleton().Unload(m);

		my::global::GetConsole() << "done\n";
	}

	MeshLoader::SingletonDestroy();
}

static void IdForStep (char (*buf)[1024], char const* const base, float const step)
	{ _snprintf_s(&(*buf)[0], countof(*buf), countof(*buf) - 1u, "%s_%3.1f", base, step); }

struct MeshStats {
	TimesList			times;
	unsigned long int	numberOfMeshElements;
};

struct MeshesStats {
	typedef std::pair<Unit, MeshStats>	MeshStatsPerStep;
	std::list<MeshStatsPerStep>			stats;

	void		New (Unit const step) { stats.push_back(MeshStatsPerStep(step, MeshStats())); }
	TimesList*	GetTimesList (void) { return &stats.back().second.times; }
	void		SetNumEls (size_t const els) { stats.back().second.numberOfMeshElements = els; }
};

static void Tesselate (void) {
	Unit steps[] = { 2e0f, 1e0f, 5e-1f }; //, 4e-1f, 3e-1f, 2e-1f };
	MeshesStats	allstats;
	Surface bob(BobRoss());

	Initialise();

	FOREACH(Unit, steps, step) {
		allstats.New(*step);

		SetTimesList(allstats.GetTimesList());

		tesselate(bob, &TesselationParameters(*step, false, DefaultPrecision()));
		generateindexedbuffer();
		computeboundinvolume();
		MeshAABBTree aabb;
		generateaabb(aabb);
		ComputeMeshAmbientOcclusion aoc(ComputeMeshAmbientOcclusion::Sampling9, &aabb);
		updateao(aoc);
		
		allstats.SetNumEls(GetNumberOfMeshElements());

		{	char buf[1024];
			IdForStep(&buf, "moon_valley", *step);
			store(&buf[0]);
		}
	}

	CleanUp();
}

namespace my {

void MeshProcess (void) {
//	Savidise();
	Tesselate();
}

} // my
