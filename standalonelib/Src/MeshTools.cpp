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

class MeshStatsHelper {
public:
	typedef nmutripletypes<MeshStats::timing_t&, bool&, char const* const> TTs;
	NMUTRIPLES(TimingEntry, TTs, value, timed, name)

	template <typename EntryType, typename MeshStatsType>
	static
	EntryType GetTimingEntry (MeshStatsType& mt, MeshStats::Timing const timing);

	static
	TimingEntry			Get (MeshStats& mt, MeshStats::Timing const timing)
							{ return GetTimingEntry<TimingEntry>(mt, timing); }
	static
	ConstTimingEntry	Get (MeshStats const& mt, MeshStats::Timing const timing)
							{ return GetTimingEntry<ConstTimingEntry>(mt, timing); }
};

template <typename EntryType, typename MeshStatsType>
static inline
EntryType MeshStatsHelper::GetTimingEntry (MeshStatsType& mt, MeshStats::Timing const timing) {
	DASSERT(MeshStats::IsValidTiming(size_t(timing)));

	switch (timing) {
		case MeshStats::Tesselation			: return EntryType(mt.valueOfTesselation		, mt.timedTesselation			, "Tesselation"			); break;
		case MeshStats::BarycentricFactors	: return EntryType(mt.valueOfBarycentricFactors	, mt.timedBarycentricFactors	, "BarycentricFactors"	); break;
		case MeshStats::BoundingVolume		: return EntryType(mt.valueOfBoundingVolume		, mt.timedBoundingVolume		, "BoundingVolume"		); break;
		case MeshStats::Savidise			: return EntryType(mt.valueOfSavidise			, mt.timedSavidise				, "Savidise"			); break;
		case MeshStats::Aabb				: return EntryType(mt.valueOfAabb				, mt.timedAabb					, "Aabb"				); break;
		case MeshStats::Update1				: return EntryType(mt.valueOfUpdate1			, mt.timedUpdate1				, "Update1"				); break;
		case MeshStats::Update2				: return EntryType(mt.valueOfUpdate2			, mt.timedUpdate2				, "Update2"				); break;
		case MeshStats::IndexBuffer1		: return EntryType(mt.valueOfIndexBuffer1		, mt.timedIndexBuffer1			, "IndexBuffer1"		); break;
		case MeshStats::IndexBuffer2		: return EntryType(mt.valueOfIndexBuffer2		, mt.timedIndexBuffer2			, "IndexBuffer2"		); break;
		case MeshStats::StoreBin1			: return EntryType(mt.valueOfStoreBin1			, mt.timedStoreBin1				, "StoreBin1"			); break;
		case MeshStats::StoreText1			: return EntryType(mt.valueOfStoreText1			, mt.timedStoreText1			, "StoreText1"			); break;
		case MeshStats::StoreBin2			: return EntryType(mt.valueOfStoreBin2			, mt.timedStoreBin2				, "StoreBin2"			); break;
		case MeshStats::StoreText2			: return EntryType(mt.valueOfStoreText2			, mt.timedStoreText2			, "StoreText2"			); break;
		default:
			DASSERT(false);
			return EntryType(mt.valueOfTesselation, mt.timedTesselation, "FAULTY TOWERS");
	};
}

// static
bool MeshStats::IsValidTiming (size_t const timing)
	{ return timing <= NumberOfTimings; }

MeshStats::timing_t MeshStats::operator [] (Timing const timing) const
	{ return MeshStatsHelper::Get(*this, timing).value; }

MeshStats& MeshStats::Reset (void) {
	TimeUpdateNotifee* const notis(notifee);
	reset(this);
	notifee = notis;
	return *this;
}

bool MeshStats::IsTimed (Timing const timing) const
	{ return MeshStatsHelper::Get(*this, timing).timed; }

void MeshStats::Start (Timing const timing) {
	DASSERT(!IsTimed(timing));
	
	MeshStatsHelper::TimingEntry entry(MeshStatsHelper::Get(*this, timing));
						
	DNULLCHECK(notifee)->TimingStarted(entry.name);	
	entry.value = ugettime();					
	entry.timed = true;						
}

void MeshStats::End (Timing const timing) {
	MeshStatsHelper::TimingEntry entry(MeshStatsHelper::Get(*this, timing));

	entry.value = ugettime() - entry.value;
	DNULLCHECK(notifee)->TimingEnded(entry.name, entry.value);
}

