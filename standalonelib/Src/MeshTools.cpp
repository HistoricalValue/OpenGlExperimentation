// MeshTools.h
// See header file for further info.
//
// MMOG Project
// Nikos Mouchtaris, 20 October 2011

#include "MeshTools.h"
#include <MyUtils.h>
#include <PStaticAssert.h>

///////////////////////////////////////////////////////////

namespace ankh_x    {
namespace shapes    {

///////////////////////////////////////////////////////////

// NormalAndWindingInverserMeshElementProcessor

void WindingInverserMeshElementProcessor::operator () (MeshElement& elem) const {
    DASSERT(elem.HasNormals());
    DASSERT(elem.HasTextureCoords());

    std::swap(elem.b, elem.c);
    std::swap(elem.GetTextureCoords(1), elem.GetTextureCoords(2));

    math::trig::vec3 nb(elem.GetNormal(1)), nc(elem.GetNormal(2));
    std::swap(nb, nc);

    elem.SetNormal(0, -elem.GetNormal(0));
    elem.SetNormal(1, -nb);
    elem.SetNormal(2, -nc);
}

///////////////////////////////////////////////////////////
//MeshTimingStats

static const MeshStats::Timing AllTimings[MeshStats::NumberOfTimings] = {
    MeshStats::Tesselation            ,    //  0u
    MeshStats::BarycentricFactors    ,    //  1u
    MeshStats::BoundingVolume        ,    //  2u
    MeshStats::Aabb                    ,    //  3u
    MeshStats::Update                ,    //  4u
    MeshStats::IndexBuffer            ,    //  5u
    MeshStats::StoreBin                ,    //  6u
    MeshStats::StoreText            ,    //  7u
};

class MeshStatsHelper {
public:
    typedef nmutripletypes<MeshStats::timing_t&, bool&, char const* const> TTs;
    NMUTRIPLES(TimingEntry, TTs, value, timed, name)

    template <typename EntryType, typename MeshStatsType>
    static
    EntryType GetTimingEntry (MeshStatsType& mt, MeshStats::Timing const timing);

