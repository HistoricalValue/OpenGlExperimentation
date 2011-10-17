#ifndef DRAWING_NURBS_H
#define DRAWING_NURBS_H

#include <my/gl/shapes/ShapeCompositionFactory.h>
#include <SurfacesTools.h>
#include <Surface.h>
#include <ComputeMeshAmbientOcclusion.h>

namespace my { namespace drawing { namespace nurbs {

extern void Initialise (void);
extern void CleanUp (void);

extern ankh::nurbs::Surface const	BobRoss (void);

// after tesselation, the following group operates on the produced mesh
extern void tesselate (ankh::nurbs::Surface const& surf, ankh::nurbs::TesselationParameters const* const = NULL);
//
extern void generateindexedbuffer (void);
extern void computeboundinvolume (void);
extern void generateaabb (ankh::shapes::MeshAABBTree&);
extern void fairprepareao (void);
extern void updateao (ankh::shapes::Mesh::AmbientOcclusionCreator const&, char const* method = NULL);
extern void updateaotraditional (ankh::ao::ProduceIntersectionData_Into& into);
//
extern void store (char const* id);
extern void load (char const* id);
extern void loadinto (char const* id, ankh::shapes::Mesh&);
//
extern void addnormalsto (my::gl::shapes::ShapeCompositionFactory& f);
extern void addastrianglesto (my::gl::shapes::ShapeCompositionFactory& f);
//
extern unsigned long int	GetNumberOfMeshElements (void);
extern void					LogInfo_MeshStats (void);
extern void					ExtractAOInto (std::vector<float>&);
//
typedef std::list<std::pair<std::string, unsigned long> >	TimesList;
extern void SetTimesList (TimesList*);
extern void UnsetTimesList (void);

}}} // my::drawing::nurbs

#endif // DRAWING_NURBS_H
