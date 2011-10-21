// MeshTools.h
// See header file for further info.
//
// MMOG Project
// Nikos Mouchtaris, 20 October 2011

#include "MeshTools.h"
#define CODESHARE_UTILITIES_IMPORT
#include <MyUtils.h>

#pragma warning( push, 0 )
#	include "DDebug.h"
#	include <limits>
#pragma warning( pop )

///////////////////////////////////////////////////////////

namespace ankh_x	{
namespace shapes	{

///////////////////////////////////////////////////////////

//MeshTimingStats

MeshTimingStats::timing_t MeshTimingStats::GetTimingFor (Timing const timing) const {
	DASSERT(size_t(Tesselation) <= timing && timing <= size_t(Savidise));
	timing_t result;
	switch (timing) {
		case Tesselation:
			result = tesselation;
			break;
		case BarycentricFactors:
			result = barycentricFactors;
			break;
		case BoundingVolume:
			result = boundingVolume;
			break;
		case Aabb:
			result = aabb;
			break;
		case Update:
			result = update;
			break;
		case IndexBuffer:
			result = indexBuffer;
			break;
		case StoreBin:
			result = storeBin;
			break;
		case StoreText:
			result = storeText;
			break;
		case Savidise:
			result = savidise;
			break;
		default:
			result = std::numeric_limits<timing_t>::max();
			DASSERT(false);
	};

	return result;
}


///////////////////////////////////////////////////////////

void WriteText (std::list<std::string>& at, const MeshTimingStats& mt) {
	static const char			LongestMessage[] = "Updating mesh with elements and computing AO";
	static const char* const	Descriptions[] = {
		"Mesh tesselation",
		"Computing barycentric factors",
		"Computing bounding volume",
		"Computing AABB tree",
		&LongestMessage[0],
		"Computing index buffer",
		"Storing binary mesh",
		"Storing text mesh",
		"Savidising (and recomputing ao)"
	};
	static const MeshTimingStats::Timing AllTimings[] = {
		MeshTimingStats::Tesselation		,
		MeshTimingStats::BarycentricFactors	,
		MeshTimingStats::BoundingVolume		,
		MeshTimingStats::Aabb				,
		MeshTimingStats::Update				,
		MeshTimingStats::IndexBuffer		,
		MeshTimingStats::StoreBin			,
		MeshTimingStats::StoreText			,
		MeshTimingStats::Savidise
	};

	size_t maxLength(_countof("Updating mesh with elements and computing AO"));
	IFOREACH(MeshTimingStats::Custom::const_iterator, mt.custom, i)
		maxLength = std::max(i->first.length(), maxLength);

	PASSERT(maxLength < 500)

	FOREACH(MeshTimingStats::Timing const, AllTimings, timing)
		at.push_back(format("%*s : %ld", maxLength, Descriptions[size_t(*timing)], mt.GetTimingFor(*timing)));

	IFOREACH(MeshTimingStats::Custom::const_iterator, mt.custom, i)
		at.push_back(format("%*s : %ld", maxLength, ucstringarg(i->first), i->second));

	// and a blank
	at.push_back(std::string());
}

void FlushTo (FILE* const fout, const std::list<std::string>& lines) {
	IFOREACH(std::list<std::string>::const_iterator, lines, i)
		fprintf(fout, "%s\n", ucstringarg(*i));
}

///////////////////////////////////////////////////////////

}	// shapes
}	// ankh_x

///////////////////////////////////////////////////////////
