// MeshTools.h
// Useful common tasks for manipulating meshes.
//
// MMOG Project
// Nikos Mouchtaris, 20 October 2011

#ifndef MESH_TOOLS_H
#define MESH_TOOLS_H

#pragma warning( push, 0 )
#	include "Mesh.h"
#	include <algorithm>
#	include <iterator>
#	include "utypes.h"
#	include "usystem.h"
#	include <list>
#pragma warning( pop )

#define UDECLARE_ABSTRACT_INPLACE_CLONE(CLASS)							\
		virtual CLASS*	Clone (void* at, size_t bytesize) const = 0;	\
		virtual CLASS*	Clone (void) const = 0;

#define UDEFINE_INPLACE_CLONE_VIA_COPY_CONSTRUCTOR(CLASS)				\
		virtual															\
		CLASS*	Clone (void* at, size_t bytesize) const {				\
					if (bytesize >= sizeof(CLASS))						\
						return new(at) CLASS (*this);					\
					return NULL;										\
				}														\
		CLASS*	Clone (void) const										\
					{ return DNEWCLASS(CLASS, (*this)); }

///////////////////////////////////////////////////////////

namespace ankh_x	{
namespace shapes	{

using namespace ankh;
using namespace ankh::shapes;

///////////////////////////////////////////////////////////

class MeshElementProcessor {
public:
	virtual void operator () (MeshElement& elem) const = 0;

	UDECLARE_ABSTRACT_INPLACE_CLONE(MeshElementProcessor)
	virtual ~MeshElementProcessor (void) {}
};

///////////////////////////////////////////////////////////

class NormalAndWindingInverserMeshElementProcessor: public MeshElementProcessor {
public:
	virtual void operator () (MeshElement& elem) const {
		DASSERT(elem.HasNormals());

		std::swap(elem.b, elem.c);
		math::trig::vec3 nb(elem.GetNormal(1)), nc(elem.GetNormal(2));
		std::swap(nb, nc);

		elem.SetNormal(0, -elem.GetNormal(0));
		elem.SetNormal(1, -nb);
		elem.SetNormal(2, -nc);

		if (elem.HasAmbientOcclusion()) {
			float aob(elem.GetAmbientOcclusion(1)), aoc(elem.GetAmbientOcclusion(2));
			std::swap(aob, aoc);

			elem.SetAmbientOcclusion(1, aob);
			elem.SetAmbientOcclusion(2, aoc);
		}
	}

	virtual ~NormalAndWindingInverserMeshElementProcessor (void) {}
	UDEFINE_INPLACE_CLONE_VIA_COPY_CONSTRUCTOR(NormalAndWindingInverserMeshElementProcessor)
};

///////////////////////////////////////////////////////////

template <typename Processor>
static inline Mesh::Elements& Process (Mesh::Elements& transformed, const Mesh::Elements& elements, const Processor& proc)
	{ transformed = elements; std::for_each(transformed.begin(), transformed.end(), proc); return transformed; }

template <typename Processor>
static inline Mesh::Elements& Process (Mesh::Elements& elements, const Processor& proc)
	{ std::for_each(elements.begin(), elements.end(), proc); return elements; }

static inline Mesh::Elements& InvertNormalsAndWinding (Mesh::Elements& transformed, const Mesh::Elements& elements)
	{ return Process(transformed, elements, NormalAndWindingInverserMeshElementProcessor()); }
static inline Mesh::Elements& InvertNormalsAndWinding (Mesh::Elements& elements)
	{ return Process(elements, NormalAndWindingInverserMeshElementProcessor()); }

static inline Mesh::Elements& ComputeBarycentricFactors (Mesh::Elements& elements)
	{ std::for_each(elements.begin(), elements.end(), std::mem_fun_ref(&math::types::Triangle::ComputeBarycentricFactors)); return elements; }

///////////////////////////////////////////////////////////

#define MESH_TIMING_STAT(NAME)											\
		timing_t	NAME;												\
		void		Start##NAME (void) {								\
						DNULLCHECK(notifee)->TimingStarted(#NAME);		\
						NAME = ugettime();								\
					}													\
		void		End##NAME (void) {									\
						NAME = ugettime() - NAME;						\
						DNULLCHECK(notifee)->TimingEnded(#NAME, NAME);	\
					}													\

struct MeshTimingStats {
	typedef unsigned long timing_t;
	enum Timing {
		Tesselation			= 0u,
		BarycentricFactors	= 1u,
		BoundingVolume		= 2u,
		Aabb				= 3u,
		Update				= 4u,
		IndexBuffer			= 5u,
		StoreBin			= 6u,
		StoreText			= 7u,
		Savidise			= 8u
	};
	timing_t	GetTimingFor (Timing) const;

	MESH_TIMING_STAT(tesselation		)
	MESH_TIMING_STAT(barycentricFactors	)
	MESH_TIMING_STAT(boundingVolume		)
	MESH_TIMING_STAT(aabb				)
	MESH_TIMING_STAT(update				)
	MESH_TIMING_STAT(indexBuffer		)
	MESH_TIMING_STAT(storeBin			)
	MESH_TIMING_STAT(storeText			)
	MESH_TIMING_STAT(savidise			)

	typedef std::list<std::pair<std::string, timing_t> >	Custom;

	void	AddCustom (char const* const what, timing_t const t) {
				custom.push_back(std::make_pair(std::string(what), t));
				DNULLCHECK(notifee)->TimingStarted(what);
				notifee->TimingEnded(what, t);
			}

	struct TimeUpdateNotifee {
		virtual ~TimeUpdateNotifee (void) {}
		virtual void	TimingStarted (char const* what) const;
		virtual void	TimingEnded (char const* what, timing_t howMuch) const;
	};

	MeshTimingStats&	Reset (void)
							{ custom.clear();tesselation = barycentricFactors = boundingVolume = aabb = update = indexBuffer = storeBin = storeText = 0ul; return *this; }


	Custom				custom;
	TimeUpdateNotifee*	notifee;
};

inline void MeshTimingStats::TimeUpdateNotifee::TimingStarted	(char const* const) const {}
inline void MeshTimingStats::TimeUpdateNotifee::TimingEnded		(char const* const, timing_t const) const {}

#define MESH_TIME(OBJ,WHAT,COMMAND)						\
		OBJ.Start##WHAT();								\
		COMMAND;										\
		OBJ.End##WHAT();								\

extern void	WriteText (std::list<std::string>& at, const MeshTimingStats&);
extern void FlushTo (FILE*, const std::list<std::string>& lines);

///////////////////////////////////////////////////////////

}	// shapes
}	// ankh_x

///////////////////////////////////////////////////////////

#endif	// Do not add stuff beyond this point.
