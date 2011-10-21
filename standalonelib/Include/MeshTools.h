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
#	include "DDebug.h"
#	include "GenericReader.h"
#	include "GenericWriter.h"
#	include "ufiles.h"
#	include <limits>
#pragma warning( pop )
#include <MyUtils.h>

///////////////////////////////////////////////////////////

namespace ankh_x	{
namespace shapes	{

using namespace ankh;
using namespace ankh::shapes;

///////////////////////////////////////////////////////////

class MeshElementProcessor {
public:
	virtual void operator () (MeshElement& elem) const = 0;

	UDECLARE_ABSTRACT_CLONE(MeshElementProcessor)
	virtual ~MeshElementProcessor (void) {}
};

///////////////////////////////////////////////////////////

class NormalAndWindingInverserMeshElementProcessor: public MeshElementProcessor {
public:
	virtual void operator () (MeshElement& elem) const;

	virtual ~NormalAndWindingInverserMeshElementProcessor (void) {}
	UDEFINE_CLONE_VIA_COPY_CONSTRUCTOR(NormalAndWindingInverserMeshElementProcessor)
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

#define MESH_TIMING_STAT(NAME)													\
	private:																	\
		timing_t	valueOf##NAME;												\
		bool		timed##NAME;												\
	public:																		\
		void		Start##NAME (void) {										\
						DASSERT(!timed##NAME);									\
						DNULLCHECK(notifee)->TimingStarted(#NAME);				\
						valueOf##NAME = ugettime();								\
						timed##NAME = true;										\
					}															\
		void		End##NAME (void) {											\
						valueOf##NAME = ugettime() - valueOf##NAME;				\
						DNULLCHECK(notifee)->TimingEnded(#NAME, valueOf##NAME);	\
					}															\

struct MeshStats {
	typedef unsigned long									timing_t;
	typedef std::list<std::pair<std::string, timing_t> >	Custom;

	struct TimeUpdateNotifee {
		virtual ~TimeUpdateNotifee (void) {}
		virtual void	TimingStarted (char const* what) const;
		virtual void	TimingEnded (char const* what, timing_t howMuch) const;
	};
	
	enum Timing {
		Tesselation			=  0u,
		BarycentricFactors	=  1u,
		BoundingVolume		=  2u,
		Savidise			=  3u,
		Aabb				=  4u,
		Update1				=  5u,
		Update2				=  6u,
		IndexBuffer1		=  7u,
		IndexBuffer2		=  8u,
		StoreBin1			=  9u,
		StoreText1			= 10u,
		StoreBin2			= 11u,
		StoreText2			= 12u
	};

	MESH_TIMING_STAT(Tesselation		)	//  0
	MESH_TIMING_STAT(BarycentricFactors	)	//  1
	MESH_TIMING_STAT(BoundingVolume		)	//  2
	MESH_TIMING_STAT(Savidise			)	//  3
	MESH_TIMING_STAT(Aabb				)	//  4
	MESH_TIMING_STAT(Update1			)	//  5
	MESH_TIMING_STAT(Update2			)	//  6
	MESH_TIMING_STAT(IndexBuffer1		)	//  7
	MESH_TIMING_STAT(IndexBuffer2		)	//  8
	MESH_TIMING_STAT(StoreBin1			)	//  9
	MESH_TIMING_STAT(StoreText1			)	// 10
	MESH_TIMING_STAT(StoreBin2			)	// 11
	MESH_TIMING_STAT(StoreText2			)	// 12

	timing_t	operator [] (Timing) const;
	MeshStats&	Reset (void) {
					TimeUpdateNotifee* const notis(notifee);
					this->~MeshStats();
					new(this) MeshStats;
					notifee = notis;
					return *this;
				}
	void		operator >> (std::list<std::string>& into) const;

	MeshStats (void);
	// State
	TimeUpdateNotifee*	notifee;
	size_t				numberOfElements;
	float				step;
};

inline void MeshStats::TimeUpdateNotifee::TimingStarted	(char const* const) const {}
inline void MeshStats::TimeUpdateNotifee::TimingEnded	(char const* const, timing_t const) const {}

#define MESH_TIME(OBJ,WHAT,COMMAND)						\
		OBJ.Start##WHAT();								\
		COMMAND;										\
		OBJ.End##WHAT();								\

extern void FlushTo (FILE*, const std::list<std::string>& lines);

///////////////////////////////////////////////////////////

class MeshIndex {
public:
	USINGLETON_APISTYLE_DECLARE_PUBLICSTDMETHODS
	USINGLETON_APISTYLE_DECLARE_GETTER(MeshIndex)

	struct Entry {
		std::string path;

		void WriteTo (GenericWriter&) const;
		void ReadFrom (GenericReader&);

		Entry (void): path() {}
		Entry (std::string const& _path): path(_path) {}
		Entry (Entry const& o): path(o.path) {}
		~Entry (void) {}
		void operator = (Entry const& o) { this->~Entry(); new (this) Entry(o); }
	};

	void Load (void);
	void Store (void) const;

private:
	std::map<std::string, Entry>	meshes;

	MeshIndex (void): meshes() {}
	MeshIndex (MeshIndex const& o): meshes(o.meshes) {}
	~MeshIndex (void) {}
	void operator = (MeshIndex const& o)
		{ this->~MeshIndex(); new(this) MeshIndex(o); }
};

///////////////////////////////////////////////////////////

}	// shapes
}	// ankh_x

///////////////////////////////////////////////////////////

#endif	// Do not add stuff beyond this point.
