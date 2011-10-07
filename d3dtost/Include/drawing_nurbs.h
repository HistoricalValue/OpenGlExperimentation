#ifndef DRAWING_NURBS_H
#define DRAWING_NURBS_H

#include <my/gl/shapes/ShapeCompositionFactory.h>
#include <SurfacesTools.h>

namespace my { namespace drawing { namespace nurbs {

extern void Initialise (void);
extern void CleanUp (void);

extern void tesselate (ankh::nurbs::TesselationParameters const* const = NULL);
extern void load (char const* id);

extern void addastrianglesto (my::gl::shapes::ShapeCompositionFactory& f);
extern void addnormalsto (my::gl::shapes::ShapeCompositionFactory& f);

}}} // my::drawing::nurbs

#endif // DRAWING_NURBS_H
