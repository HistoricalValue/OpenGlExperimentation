#ifndef DRAWING_NURBS_H
#define DRAWING_NURBS_H

#include <my/gl/shapes/ShapeCompositionFactory.h>
#pragma warning( push, 0 )
#	include "Mesh.h"
#pragma warning( pop )

namespace my { namespace drawing { namespace nurbs {

using namespace ankh;

extern void Initialise (void);
extern void CleanUp (void);

extern ankh::shapes::Mesh* load (char const* const id);
extern void unload (ankh::shapes::Mesh* mesh);

extern void addnormalsto (ankh::shapes::Mesh::Elements const& elements, my::gl::shapes::ShapeCompositionFactory& f);
extern void addastrianglesto (ankh::shapes::Mesh::Elements const& elements, my::gl::shapes::ShapeCompositionFactory& f);

}}} // my::drawing::nurbs

#endif // DRAWING_NURBS_H
