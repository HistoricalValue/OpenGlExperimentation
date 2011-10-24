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
#	include "MeshLoader.h"
#	include "BoundingBox.h"
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

	NMUDECLARE_ABSTRACT_CLONE(MeshElementProcessor)
	virtual ~MeshElementProcessor (void) {}
};

///////////////////////////////////////////////////////////

class NormalAndWindingInverserMeshElementProcessor: public MeshElementProcessor {
public:
	virtual void operator () (MeshElement& elem) const;

	virtual ~NormalAndWindingInverserMeshElementProcessor (void) {}
	NMUINLINE_DEFINE_CLONE_VIA_COPY_CONSTRUCTOR(NormalAndWindingInverserMeshElementProcessor)
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

#define MESH_TIMING_STAT(NAME)						\
	private:										\
		timing_t	valueOf##NAME;					\
		bool		timed##NAME;					\
	public:											\
		void		Start##NAME (void)				\
						{ Start(NAME); }			\
		void		End##NAME (void)				\
						{ End(NAME); }				\
		bool		IsTimed##NAME (void) const		\
						{ return IsTimed(NAME); }	\
		void		Reset##NAME (void)				\
						{ return Reset(NAME); }		\

struct MeshStats {
	typedef unsigned long	timing_t;

	struct TimeUpdateNotifee {
		virtual ~TimeUpdateNotifee (void) {}
		virtual void	TimingStarted (char const* what) const;
		virtual void	TimingEnded (char const* what, timing_t howMuch) const;
		virtual void	TimingRestarted (char const* what) const;
	};

	enum Timing {
		Tesselation			=  0u,
		BarycentricFactors	=  1u,
		BoundingVolume		=  2u,
		Savidise			=  3u,
		Aabb				=  4u,
		Update				=  5u,
		IndexBuffer			=  6u,
		StoreBin			=  7u,
		StoreText			=  8u
	};
	static const size_t NumberOfTimings = size_t(StoreText) + 1;

	static bool		IsValidTiming (size_t);

	bool			AreAllTimed (void) const;
	bool			IsNoneTimed (void) const;

	bool			IsTimed (Timing) const;
	void			Start (Timing);
	void			End (Timing);
	void			Reset (Timing);

	timing_t		operator [] (Timing) const;
	MeshStats&		Reset (void);

	void			SetNotifee (TimeUpdateNotifee* const _notifee)
						{ notifee = _notifee; }

	NMUDECLARE_WRITE_LINES
	MeshStats (void);
	MeshStats (MeshStats const&);
	~MeshStats (void);

private:
	// State
	TimeUpdateNotifee*	notifee;
	MESH_TIMING_STAT(Tesselation		)	//  0u
	MESH_TIMING_STAT(BarycentricFactors	)	//  1u
	MESH_TIMING_STAT(BoundingVolume		)	//  2u
	MESH_TIMING_STAT(Savidise			)	//  3u
	MESH_TIMING_STAT(Aabb				)	//  4u
	MESH_TIMING_STAT(Update				)	//  5u
	MESH_TIMING_STAT(IndexBuffer		)	//  6u
	MESH_TIMING_STAT(StoreBin			)	//  7u
	MESH_TIMING_STAT(StoreText			)	//  8u

private:
	friend class MeshStatsHelper;
};

inline void MeshStats::TimeUpdateNotifee::TimingStarted		(char const* const) const {}
inline void MeshStats::TimeUpdateNotifee::TimingEnded		(char const* const, timing_t const) const {}
inline void MeshStats::TimeUpdateNotifee::TimingRestarted	(char const* const) const {}

#define MESH_TIME(OBJ,WHAT,COMMAND)		\
		OBJ.Start##WHAT();				\
		COMMAND;						\
		OBJ.End##WHAT();				\

#define MESH_TIME_VAR(OBJ,WHAT,COMMAND)	\
		OBJ.Start(WHAT);				\
		COMMAND;						\
		OBJ.End(WHAT);					\

///////////////////////////////////////////////////////////

class MeshIndex {
public:
	USINGLETON_APISTYLE_DECLARE_PUBLICSTDMETHODS
	USINGLETON_APISTYLE_DECLARE_GETTER(MeshIndex)

	struct Entry {
		std::string path;

		void	WriteTo (GenericWriter&) const;
		void	ReadFrom (GenericReader&);

		bool	operator == (Entry const& o) const
					{ return path == o.path; }

		Entry (void): path() {}
		Entry (std::string const& _path): path(_path) {}
		Entry (Entry const& o): path(o.path) {}
		~Entry (void) {}
		void operator = (Entry const& o) { this->~Entry(); new (this) Entry(o); }
	};

	void	Load (void);
	void	Store (void) const;

	void	ImportAllFromMeshLoader (void);
	void	ExportAllToMeshLoader (void);

private:
	typedef std::map<std::string, Entry>	MeshesEntries;

	MeshesEntries meshes;

	MeshIndex (void): meshes() {}
	MeshIndex (MeshIndex const& o): meshes(o.meshes) {}
	~MeshIndex (void) {}
	void operator = (MeshIndex const& o)
		{ this->~MeshIndex(); new(this) MeshIndex(o); }
};

///////////////////////////////////////////////////////////

class MeshInfo {
public:
	NMUDECLARE_WRITE_LINES

	MeshInfo (
		Mesh const&			mesh,
		float				step,
		MeshStats const&	stats);
	MeshInfo (MeshInfo const&);
	~MeshInfo (void) {}
	UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(MeshInfo);
private:
	std::string				meshId;
	MeshStats const			stats;
	math::trig::vec3 const	min, max;
	size_t const			numberOfElements;
	float const				step;
};

struct MeshWithInfo {
	dptr<Mesh>			mesh;
	MeshInfo const		info;

	void	NullifyMeshPointer (void)
				{ mesh.nullify(); }

	MeshWithInfo (dptr<Mesh> const& _mesh, float const step, MeshStats const& stats):
		mesh(_mesh),
		info(*_mesh, step, stats)
		{}
	MeshWithInfo (MeshWithInfo const& o): mesh(o.mesh), info(o.info) {}
	~MeshWithInfo (void) {}
	UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(MeshWithInfo);
};

///////////////////////////////////////////////////////////

}	// shapes
}	// ankh_x

///////////////////////////////////////////////////////////

#endif	// Do not add stuff beyond this point.
