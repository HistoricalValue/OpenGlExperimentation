// MeshTools.h
// See header file for further info.
//
// MMOG Project
// Nikos Mouchtaris, 20 October 2011

#include "MeshTools.h"
#define CODESHARE_UTILITIES_IMPORT
#include <MyUtils.h>

///////////////////////////////////////////////////////////

namespace ankh_x	{
namespace shapes	{

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
		"Storing text mesh"
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
	};

	size_t maxLength(_countof("Updating mesh with elements and computing AO"));
	IFOREACH(MeshTimingStats::Custom::const_iterator, mt.custom, i)
		maxLength = std::max(i->first.length(), maxLength);

	PASSERT(maxLength < 500)

	FOREACH(MeshTimingStats::Timing const, AllTimings, timing)
		at.push_back(format<char, &vsnprintf_s>("%*s : %ld", maxLength, Descriptions[size_t(*timing)], mt.GetTimingFor(*timing)));

	IFOREACH(MeshTimingStats::Custom::const_iterator, mt.custom, i)
		at.push_back(format<char, &vsnprintf_s>("%*s : %ld", maxLength, ucstringarg(i->first), i->second));
}

void FlushTo (FILE* const fout, const std::list<std::string>& lines) {
	IFOREACH(std::list<std::string>::const_iterator, lines, i)
		fputs(ucstringarg(*i), fout);
}

///////////////////////////////////////////////////////////

}	// shapes
}	// ankh_x

///////////////////////////////////////////////////////////
