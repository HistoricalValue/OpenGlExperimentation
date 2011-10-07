#include <stdafx.h>
#include <Mesh.h>
#include <MeshLoader.h>
#include <drawing_nurbs.h>

using namespace ankh;
using namespace shapes;
using namespace math;
using namespace trig;
using namespace types;

static char const* const meshesnames[] = {
	"moon_valley_bumps_1700_with_ao17_aotime891116ms",
	"moon_valley_bumps_1700_with_ao9_aotime455218ms"
};

static void Savidise (void) {
	MeshLoader::SingletonCreate();

	for (char const* const* meshname = &meshesnames[0]; meshname < &meshesnames[_countof(meshesnames)]; ++meshname) {
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

static void Tesselate (void) {
	my::drawing::nurbs::Initialise();
	my::drawing::nurbs::tesselate(&ankh::nurbs::TesselationParameters(1e-2f, false, ankh::nurbs::DefaultPrecision()));
	my::drawing::nurbs::CleanUp();
}

namespace my {

void MeshProcess (void) {
	Savidise();
//	Tesselate();
} // MeshProcess()

} // my
