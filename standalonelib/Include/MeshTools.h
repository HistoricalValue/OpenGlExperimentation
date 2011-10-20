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

namespace ankh		{
namespace shapes	{
namespace x			{

///////////////////////////////////////////////////////////

class MeshElementProcessor {
public:
	virtual MeshElement operator () (const MeshElement& original) const = 0;

	UDECLARE_ABSTRACT_INPLACE_CLONE(MeshElementProcessor)
	virtual ~MeshElementProcessor (void) {}
};

///////////////////////////////////////////////////////////

class NormalAndWindingInverserMeshElementProcessor: public MeshElementProcessor {
public:
	virtual MeshElement operator () (const MeshElement& original) const {
		DASSERT(original.HasNormals());
		MeshElement resulting(math::types::Triangle(original.a, original.c, original.b));
		resulting.CopyAllFrom(original);

		for (util_ui8 i(0u); i < 3u; ++i)
			resulting.SetNormal(i, -resulting.GetNormal(i));

		return resulting;
	}

	virtual ~NormalAndWindingInverserMeshElementProcessor (void) {}
	UDEFINE_INPLACE_CLONE_VIA_COPY_CONSTRUCTOR(NormalAndWindingInverserMeshElementProcessor)
};

///////////////////////////////////////////////////////////

template <typename Processor>
static inline Mesh::Elements& Process (Mesh::Elements& transformed, const Mesh::Elements& elements, const Processor& proc)
	{ std::transform(elements.begin(), elements.end(), std::back_inserter(transformed), proc); return transformed; }

template <typename Processor>
static inline Mesh::Elements& Process (Mesh::Elements& elements, const Processor& proc)
	{ std::transform(elements.begin(), elements.end(), elements.begin(), proc); return elements; }

static inline Mesh::Elements& InvertNormalsAndWinding (Mesh::Elements& transformed, const Mesh::Elements& elements)
	{ return Process(transformed, elements, NormalAndWindingInverserMeshElementProcessor()); }
static inline Mesh::Elements& InvertNormalsAndWinding (Mesh::Elements& elements)
	{ return Process(elements, NormalAndWindingInverserMeshElementProcessor()); }

static inline Mesh::Elements& ComputeBarycentricFactors (Mesh::Elements& elements)
	{ std::for_each(elements.begin(), elements.end(), std::mem_fun_ref(&math::types::Triangle::ComputeBarycentricFactors)); return elements; }

///////////////////////////////////////////////////////////

}	// x
}	// shapes
}	// ankh

///////////////////////////////////////////////////////////

#endif	// Do not add stuff beyond this point.
