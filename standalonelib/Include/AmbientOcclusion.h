// AmbientOcclusion.h
// Calculation of ambient occlusion factors
// for a triangle mesh.
//
// MMOG Project
// Nikos Mouchtaris, 2nd October 2011

#ifdef ANKH_AMBIENT_OCCLUSION_H_INCLUDING
#    error "Cyclic inclusion"
#endif
#define ANKH_AMBIENT_OCCLUSION_H_INCLUDING

#ifndef ANKH_AMBIENT_OCCLUSION_H
#define ANKH_AMBIENT_OCCLUSION_H

#include <MyUtils.h>
#pragma warning( push, 0 )
#    include "Mesh.h"
#    include "MathTypes.h"
#    include "Trig.h"
#    include <vector>
#    include <map>
#pragma warning( pop )

///////////////////////////////////////////////////////////

namespace ankh    {
namespace ao    {

///////////////////////////////////////////////////////////

enum SamplingRate {
    SamplingRate_9        = 9u,
    SamplingRate_17        = 17u
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

struct vec3_comparator {
    bool operator () (math::trig::vec3 const& l, math::trig::vec3 const& r) const
        { return l.x < r.x || l.x == r.x && (l.y < r.y || l.y == r.y && l.z < r.z); }
};

struct coordsys_equator {
    bool operator () (math::types::CoordinateSystem const& l, math::types::CoordinateSystem const& r) const
        { return l.GetOrigin() == r.GetOrigin() && l.GetX() == r.GetX() && l.GetY() == r.GetY() && l.GetZ() == r.GetZ(); }
};

struct IntersectionData {
    shapes::MeshElement const*        hitElement;
    float                            distance;
    math::trig::vec3                pt, worldRay;
    math::types::CoordinateSystem    coordsys;

    bool    HasIntersected (void) const
                { return hitElement != NULL; }

    bool    CoordSysEquals (math::types::CoordinateSystem const& other) const
                { return coordsys_equator()(coordsys, other); }

    IntersectionData (
            math::trig::vec3 const&                _pt            = math::trig::vec3(),
            math::trig::vec3 const&                _worldRay    = math::trig::vec3(),
            math::types::CoordinateSystem const&_coordsys    = math::types::CoordinateSystem(math::trig::vec3(), math::trig::vec3(0, 0, 1)),
            shapes::MeshElement const* const    _hitElement    = NULL,
            float const                            _distance    = 1.0f):
        hitElement(_hitElement), distance(_distance), pt(_pt), worldRay(_worldRay), coordsys(_coordsys) {}

    IntersectionData (IntersectionData const& d):
        hitElement(d.hitElement), distance(d.distance), pt(d.pt), worldRay(d.worldRay), coordsys(d.coordsys) {}

    UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(IntersectionData)
};

///////////////////////////////////////////////////////////

// Mapping from base-sampling-ray to (ray) IntersectionData
typedef std::map<math::trig::vec3, IntersectionData, vec3_comparator> VertexIntersectionData;

struct MeshElementIntersectionData {
    VertexIntersectionData a, b, c;
    shapes::MeshElement el;

    MeshElementIntersectionData&    Reset (shapes::MeshElement const& _el)
                                        { a.clear(); b.clear(); c.clear(); el = _el; return *this; }

    VertexIntersectionData&            operator [] (util_ui8 const i)
                                        { DASSERT(i < 3); return i == 0? a : i == 1? b : c; }
    const VertexIntersectionData&    operator [] (util_ui8 const i) const
                                        { DASSERT(i < 3); return i == 0? a : i == 1? b : c; }