    static
    TimingEntry            Get (MeshStats& mt, MeshStats::Timing const timing)
                            { return GetTimingEntry<TimingEntry>(mt, timing); }
    static
    ConstTimingEntry    Get (MeshStats const& mt, MeshStats::Timing const timing)
                            { return GetTimingEntry<ConstTimingEntry>(mt, timing); }
};

template <typename EntryType, typename MeshStatsType>
static inline
EntryType MeshStatsHelper::GetTimingEntry (MeshStatsType& mt, MeshStats::Timing const timing) {
    DASSERT(MeshStats::IsValidTiming(size_t(timing)));

    switch (timing) {
        case MeshStats::Tesselation            : return EntryType(mt.valueOfTesselation        , mt.timedTesselation            , "Tesselation"            ); break;
        case MeshStats::BarycentricFactors    : return EntryType(mt.valueOfBarycentricFactors    , mt.timedBarycentricFactors    , "BarycentricFactors"    ); break;
        case MeshStats::BoundingVolume        : return EntryType(mt.valueOfBoundingVolume        , mt.timedBoundingVolume        , "BoundingVolume"        ); break;
        case MeshStats::Aabb                : return EntryType(mt.valueOfAabb                , mt.timedAabb                    , "Aabb"                ); break;
        case MeshStats::Update                : return EntryType(mt.valueOfUpdate                , mt.timedUpdate                , "Update"                ); break;
        case MeshStats::IndexBuffer            : return EntryType(mt.valueOfIndexBuffer        , mt.timedIndexBuffer            , "IndexBuffer"            ); break;
        case MeshStats::StoreBin            : return EntryType(mt.valueOfStoreBin            , mt.timedStoreBin                , "StoreBin"            ); break;
        case MeshStats::StoreText            : return EntryType(mt.valueOfStoreText            , mt.timedStoreText                , "StoreText"            ); break;
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
    PASSERT(AreAllTimed() || IsNoneTimed())

    TimeUpdateNotifee* const notis(notifee);
    reset(this);
    notifee = notis;

    return *this;
}

bool MeshStats::AreAllTimed (void) const {
    FOREACH(const MeshStats::Timing, AllTimings, timing)
        if (!IsTimed(*timing))
            return false;
    return true;
}

bool MeshStats::IsNoneTimed (void) const {
    FOREACH(const MeshStats::Timing, AllTimings, timing)
        if (IsTimed(*timing))
            return false;
    return custom.empty();
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

void MeshStats::Reset (Timing const timing) {
    MeshStatsHelper::TimingEntry entry(MeshStatsHelper::Get(*this, timing));

    PASSERT(entry.timed)
    entry.timed = false;
    entry.value = ULONG_MAX;
    DNULLCHECK(notifee)->TimingRestarted(entry.name);
}

void MeshStats::AddCustomTiming (ConstCustom::T1ref what, ConstCustom::T2ref timing)
    { custom.push_back(ConstCustom(what, timing)); }

std::list<std::string>& MeshStats::WriteLinesTo (std::list<std::string>& at) const {
    static const char            LongestMessage[] = "Update mesh with elements (and compute AO)";
    static const char* const    Descriptions[NumberOfTimings] = {
        "Mesh tesselation",
        "Compute barycentric factors",
        "Compute bounding volume",
        "Compute AABB Mesh Tree",
        &LongestMessage[0],
        "Compute index buffer",
        "Store binary mesh",
        "Store text mesh",
    };

    size_t maxLength(countof(LongestMessage));
    IFOREACH(std::list<ConstCustom>::const_iterator, custom, cust)
        maxLength = std::max(maxLength, cust->what.length());

    PASSERT(maxLength < 500 && AreAllTimed())

    FOREACH(MeshStats::Timing const, AllTimings, timing)
        at.push_back(format("%*s : %ld", maxLength, Descriptions[size_t(*timing)], (*this)[*timing]));

    IFOREACH(std::list<ConstCustom>::const_iterator, custom, cust)
        at.push_back(format("%*s : %ld", maxLength, ucstringarg(cust->what), cust->time));

    // and a blank
    at.push_back(std::string());

    return at;
}

MeshStats::MeshStats (void):
    valueOfTesselation                (ULONG_MAX    ),    //  0u
    valueOfBarycentricFactors        (ULONG_MAX    ),    //  1u
    valueOfBoundingVolume            (ULONG_MAX    ),    //  2u
    valueOfAabb                        (ULONG_MAX    ),    //  3u
    valueOfUpdate                    (ULONG_MAX    ),    //  4u
    valueOfIndexBuffer                (ULONG_MAX    ),    //  5u
    valueOfStoreBin                    (ULONG_MAX    ),    //  6u
    valueOfStoreText                (ULONG_MAX    ),    //  7u
    timedTesselation                (false        ),    //  0u
    timedBarycentricFactors            (false        ),    //  1u
    timedBoundingVolume                (false        ),    //  2u
    timedAabb                        (false        ),    //  3u
    timedUpdate                        (false        ),    //  4u
    timedIndexBuffer                (false        ),    //  5u
    timedStoreBin                    (false        ),    //  6u
    timedStoreText                    (false        ),    //  7u
    notifee                            (NULL        ),
    custom                            (            )
    {}

MeshStats::MeshStats (MeshStats const& other):
    valueOfTesselation                (other.valueOfTesselation            ),    //  0u
    valueOfBarycentricFactors        (other.valueOfBarycentricFactors    ),    //  1u
    valueOfBoundingVolume            (other.valueOfBoundingVolume        ),    //  2u
    valueOfAabb                        (other.valueOfAabb                    ),    //  3u
    valueOfUpdate                    (other.valueOfUpdate                ),    //  4u
    valueOfIndexBuffer                (other.valueOfIndexBuffer            ),    //  5u
    valueOfStoreBin                    (other.valueOfStoreBin                ),    //  6u
    valueOfStoreText                (other.valueOfStoreText                ),    //  7u
    timedTesselation                (other.timedTesselation                ),    //  0u
    timedBarycentricFactors            (other.timedBarycentricFactors        ),    //  1u
    timedBoundingVolume                (other.timedBoundingVolume            ),    //  2u
    timedAabb                        (other.timedAabb                    ),    //  3u
    timedUpdate                        (other.timedUpdate                    ),    //  4u
    timedIndexBuffer                (other.timedIndexBuffer                ),    //  5u
    timedStoreBin                    (other.timedStoreBin                ),    //  6u
    timedStoreText                    (other.timedStoreText                ),    //  7u
    notifee                            (other.notifee                        ),
    custom                            (other.custom                        )
    { PASSERT(AreAllTimed() || IsNoneTimed()) }

MeshStats::~MeshStats (void)
    { PASSERT(AreAllTimed() || IsNoneTimed()) }

///////////////////////////////////////////////////////////

void FlushTo (FILE* const fout, const std::list<std::string>& lines) {
    IFOREACH(std::list<std::string>::const_iterator, lines, i)
        fprintf(fout, "%s\n", ucstringarg(*i));
}

///////////////////////////////////////////////////////////

// MeshIndex
struct MeshIndexPlaceholder { char data[sizeof(MeshIndex)]; };\
static MeshIndexPlaceholder*    meshIndexSingleton(NULL);

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
    typedef std::map<std::string, Mesh*>                MeshesIdMap;
    typedef std::pair<MeshesEntries::iterator, bool>    MeshesEntriesInsertion;

    MeshLoader&                                Loader(MeshLoader::GetSingleton());
    MeshesIdMap const&                        allMeshes(Loader.GetAll());

    IFOREACH(MeshesIdMap::const_iterator, allMeshes, meshPair) {
        Mesh* const            mesh(meshPair->second);
        std::string const&    id    (meshPair->first);

        Entry newEntry(Loader.GetPath(mesh));
        MeshesEntriesInsertion insertion(meshes.insert(std::make_pair(id, newEntry)));
        // Must be fresh, otherwise data must agree
        PASSERT(insertion.second || insertion.first->first == id && insertion.first->second == newEntry);
    }
}

void MeshIndex::ExportAllToMeshLoader (void) {
    MeshLoader& Loader(MeshLoader::GetSingleton());

    IFOREACH(MeshesEntries::const_iterator, meshes, entryPair) {
        Dptr<Mesh> mesh(Loader.Load(entryPair->second.path));
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

            PortableBinFileReader    r(fin);
            bool                    ok;

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
    Entry    loading;

    {
        bool    ok;

        ok = r.read(loading.path, false);
        DASSERT(ok);
    }

    *this = loading;
}

///////////////////////////////////////////////////////////

std::list<std::string>& MeshInfo::WriteLinesTo (std::list<std::string>& into) const {
    into.push_back(format("\n\n === Mesh : %s === ", ucstringarg(meshId)));
    into.push_back(format(" with step=%5.3f => number of triangles: %lu", step, numberOfElements));
    into.push_back(format(" bounded by <%3.1f, %3.1f, %3.1f> - <%3.1f, %3.1f, %3.1f>", min.x, min.y, min.z, max.x, max.y, max.z));
    into.push_back(" generation statistics:");
    return stats.WriteLinesTo(into);
}

static inline math::trig::vec3 MakeLimitFrom (shapes::volumes::BoundingBox const& box, float const& (*cmp) (float const&, float const&)) {
    math::trig::vec3 const    a(box.GetTopLeftFront()),
                            b(box.GetBottomRightBack());
    return math::trig::vec3((*cmp)(a.x, b.x), (*cmp)(a.y, b.y), (*cmp)(a.z, b.z));
}
static inline math::trig::vec3 MakeMinFrom (shapes::volumes::BoundingBox const& box)
    { return MakeLimitFrom(box, &std::min<float>); }
static inline math::trig::vec3 MakeMaxFrom (shapes::volumes::BoundingBox const& box)
    { return MakeLimitFrom(box, &std::max<float>); }

// MeshInfo
MeshInfo::MeshInfo (Mesh const& mesh, float const _step, MeshStats const& _stats):
    meshId                (mesh.GetUniqueId()                                                    ),
    stats                (_stats                                                                ),
    min                    (MakeMinFrom(DPTR(DNULLCHECK(mesh.GetBoundingVolume()))->GetBox())    ),
    max                    (MakeMaxFrom(DPTR(DNULLCHECK(mesh.GetBoundingVolume()))->GetBox())    ),
    numberOfElements    (mesh.GetElements().size()                                            ),
    step                (_step                                                                )
    {}

MeshInfo::MeshInfo (MeshInfo const& o):
    meshId                (o.meshId            ),
    stats                (o.stats            ),
    min                    (o.min                ),
    max                    (o.max                ),
    numberOfElements    (o.numberOfElements    ),
    step                (o.step                )
    {}

//
//    at.push_back(format(" ==== Generation statistic for step = %3.1f ==== ", step));
//    at.push_back(format(" Number of triangles: %lu ", numberOfElements));

///////////////////////////////////////////////////////////

}    // shapes
}    // ankh_x

///////////////////////////////////////////////////////////
