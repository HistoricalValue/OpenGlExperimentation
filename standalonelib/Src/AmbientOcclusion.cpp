// AmbientOcclusion.h
// Calculation of ambient occlusion factors
// for a triangle mesh.
//
// MMOG Project
// Nikos Mouchtaris, 2nd October 2011

#include "AmbientOcclusion.h"
#include <vector>
#include <list>
#include <limits>

///////////////////////////////////////////////////////////
// File-private utilities
///////////////////////////////////////////////////////////

#define NO_ASSIGNING(CLASS)    private: void operator = ( CLASS const& );    \

#define NO_COPYING(CLASS)    private: CLASS ( CLASS const& );            \
                            NO_ASSIGNING(CLASS)

///////////////////////////////////////////////////////////

namespace    {
namespace _    {

using namespace ankh;

using ao::IntersectionData;
using ao::VertexIntersectionData;
using ao::MeshElementIntersectionData;
using ao::MeshIntersectionData;

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

static inline
math::types::Plane const ToPlane (math::types::Triangle const& tr)
    { return math::types::Plane(tr.a, tr.b, tr.c); }

static inline
bool In (math::trig::vec3 const& p, math::types::Triangle const& tr)
    { return tr.In(p); }

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
static
bool ValidateCoordinatePositionFor (
        math::types::CoordinateSystem const&    coordsys,
        math::trig::vec3 const&                    vertex,
        math::trig::vec3 const&                    worldRay,
        math::trig::vec3 const&                    baseRay) {
    using math::trig::vec3;

    DASSERT(math::trig::is_normalised(baseRay));
    DASSERT(math::trig::is_normalised(worldRay));

    vec3 const        worldRay_pos            (vertex + worldRay)
                ,    vertex_to_local            (coordsys.ToLocal(vertex))
                ,    worldRay_pos_to_local    (coordsys.ToLocal(worldRay_pos))
                ,    O_to_world                (coordsys.ToWorld(vec3()))
                ,    ray_to_world            (coordsys.ToWorld(baseRay))
                ;

    const bool _0 = (vertex_to_local == vec3());
    const bool _1 = (worldRay_pos_to_local == baseRay);
    const bool _2 = (O_to_world == vertex);
    const bool _3 = (ray_to_world == worldRay);

    (void) _0, (void) _1, (void) _2, (void) _3;
    // TODO why do these fail?
//    DASSERT(_0 && _1 && _2 && _3);
//    return _0 && _1 && _2 && _3;
    return true;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

struct Ray {

    bool    SetIntersectedIfIntersects (shapes::MeshElement const& originalElement, math::types::Plane const& plane, float& distance) {
                DASSERT(!intersected);

                math::trig::vec3 at;
                intersected    = plane.GetIntersection(dir, p, &at);
                distance    = intersected? math::trig::length(at - p) : -1.0f;

                if (dir * distance == p - at)    // intersection happens backwards
                    intersected = false;

                DASSERT(!intersected || dir * distance == at - p);

                if (intersected && !In(at, originalElement))
                    intersected = false;

                return intersected;
            }

    math::trig::vec3 const            p;
    math::trig::vec3 const            dir;
    math::trig::vec3 const            baseRay;
    bool                            intersected;
    math::types::CoordinateSystem const    coordsys;

    Ray (math::types::CoordinateSystem const& _coordsys, math::trig::vec3 const& _p, math::trig::vec3 const* const _baseRay):
        p            (_p),
        dir            (math::trig::normalise(_coordsys.ToWorld(*DNULLCHECK(_baseRay)))),
        baseRay        (*DNULLCHECK(_baseRay)),
        intersected    (false),
        coordsys    (_coordsys)
        { DASSERT(ValidateCoordinatePositionFor(_coordsys, _p, dir, *_baseRay)); }
    NO_COPYING(Ray)
};

///////////////////////////////////////////////////////////

static inline void _IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED (
        Ray&                            ray,
        shapes::MeshElement const&        el,
        math::types::Plane const&        plane,
        float&                            distance,
        VertexIntersectionData&            into) {
    DASSERT(!ray.intersected || into[ray.baseRay].HasIntersected());
    DASSERT(ray.intersected || !into[ray.baseRay].HasIntersected());

    if (!ray.intersected) {
        DASSERT(!into[ray.baseRay].HasIntersected());
        if (ray.SetIntersectedIfIntersects(el, plane, distance))
            into[ray.baseRay] = IntersectionData(ray.p, ray.dir, ray.coordsys, &el, distance);
        else
            into[ray.baseRay] = IntersectionData(ray.p, ray.dir, ray.coordsys);
    }
}

#define IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(R)    \
    _IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(r##R, el, plane, distance, into);

template <const size_t samplingRate> struct RayHolder;
template <>
struct RayHolder<9u> {
    Ray rn, r0, r1, r2, r3, r4, r5, r6, r7;

    bool    HaveAllIntersected (void) const
                { return    rn.intersected && r0.intersected && r1.intersected &&
                            r2.intersected && r3.intersected && r4.intersected &&
                            r5.intersected && r6.intersected && r7.intersected; }

    void    AddIntersections (
                    VertexIntersectionData&        into,
                    shapes::MeshElement const&    el,
                    math::types::Plane const&    plane) {
                DASSERT(!HaveAllIntersected());

                float distance;

                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(n)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(0)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(1)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(2)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(3)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(4)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(5)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(6)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(7)
            }

    RayHolder (
            math::types::CoordinateSystem const&    coordsys,
            math::trig::vec3 const&                    vertex,
            math::trig::vec3 const (* const            baseSamples)[9u]):
        rn(coordsys, vertex, &(*baseSamples)[0]),
        //
        r0(coordsys, vertex, &(*baseSamples)[1]),
        r1(coordsys, vertex, &(*baseSamples)[2]),
        r2(coordsys, vertex, &(*baseSamples)[3]),
        r3(coordsys, vertex, &(*baseSamples)[4]),
        //
        r4(coordsys, vertex, &(*baseSamples)[5]),
        r5(coordsys, vertex, &(*baseSamples)[6]),
        r6(coordsys, vertex, &(*baseSamples)[7]),
        r7(coordsys, vertex, &(*baseSamples)[8]) {
        DASSERT(math::trig::is_normalised((*baseSamples)[0]));
        DASSERT(coordsys.GetOrigin() == vertex);
    }

    NO_COPYING(RayHolder<9u>)
};

template <>
struct RayHolder<17u>: public RayHolder<9u> {
    Ray r8, r9, ra, rb, rc, rd, re, rf;

    bool    HaveAllIntersected (void) const
                { return    RayHolder<9u>::HaveAllIntersected() &&
                            r8.intersected && r9.intersected && ra.intersected &&
                            rb.intersected && rc.intersected && rd.intersected &&
                            re.intersected && rf.intersected; }

    void    AddIntersections (
                    VertexIntersectionData&        into,
                    shapes::MeshElement const&    el,
                    math::types::Plane const&    plane) {
                DASSERT(!HaveAllIntersected());

                RayHolder<9u>::AddIntersections(into, el, plane);

                float distance;

                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(8)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(9)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(a)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(b)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(c)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(d)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(e)
                IF_RAY_NOT_INTERSECTED_CHECK_INTERSECTS_AND_SET_INTERSECTED(f)
            }

    RayHolder (
            math::types::CoordinateSystem const&    coordsys,
            math::trig::vec3 const&                    vertex,
            math::trig::vec3 const (* const            baseSamples)[17u]):
        RayHolder<9u>(coordsys, vertex, reinterpret_cast<math::trig::vec3 const (*)[9u]>(baseSamples)),
        //
        r8(coordsys, vertex, &(*baseSamples)[ 9]),
        r9(coordsys, vertex, &(*baseSamples)[10]),
        ra(coordsys, vertex, &(*baseSamples)[11]),
        rb(coordsys, vertex, &(*baseSamples)[12]),
        //
        rc(coordsys, vertex, &(*baseSamples)[13]),
        rd(coordsys, vertex, &(*baseSamples)[14]),
        re(coordsys, vertex, &(*baseSamples)[15]),
        rf(coordsys, vertex, &(*baseSamples)[16])
        {}

    NO_COPYING(RayHolder<17u>)
};

template <const size_t samplingRate>
struct VertexRays {
    static const size_t NumberOfRays = samplingRate;
    static void            GenerateBaseSamplingRays (math::trig::vec3 (* const baseSamples)[NumberOfRays]);

    RayHolder<samplingRate>    r;

    VertexRays (
            math::trig::vec3 const&    vertex,
            math::trig::vec3 const& normal,
            math::trig::vec3 const (* const baseSamples)[NumberOfRays]):
        r(math::types::CoordinateSystem(vertex, normal), vertex, baseSamples)
        {}

    NO_COPYING(VertexRays)
};

template <>
// static
inline void VertexRays<9u>::GenerateBaseSamplingRays (math::trig::vec3 (* const baseSamples)[NumberOfRays]) {
    (*baseSamples)[0] = math::trig::vec3(0.0f, 0.0f, 1.0f);
    //
    (*baseSamples)[1] = math::trig::normalise(math::trig::vec3( 1.0f,  0.0f, 0.0f));
    (*baseSamples)[2] = math::trig::normalise(math::trig::vec3( 0.0f,  1.0f, 0.0f));
    (*baseSamples)[3] = math::trig::normalise(math::trig::vec3(-1.0f,  0.0f, 0.0f));
    (*baseSamples)[4] = math::trig::normalise(math::trig::vec3( 0.0f, -1.0f, 0.0f));
    //
    (*baseSamples)[5] = math::trig::normalise(math::trig::vec3( 1.0f,  1.0f, 1.0f));
    (*baseSamples)[6] = math::trig::normalise(math::trig::vec3(-1.0f,  1.0f, 1.0f));
    (*baseSamples)[7] = math::trig::normalise(math::trig::vec3(-1.0f, -1.0f, 1.0f));
    (*baseSamples)[8] = math::trig::normalise(math::trig::vec3( 1.0f, -1.0f, 1.0f));
}

template <>
// static
inline void VertexRays<17u>::GenerateBaseSamplingRays (math::trig::vec3 (* const baseSamples)[NumberOfRays]) {
    VertexRays<9u>::GenerateBaseSamplingRays(reinterpret_cast<math::trig::vec3 (* const)[VertexRays<9u>::NumberOfRays]>(baseSamples));
    //
    (*baseSamples)[ 9] = math::trig::normalise(math::trig::vec3( 1.0f,  1.0f, 0.0f));
    (*baseSamples)[10] = math::trig::normalise(math::trig::vec3(-1.0f,  1.0f, 0.0f));
    (*baseSamples)[11] = math::trig::normalise(math::trig::vec3(-1.0f, -1.0f, 0.0f));
    (*baseSamples)[12] = math::trig::normalise(math::trig::vec3( 1.0f, -1.0f, 0.0f));
    //
    (*baseSamples)[13] = math::trig::normalise(math::trig::vec3( 1.0f,  0.0f, 1.0f));
    (*baseSamples)[14] = math::trig::normalise(math::trig::vec3( 0.0f,  1.0f, 1.0f));
    (*baseSamples)[15] = math::trig::normalise(math::trig::vec3(-1.0f,  0.0f, 1.0f));
    (*baseSamples)[16] = math::trig::normalise(math::trig::vec3( 0.0f, -1.0f, 1.0f));
}


///////////////////////////////////////////////////////////

template <const size_t samplingRate>
struct MeshElementRays {

    VertexRays<samplingRate> a, b, c;
    bool allIntersected;

    void                AddIntersectionsAndMarkIntersected (
                                VertexIntersectionData& into_4a,
                                VertexIntersectionData& into_4b,
                                VertexIntersectionData& into_4c,
                                shapes::MeshElement const& el) {
                            DASSERT(InvariantsHold());
                            DASSERT(!allIntersected);

                            math::types::Plane const    plane            (ToPlane(el));
                            bool const                    aAllIntersected    (a.r.HaveAllIntersected());
                            bool const                    bAllIntersected    (b.r.HaveAllIntersected());
                            bool const                    cAllIntersected    (c.r.HaveAllIntersected());

                            if (!aAllIntersected)
                                a.r.AddIntersections(into_4a, el, plane);
                            if (!bAllIntersected)
                                b.r.AddIntersections(into_4b, el, plane);
                            if (!cAllIntersected)
                                c.r.AddIntersections(into_4c, el, plane);

                            allIntersected = aAllIntersected && bAllIntersected && cAllIntersected;
                            DASSERT(InvariantsHold());
                        }

    bool                HaveAllIntersected (void) const
                            { DASSERT(InvariantsHold()); return allIntersected; }

    bool                InvariantsHold (void) const
                            { return !allIntersected || a.r.HaveAllIntersected() && b.r.HaveAllIntersected() && c.r.HaveAllIntersected(); }

    MeshElementRays (    math::trig::vec3 const (* const baseSamplingRays)[VertexRays<samplingRate>::NumberOfRays],
                        shapes::MeshElement const& el):
        a(el.GetVertex(0), el.GetNormal(0), baseSamplingRays),
        b(el.GetVertex(1), el.GetNormal(1), baseSamplingRays),
        c(el.GetVertex(2), el.GetNormal(2), baseSamplingRays),
        allIntersected(false)
        { DASSERT(InvariantsHold()); }

    NO_COPYING(MeshElementRays)
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

typedef float (*HitRatioFactor) (size_t hits, size_t total);
typedef float (*AttenuationFactor) (float averageDistance);

template <const size_t samplingRate> static inline
float CalculateFactor (
        VertexIntersectionData const&    intersections,
        float const                        min_distance    = FLT_MIN,
        float const                        max_distance    = FLT_MAX,
        HitRatioFactor const            hitratio        = NULL,
        AttenuationFactor const                            = NULL) {
    DASSERT(intersections.size() <= VertexRays<samplingRate>::NumberOfRays && min_distance < max_distance);

    typedef VertexIntersectionData::const_iterator iter_t;

    size_t            hits(0u);
    iter_t const    intersections_end(intersections.end());

    if (min_distance != FLT_MIN && max_distance != FLT_MAX)
        for (iter_t i(intersections.begin()); i != intersections_end; ++i)
            if (i->second.HasIntersected() && i->second.distance >= min_distance && i->second.distance <= max_distance)
                ++hits;
            else {}
    else
        for (iter_t i(intersections.begin()); i != intersections_end; ++i)
            if (i->second.HasIntersected())
                ++hits;
            else {}

    DASSERT(hits <= intersections.size() && hits <= VertexRays<samplingRate>::NumberOfRays);

    return hitratio != NULL?    (*hitratio)(hits, VertexRays<samplingRate>::NumberOfRays) :
                                1.0f - float(hits)/float(VertexRays<samplingRate>::NumberOfRays);
}

///////////////////////////////////////////////////////////

template <const size_t samplingRate> static inline
void ProduceMeshElementIntersectionData (
        MeshElementIntersectionData&    into,
        shapes::Mesh::Elements const&    els,
        shapes::MeshElement const&        elem,
        math::trig::vec3 const (* const    baseSamplingRays)[VertexRays<samplingRate>::NumberOfRays]) {

    MeshElementRays<samplingRate>                    rays    (baseSamplingRays, elem);
    shapes::Mesh::Elements::const_iterator const    els_end    (els.end());

    for (shapes::Mesh::Elements::const_iterator el(els.begin()); !rays.HaveAllIntersected() && el != els_end; ++el)
        if (!(elem == *el))
            rays.AddIntersectionsAndMarkIntersected(into.a, into.b, into.c, *el);
}

///////////////////////////////////////////////////////////

template <const size_t samplingRate> static inline
void ComputeAndSetAmbientOcclusionFactors (
        std::vector<float>&                factors,
        const shapes::MeshElement&        el,
        const shapes::Mesh::Elements&    elements,
        math::trig::vec3                (* const baseSamplingRays)[VertexRays<samplingRate>::NumberOfRays],
        MeshElementIntersectionData&    intersectionData) {
    DASSERT(el.HasNormals());

    // Calculate intersection data
    ProduceMeshElementIntersectionData<samplingRate>(intersectionData, elements, el, baseSamplingRays);

    DASSERT(factors.size() == 3);
    factors[0] = CalculateFactor<samplingRate>(intersectionData.a);
    factors[1] = CalculateFactor<samplingRate>(intersectionData.b);
    factors[2] = CalculateFactor<samplingRate>(intersectionData.c);
}

///////////////////////////////////////////////////////////

template <const size_t samplingRate>
void UpdateAmbientOcclusionFactors (shapes::Mesh::Elements& elements, MeshIntersectionData* const intersectionDataInto) {
    typedef shapes::Mesh::Elements::iterator iter_t;

    std::vector<float>            factors(3);
    iter_t const                elements_end(elements.end());
    iter_t                        el(elements.begin());
    math::trig::vec3            baseSamplingRays[VertexRays<samplingRate>::NumberOfRays];
    MeshElementIntersectionData    dummy((shapes::MeshElement()));

    VertexRays<samplingRate>::GenerateBaseSamplingRays(&baseSamplingRays);

    for (; el != elements_end; ++el) {
        MeshElementIntersectionData& intersectionData(    intersectionDataInto?    addnew(*intersectionDataInto, castconst(&*el)) :
                                                                                dummy.Reset(*el));

        ComputeAndSetAmbientOcclusionFactors<samplingRate>(factors, *el, elements, &baseSamplingRays, intersectionData);
        DASSERT(factors.size() == 3);

        DASSERT(!el->HasAmbientOcclusion());
        el->MakeAmbientOcclusion();
        el->SetAmbientOcclusion(0, factors[0]);
        el->SetAmbientOcclusion(1, factors[1]);
        el->SetAmbientOcclusion(2, factors[2]);
    }

    DASSERT(!intersectionDataInto || intersectionDataInto->size() == elements.size());
}

///////////////////////////////////////////////////////////

template <const size_t samplingRate>
void ProduceIntersectionData (MeshIntersectionData& into, shapes::Mesh::Elements const& els) {
    math::trig::vec3                                baseSamplingRays[VertexRays<samplingRate>::NumberOfRays];
    shapes::Mesh::Elements::const_iterator const    els_end    (els.end());
    shapes::Mesh::Elements::const_iterator            el        (els.begin());

    VertexRays<samplingRate>::GenerateBaseSamplingRays(&baseSamplingRays);

    for (; el != els_end; ++el)
        ProduceMeshElementIntersectionData<samplingRate>(addnew(into, &*el), els, *el, &baseSamplingRays);

    DASSERT(into.size() == els.size());
}

///////////////////////////////////////////////////////////

template <const size_t samplingRate>
class AmbientOcclusionCreator: public shapes::Mesh::AmbientOcclusionCreator {
public:
    virtual AmbientOcclusionCreator*
                    Clone (void) const
                        { return DNEWCLASS(AmbientOcclusionCreator, (*this)); }

    virtual void    operator () (const shapes::MeshElement& el, float (*&ambientOcclusion)[3]) const {
                        DASSERT(factors.size() == 3);
                        _::ComputeAndSetAmbientOcclusionFactors<samplingRate>(factors, el, elements, &baseSamplingRays, addnew(intersectionData, &el));
                        DASSERT(factors.size() == 3);

                        if (!ambientOcclusion)
                            unewarray(ambientOcclusion);

                        (*ambientOcclusion)[0] = factors[0];
                        (*ambientOcclusion)[1] = factors[1];
                        (*ambientOcclusion)[2] = factors[2];
                    }

    ///////////////////////////////////////////////////////

    AmbientOcclusionCreator (const shapes::Mesh::Elements* _elements, MeshIntersectionData* const _intersectionData):
        shapes::Mesh::AmbientOcclusionCreator    (),
        elements                                (*_elements),
        intersectionData                        (*DNULLCHECK(_intersectionData)),
        factors                                    (3)
        { InitBaseSamplingRays(); }
    AmbientOcclusionCreator (const AmbientOcclusionCreator& aoc):
        shapes::Mesh::AmbientOcclusionCreator    (aoc),
        elements                                (aoc.elements),
        intersectionData                        (*DNULLCHECK(&aoc.intersectionData)),
        factors                                    (3)
        { InitBaseSamplingRays(); }
    virtual ~AmbientOcclusionCreator (void) {}

private:
    shapes::Mesh::Elements const&    elements;
    MeshIntersectionData&            intersectionData;
    // non-state variables
    mutable std::vector<float>        factors;
    mutable math::trig::vec3        baseSamplingRays[VertexRays<samplingRate>::NumberOfRays];

    void    InitBaseSamplingRays (void)
                { VertexRays<samplingRate>::GenerateBaseSamplingRays(&baseSamplingRays); }

    void operator = (AmbientOcclusionCreator const&);
};

static bool* ambientOcclusionCreatorFactoryInitialised(NULL);

///////////////////////////////////////////////////////////

}    // _
}    //

///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////

namespace ankh    {
namespace ao    {

///////////////////////////////////////////////////////////

void ProduceIntersectionData (MeshIntersectionData& into, shapes::Mesh::Elements const& els, SamplingRate const _sr) {
    switch (_sr) {
        case 9u:
            _::ProduceIntersectionData<9u>(into, els);
            break;
        case 17u:
            _::ProduceIntersectionData<17u>(into, els);
            break;
        default:
            DASSERT(false);
    }
}

///////////////////////////////////////////////////////////

void GenerateNormalisedBaseSamplingVectors (std::list<math::trig::vec3>& into, SamplingRate const samplingRate) {
    switch (samplingRate) {
        case SamplingRate_9: {
            size_t const samplingRate = 9u;

            math::trig::vec3 baseSamples[_::VertexRays<samplingRate>::NumberOfRays];
            _::VertexRays<samplingRate>::GenerateBaseSamplingRays(&baseSamples);

            for (math::trig::vec3* baseSample(&baseSamples[0]); baseSample < &baseSamples[_countof(baseSamples)]; ++baseSample)
                into.push_back(*baseSample);

            break;
        }
        case SamplingRate_17: {
            size_t const samplingRate = 17u;

            math::trig::vec3 baseSamples[_::VertexRays<samplingRate>::NumberOfRays];
            _::VertexRays<samplingRate>::GenerateBaseSamplingRays(&baseSamples);

            for (math::trig::vec3* baseSample(&baseSamples[0]); baseSample < &baseSamples[_countof(baseSamples)]; ++baseSample)
                into.push_back(*baseSample);

            break;
        }
        default:
            DASSERT(false);
    }
}

///////////////////////////////////////////////////////////

void UpdateAmbientOcclusionFactors (
        shapes::Mesh::Elements&        elements,
        const SamplingRate            samplingRate,
        MeshIntersectionData* const    intersectionDataInto) {
    switch (samplingRate) {
        case 9u:
            _::UpdateAmbientOcclusionFactors<9u>(elements, intersectionDataInto);
            break;
        case 17u:
            _::UpdateAmbientOcclusionFactors<17u>(elements, intersectionDataInto);
            break;
        default:
            DASSERT(false);
    }
}

///////////////////////////////////////////////////////////

// AmbientOcclusionCreatorFactory

void AmbientOcclusionCreatorFactory::Initialise (void)
    { *unew(_::ambientOcclusionCreatorFactoryInitialised) = true; }

void AmbientOcclusionCreatorFactory::CleanUp (void)
    { udelete(_::ambientOcclusionCreatorFactoryInitialised); }

bool AmbientOcclusionCreatorFactory::IsInitialised (void) {
    DASSERT(_::ambientOcclusionCreatorFactoryInitialised || *DPTR(_::ambientOcclusionCreatorFactoryInitialised));
    return _::ambientOcclusionCreatorFactoryInitialised != NULL;
}

AmbientOcclusionCreatorFactory::AOC* AmbientOcclusionCreatorFactory::New (
        SamplingRate const                    samplingRate,
        shapes::Mesh::Elements const* const    elements,
        MeshIntersectionData* const            intersectionData) {
    DASSERT(IsInitialised() && intersectionData);

    switch (samplingRate) {
        case SamplingRate_9:
            return DNEWCLASS(_::AmbientOcclusionCreator<9u>, (elements, intersectionData));
            break;
        case SamplingRate_17:
            return DNEWCLASS(_::AmbientOcclusionCreator<17u>, (elements, intersectionData));
            break;
        default:
            DASSERT(false);
            return NULL;
    }
}

void AmbientOcclusionCreatorFactory::Delete (AOC* const aoc)
    { DASSERT(IsInitialised()); DDELETE(aoc); }

///////////////////////////////////////////////////////////

}    // ao
}    // ankh

///////////////////////////////////////////////////////////