std::list<std::string>& MeshStats::WriteLinesTo (std::list<std::string>& at) const {
	static const char			LongestMessage[] = "Compute index buffer for mesh2 (antisavidised)";
	static const char* const	Descriptions[NumberOfTimings] = {
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
	static const Timing AllTimings[NumberOfTimings] = {
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

	size_t maxLength(countof(LongestMessage));

	PASSERT(maxLength < 500)

	FOREACH(MeshStats::Timing const, AllTimings, timing)
		at.push_back(format("%*s : %ld", maxLength, Descriptions[size_t(*timing)], (*this)[*timing]));

	// and a blank
	at.push_back(std::string());

	return at;
}

MeshStats::MeshStats (void):
	valueOfTesselation				(ULONG_MAX),	//  0u
	valueOfBarycentricFactors		(ULONG_MAX),	//  1u
	valueOfBoundingVolume			(ULONG_MAX),	//  2u
	valueOfSavidise					(ULONG_MAX),	//  3u
	valueOfAabb						(ULONG_MAX),	//  4u
	valueOfUpdate1					(ULONG_MAX),	//  5u
	valueOfUpdate2					(ULONG_MAX),	//  6u
	valueOfIndexBuffer1				(ULONG_MAX),	//  7u
	valueOfIndexBuffer2				(ULONG_MAX),	//  8u
	valueOfStoreBin1				(ULONG_MAX),	//  9u
	valueOfStoreText1				(ULONG_MAX),	// 10
	valueOfStoreBin2				(ULONG_MAX),	// 11u
	valueOfStoreText2				(ULONG_MAX),	// 12u	
	timedTesselation				(false),		//  0u
	timedBarycentricFactors			(false),		//  1u
	timedBoundingVolume				(false),		//  2u
	timedSavidise					(false),		//  3u
	timedAabb						(false),		//  4u
	timedUpdate1					(false),		//  5u
	timedUpdate2					(false),		//  6u
	timedIndexBuffer1				(false),		//  7u
	timedIndexBuffer2				(false),		//  8u
	timedStoreBin1					(false),		//  9u
	timedStoreText1					(false),		// 10
	timedStoreBin2					(false),		// 11u
	timedStoreText2					(false),		// 12u
	notifee							(NULL)
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

///////////////////////////////////////////////////////////

void MeshIndex::ImportAllFromMeshLoader (void) {
	typedef std::map<std::string, Mesh*>				MeshesIdMap;
	typedef std::pair<MeshesEntries::iterator, bool>	MeshesEntriesInsertion;

	MeshLoader&								Loader(MeshLoader::GetSingleton());
	MeshesIdMap const&						allMeshes(Loader.GetAll());

	IFOREACH(MeshesIdMap::const_iterator, allMeshes, meshPair) {
		Mesh* const			mesh(meshPair->second);
		std::string const&	id	(meshPair->first);

		Entry newEntry(Loader.GetPath(mesh));
		MeshesEntriesInsertion insertion(meshes.insert(std::make_pair(id, newEntry)));
		// Must be fresh, otherwise data must agree
		PASSERT(insertion.second || insertion.first->first == id && insertion.first->second == newEntry);
	}
}

void MeshIndex::ExportAllToMeshLoader (void) {
	MeshLoader& Loader(MeshLoader::GetSingleton());

	IFOREACH(MeshesEntries::const_iterator, meshes, entryPair) {
		dptr<Mesh> mesh(Loader.Load(entryPair->second.path));
		PASSERT(mesh->GetUniqueId() == entryPair->first)
		mesh.nullify();
	}
}

///////////////////////////////////////////////////////////

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

				umapadd(loading.meshes, id).ReadFrom(r);
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

		ok = r.read(loading.path, false);
		DASSERT(ok);
	}

	*this = loading;
}

///////////////////////////////////////////////////////////

std::list<std::string>& MeshInfo::WriteLinesTo (std::list<std::string>& into) const {
	into.push_back(format("\n\n === Mesh : %s === ", ucstringarg(meshId)));
	into.push_back(format(" with step=%3.1f => number of triangles: %lu", step, numberOfElements));
	into.push_back(format(" bounded by <%3.1f, %3.1f, %3.1f> - <%3.1f, %3.1f, %3.1f>", min.x, min.y, min.z, max.x, max.y, max.z));
	into.push_back(" generation statistics:");
	return stats.WriteLinesTo(into);
}

static inline math::trig::vec3 MakeLimitFrom (shapes::volumes::BoundingBox const& box, float const& (*cmp) (float const&, float const&)) {
	math::trig::vec3 const	a(box.GetTopLeftFront()),
							b(box.GetBottomRightBack());
	return math::trig::vec3((*cmp)(a.x, b.x), (*cmp)(a.y, b.y), (*cmp)(a.z, b.z));
}
static inline math::trig::vec3 MakeMinFrom (shapes::volumes::BoundingBox const& box)
	{ return MakeLimitFrom(box, &std::min<float>); }
static inline math::trig::vec3 MakeMaxFrom (shapes::volumes::BoundingBox const& box)
	{ return MakeLimitFrom(box, &std::max<float>); }

// MeshInfo
MeshInfo::MeshInfo (Mesh const& mesh, float const _step, MeshStats const& _stats):
	meshId				(mesh.GetUniqueId()													),
	stats				(_stats																),
	min					(MakeMinFrom(DPTR(DNULLCHECK(mesh.GetBoundingVolume()))->GetBox())	),
	max					(MakeMaxFrom(DPTR(DNULLCHECK(mesh.GetBoundingVolume()))->GetBox())	),
	numberOfElements	(mesh.GetElements().size()											),
	step				(_step																)
	{}

MeshInfo::MeshInfo (MeshInfo const& o):
	meshId				(o.meshId			),
	stats				(o.stats			),
	min					(o.min				),
	max					(o.max				),
	numberOfElements	(o.numberOfElements	),
	step				(o.step				)
	{}

//
//	at.push_back(format(" ==== Generation statistic for step = %3.1f ==== ", step));
//	at.push_back(format(" Number of triangles: %lu ", numberOfElements));

///////////////////////////////////////////////////////////

}	// shapes
}	// ankh_x

///////////////////////////////////////////////////////////