    MeshElementIntersectionData (shapes::MeshElement const& _el): a(), b(), c(), el(_el) {}
    MeshElementIntersectionData (MeshElementIntersectionData const& d): a(d.a), b(d.b), c(d.c), el(d.el) {}

private:
    void operator = (MeshElementIntersectionData const&);
};

///////////////////////////////////////////////////////////

typedef std::map<shapes::MeshElement const*, MeshElementIntersectionData> MeshIntersectionData;

static inline MeshElementIntersectionData& addnew (MeshIntersectionData& into, shapes::MeshElement const* el)
    { DASSERT(el); return umapadd(into, el, MeshElementIntersectionData(*el)); }

extern
void ProduceIntersectionData (MeshIntersectionData&, shapes::Mesh::Elements const&, SamplingRate);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

// Makes sampling vectos assuming the O point as "begin"
// and the z axis as "normal".
extern
void GenerateNormalisedBaseSamplingVectors (std::list<math::trig::vec3>& into, SamplingRate samplingRate);

// Complexity of about 27n^2 (very unusable, requires 78,030,000 "steps" for just 1700 triangles)
extern
void UpdateAmbientOcclusionFactors (shapes::Mesh::Elements& elements, const SamplingRate samplingRate, MeshIntersectionData* = NULL);

///////////////////////////////////////////////////////////
// AmbientOcclusionCreator related

class AmbientOcclusionCreatorFactory {
public:
    static void        Initialise (void);
    static void        CleanUp (void);
    static bool        IsInitialised (void);

    typedef shapes::Mesh::AmbientOcclusionCreator    AOC;

    static AOC*        New (SamplingRate, const shapes::Mesh::Elements*, MeshIntersectionData*);
    static void        Delete (AOC*);
};

///////////////////////////////////////////////////////////

class AmbientOcclusionCreatorProxy: public shapes::Mesh::AmbientOcclusionCreator {
public:
    typedef shapes::Mesh::AmbientOcclusionCreator Base;
    typedef shapes::Mesh::AmbientOcclusionCreator Gateway;

private:
    Dptr<Gateway> gateway;

public:
    virtual void                            operator () (shapes::MeshElement const& elem, float (*&ambientOcclusion)[3]) const
                                                { gateway->operator ()(elem, ambientOcclusion); }

    shapes::Mesh::AmbientOcclusionCreator*    Clone (void) const
                                                { return gateway->Clone(); }

    AmbientOcclusionCreatorProxy (SamplingRate const sr, const shapes::Mesh::Elements* const els, MeshIntersectionData* const mid):
        Base    (),
        gateway    (AmbientOcclusionCreatorFactory::New(sr, els, mid))
        {}
    virtual ~AmbientOcclusionCreatorProxy (void)
        { AmbientOcclusionCreatorFactory::Delete(gateway.native()); gateway.nullify(); }

private:
    AmbientOcclusionCreatorProxy (AmbientOcclusionCreatorProxy const&);
    void operator = (AmbientOcclusionCreatorProxy const&);
};

///////////////////////////////////////////////////////////

class AnyAmbientOcclusionCreatorProxy: public shapes::Mesh::AmbientOcclusionCreator {
public:
    typedef shapes::Mesh::AmbientOcclusionCreator    Gateway;
    typedef    shapes::Mesh::AmbientOcclusionCreator    Base;

    virtual void    operator () (shapes::MeshElement const& elem, float (*&ambientOcclusion)[3]) const
                        { gateway->operator ()(elem, ambientOcclusion); }

    UCLONE_VIA_COPY_CONSTRUCTOR(AnyAmbientOcclusionCreatorProxy, Clone)
    UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(AnyAmbientOcclusionCreatorProxy)
    AnyAmbientOcclusionCreatorProxy (Gateway* const _gateway):
        Base    (),
        gateway    (_gateway)
        {}
    AnyAmbientOcclusionCreatorProxy (AnyAmbientOcclusionCreatorProxy const& other):
        Base    (other),
        gateway    (other.gateway->Clone())
        {}
    virtual ~AnyAmbientOcclusionCreatorProxy (void)
        { gateway.Delete(); }

private:
    Dptr<Gateway>    gateway;
};

///////////////////////////////////////////////////////////

class IneffectiveAmbientOcclusionCreator: public shapes::Mesh::AmbientOcclusionCreator {
public:
    virtual void    operator () (shapes::MeshElement const&, float (*&)[3]) const {}
    NMUSTANDARD_STATELESS_OBJECT_METHODS(IneffectiveAmbientOcclusionCreator)
};

///////////////////////////////////////////////////////////

}    // ao
}    // ankh

///////////////////////////////////////////////////////////

#endif    // Do not add stuff beyond this point.
#undef ANKH_AMBIENT_OCCLUSION_H_INCLUDING
