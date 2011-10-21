// MeshTools.h
// See header file for further info.
//
// MMOG Project
// Nikos Mouchtaris, 20 October 2011

#include "MeshTools.h"
#include <MyUtils.h>
#include <PStaticAssert.h>

///////////////////////////////////////////////////////////

namespace ankh_x	{
namespace shapes	{

///////////////////////////////////////////////////////////

// NormalAndWindingInverserMeshElementProcessor

void NormalAndWindingInverserMeshElementProcessor::operator () (MeshElement& elem) const {
	DASSERT(elem.HasNormals());

	std::swap(elem.b, elem.c);
	math::trig::vec3 nb(elem.GetNormal(1)), nc(elem.GetNormal(2));
	std::swap(nb, nc);

	elem.SetNormal(0, -elem.GetNormal(0));
	elem.SetNormal(1, -nb);
	elem.SetNormal(2, -nc);
}

///////////////////////////////////////////////////////////

//MeshTimingStats

MeshStats::timing_t MeshStats::operator [] (Timing const timing) const {
	DASSERT(size_t(Tesselation) <= timing && timing <= size_t(StoreText2));

	timing_t result;

	switch (timing) {
		case Tesselation		: result = Tesselation				; break;
		case BarycentricFactors	: result = BarycentricFactors		; break;
		case BoundingVolume		: result = BoundingVolume			; break;
		case Savidise			: result = Savidise					; break;
		case Aabb				: result = Aabb						; break;
		case Update1			: result = Update1					; break;
		case Update2			: result = Update2					; break;
		case IndexBuffer1		: result = IndexBuffer1				; break;
		case IndexBuffer2		: result = IndexBuffer2				; break;
		case StoreBin1			: result = StoreBin1				; break;
		case StoreText1			: result = StoreText1				; break;
		case StoreBin2			: result = StoreBin2				; break;
		case StoreText2			: result = StoreText2				; break;
		default:
			result = std::numeric_limits<timing_t>::max();
			DASSERT(false);
	};

	return result;
}

void MeshStats::operator >> (std::list<std::string>& at) const {
	static const char			LongestMessage[] = "Compute index buffer for mesh2 (antisavidised)";
	static const char* const	Descriptions[] = {
		"Mesh tesselation",
		"Compute barycentric factors",
		"Compute bounding volume",
		"Savidise Mesh Elements",
		"Compute AABB Mesh Tree",
		"Update mesh1 with elements",
		"Update mesh2 (antisavidised) with elements",
		"Compute index buffer for mesh1",
		&LongestMessage[0],
		"Store binary mesh",
		"Store text mesh",
		"Store binary mesh (antisavidised)",
		"Store text mesh (antisavidised)"
	};
	static const Timing AllTimings[] = {
		Tesselation			,	//  0u
		BarycentricFactors	,	//  1u
		BoundingVolume		,	//  2u
		Savidise			,	//  3u
		Aabb				,	//  4u
		Update1				,	//  5u
		Update2				,	//  6u
		IndexBuffer1		,	//  7u
		IndexBuffer2		,	//  8u
		StoreBin1			,	//  9u
		StoreText1			,	// 10
		StoreBin2			,	// 11u
		StoreText2				// 12u
	};
	P_STATIC_ASSERT(countof(AllTimings) == countof(Descriptions))

	size_t maxLength(countof(LongestMessage));

	PASSERT(maxLength < 500)

	at.push_back(format(" ==== Generation statistic for step = %3.1f ==== ", step));
	at.push_back(format(" Number of triangles: %lu ", numberOfElements));

	FOREACH(MeshStats::Timing const, AllTimings, timing)
		at.push_back(format("%*s : %ld", maxLength, Descriptions[size_t(*timing)], (*this)[*timing]));

	// and a blank
	at.push_back(std::string());
}

MeshStats::MeshStats (void):
	valueOfTesselation				(ULONG_MAX),
	valueOfBarycentricFactors		(ULONG_MAX),
	valueOfBoundingVolume			(ULONG_MAX),
	valueOfSavidise					(ULONG_MAX),
	valueOfAabb						(ULONG_MAX),
	valueOfUpdate1					(ULONG_MAX),
	valueOfUpdate2					(ULONG_MAX),
	valueOfIndexBuffer1				(ULONG_MAX),
	valueOfIndexBuffer2				(ULONG_MAX),
	valueOfStoreBin1				(ULONG_MAX),
	valueOfStoreText1				(ULONG_MAX),
	valueOfStoreBin2				(ULONG_MAX),
	valueOfStoreText2				(ULONG_MAX),
	timedTesselation				(false),
	timedBarycentricFactors			(false),
	timedBoundingVolume				(false),
	timedSavidise					(false),
	timedAabb						(false),
	timedUpdate1					(false),
	timedUpdate2					(false),
	timedIndexBuffer1				(false),
	timedIndexBuffer2				(false),
	timedStoreBin1					(false),
	timedStoreText1					(false),
	timedStoreBin2					(false),
	timedStoreText2					(false),
	notifee							(NULL),
	numberOfElements				(0u),	
	step							(-1.0f)
	{}

///////////////////////////////////////////////////////////

void FlushTo (FILE* const fout, const std::list<std::string>& lines) {
	IFOREACH(std::list<std::string>::const_iterator, lines, i)
		fprintf(fout, "%s\n", ucstringarg(*i));
}

///////////////////////////////////////////////////////////

// MeshIndex
struct MeshIndexPlaceholder { char data[sizeof(MeshIndex)]; };\
static MeshIndexPlaceholder*	meshIndexSingleton(NULL);

void MeshIndex::SingletonCreate (void) {
	unew(meshIndexSingleton);
	new(&GetSingleton()) MeshIndex;
}

void MeshIndex::SingletonDestroy (void) {
	GetSingleton().~MeshIndex();
	udelete(meshIndexSingleton);
}

MeshIndex& MeshIndex::GetSingleton (void)
	{ return *reinterpret_cast<MeshIndex* const>(DPTR(DNULLCHECK(meshIndexSingleton))); }

void MeshIndex::Store (void) const {
	FILE* const fout(ubinaryfileopen("../meshes/index.mdx", "w"));
	DASSERT(fout);
	
	{
		PortableBinFileWriter w(fout);

		w.write(util_ui32(meshes.size()));
		for (std::map<std::string, Entry>::const_iterator p(meshes.begin()); p != meshes.end(); ++p) {
			w.write(p->first);
			p->second.WriteTo(w);
		}
	}

	fclose(fout);
}

void MeshIndex::Load (void) {
	MeshIndex loading;

	{
		FILE* const fin(ubinaryfileopen("../meshes/index.mdx", "r"));
		DASSERT(fin);
		{
		
			PortableBinFileReader	r(fin);
			bool					ok;

			util_ui32 numEntries(0u);
			ok = r.read(&numEntries);
			DASSERT(ok);

			for (util_ui32 i(0u); i < numEntries; ++i) {
				std::string id;
				ok = r.read(id, false);
				DASSERT(ok);

				umapadd(meshes, id).ReadFrom(r);
			}
		}
		fclose(fin);
	}

	*this = loading;
}

// MeshIndex::Entry

void MeshIndex::Entry::WriteTo (GenericWriter& w) const {
	w.write(path);
}

void MeshIndex::Entry::ReadFrom (GenericReader& r) {
	Entry	loading;
	
	{
		bool	ok;

		ok = r.read(path, false);
		DASSERT(ok);
	}

	*this = loading;
}

///////////////////////////////////////////////////////////

}	// shapes
}	// ankh_x

///////////////////////////////////////////////////////////
