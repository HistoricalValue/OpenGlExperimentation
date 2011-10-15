#include <stdafx.h>

#include "TheCursed.h"

#include <Mesh.h>
#include <MeshLoader.h>
#include <drawing_nurbs.h>
#include <drawing_utils.h>
#include <BuiltinShapesBoundingVolume.h>

using namespace ankh;
using namespace shapes;
using namespace math;
using namespace trig;
using namespace types;

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

static void IdForStep (char (*buf)[1024], char const* const base, float const step) {
	_snprintf_s(&(*buf)[0], countof(*buf), countof(*buf) - 1u, "%s_%3.1f", base, step);
}

struct MeshTimingsEntry {
	std::list<std::pair<std::string, unsigned long int> >	times;
	unsigned long int										numberOfMeshElements;
};

static void Tesselate (void) {
	using ankh::nurbs::Unit;
	Unit steps[] = { 2e0f, 1e0f, 5e-1f }; //, 4e-1f, 3e-1f, 2e-1f };
	typedef std::pair<Unit, MeshTimingsEntry>	MeshTimingsForUnit;
	typedef std::list<MeshTimingsForUnit>		MeshTimingsPerUnit;

	MeshTimingsPerUnit	timings;

	my::drawing::nurbs::Initialise();

	FOREACH(Unit, steps, step) {
		timings.push_back(MeshTimingsForUnit(*step, MeshTimingsEntry()));

		my::drawing::nurbs::SetTimesList(&timings.back().second.times);
		my::drawing::nurbs::tesselate(&ankh::nurbs::TesselationParameters(*step, false, ankh::nurbs::DefaultPrecision()));
		
		timings.back().second.numberOfMeshElements = my::drawing::nurbs::GetNumberOfMeshElements();

		{	char buf[1024]; IdForStep(&buf, "moon_valley", *step);
			my::drawing::nurbs::store(&buf[0]);
		}
	}

	my::drawing::nurbs::CleanUp();
}

namespace my {

void MeshProcess (void) {
//	Savidise();
	Tesselate();
}

} // my
