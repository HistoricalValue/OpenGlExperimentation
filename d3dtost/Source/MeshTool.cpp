#include <stdafx.h>

#include "TheCursed.h"

#include <Mesh.h>
#include <MeshLoader.h>
#include <drawing_nurbs.h>
#include <drawing_utils.h>
#include <BuiltinShapesBoundingVolume.h>
#include <ComputeMeshAmbientOcclusion.h>

#include <cstdio>

using namespace ankh;
using namespace shapes;
using namespace math;
using namespace trig;
using namespace types;
using namespace nurbs;
using namespace my::drawing::nurbs;
using namespace volumes;

static void Savidise (std::list<std::string> const& generatedIds) {
	BoundingVolume::SingletonCreate();
	MeshLoader::SingletonCreate();

	for (std::list<std::string>::const_iterator meshname(generatedIds.begin()); meshname != generatedIds.end(); ++meshname) {
		std::string const path		(std::string("../meshes/") + *meshname + ".msh");
		std::string const outpath	(std::string("../meshes/") + "savidised_" + *meshname + ".msh");

		my::global::GetConsole() << "Savidising " << meshname->c_str() << " . . . ";

		my::global::GetConsole() << "loading . . . ";
		Mesh m;
		loadinto(meshname->c_str(), m);

		my::global::GetConsole() << "savidising . . . ";
		{
			typedef Mesh::Elements::const_iterator ite_t;
			Mesh::Elements inverted;

			ite_t const end(m.GetElements().end());
			for (ite_t el(m.GetElements().begin()); el != end; ++el) {
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

			m.Update(inverted);
		}

		my::global::GetConsole() << "storing . . . ";
		m.StoreBin(outpath);

		my::global::GetConsole() << "done\n";
	}

	MeshLoader::SingletonDestroy();
	BoundingVolume::SingletonDestroy();
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
	typedef std::list<MeshStatsPerStep>::const_iterator	Iterator;

	void		New (Unit const step) { stats.push_back(MeshStatsPerStep(step, MeshStats())); }
	TimesList*	GetTimesList (void) { return &stats.back().second.times; }
	void		SetNumEls (size_t const els) { stats.back().second.numberOfMeshElements = els; }
};

static inline void WriteAllMeshesStats (MeshesStats const& allstats) {
	std::ofstream fout("../meshes/generation_stats.txt", std::ios::out | std::ios::trunc);
	DASSERT(!fout.bad());

	my::global::GetConsole() << "writing stats . . ." ;

	for (MeshesStats::Iterator i(allstats.stats.begin()); i != allstats.stats.end(); ++i) {
		fout << " === Tesselation with step " << i->first << " ===" << std::endl
			<< i->second.numberOfMeshElements << " number of elements" << std::endl;
		for (TimesList::const_iterator j(i->second.times.begin()); j != i->second.times.end(); ++j)
			fout << "\t" << j->first << ": " << j->second << " miliseconds" << std::endl;
		fout << std::endl;
	}

	my::global::GetConsole() << " done\n";
}

static void Tesselate (std::list<std::string>& generatedIds, bool const doWork) {
	Unit steps[] = { 2e-0f }; // { 2e0f, 1e0f, 5e-1f, 4e-1f, 3e-1f, 2e-1f };
	MeshesStats	allstats;
	Surface bob(BobRoss());

	Initialise();

	FOREACH(Unit, steps, step) {
		{	char buf[1024];
			IdForStep(&buf, "moon_valley", *step);
			generatedIds.push_back(&buf[0]);
		}
		if (doWork) {
			allstats.New(*step);

			SetTimesList(allstats.GetTimesList());

			tesselate(bob, &TesselationParameters(*step, false, DefaultPrecision()));
			generateindexedbuffer();
			computeboundinvolume();
		
			ao::ProduceIntersectionData_Into intersectionData;

			{
				fairprepareao();
				updateaotraditional(intersectionData);
			}
			{
				fairprepareao();
				MeshAABBTree aabb;
				generateaabb(aabb);

				struct { std::vector<float> trad, nu, alt; } aos;

				// Do with alt
				{	fairprepareao();
					ComputeMeshAmbientOcclusionAlt aoc(&intersectionData, ComputeMeshAmbientOcclusion::Sampling9, &aabb);
					updateao(aoc, "ALT");
					ExtractAOInto(aos.alt);	}

				// Do with nu
				{	fairprepareao();
					ComputeMeshAmbientOcclusion aoc(ComputeMeshAmbientOcclusion::Sampling9, &aabb);
					updateao(aoc, "NU");
					ExtractAOInto(aos.nu); }

				DASSERT(std::equal(aos.alt.begin(), aos.alt.end(), aos.nu.begin()));
			}

			allstats.SetNumEls(GetNumberOfMeshElements());

			store(generatedIds.back().c_str());
		}
	}

	if (doWork)
		WriteAllMeshesStats(allstats);
	else
		my::global::GetConsole() << "skipping work...\n";

	CleanUp();
}

namespace my {

void MeshProcess (void) {
	std::list<std::string> generatedIds;

	Tesselate(generatedIds, true);
	Savidise(generatedIds);
}

} // my
