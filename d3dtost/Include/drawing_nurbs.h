#ifndef DRAWING_NURBS_H
#define DRAWING_NURBS_H

#include <my/gl/shapes/ShapeCompositionFactory.h>


namespace my { namespace drawing { namespace nurbs {

extern void Initialise (void);
extern void CleanUp (void);

extern void tesselate (void);
extern void load (std::string const& path);

extern void addastrianglesto (my::gl::shapes::ShapeCompositionFactory&	f);
extern void addnormalsto (my::gl::shapes::ShapeCompositionFactory& f);

}}} // my::drawing::nurbs

#endif // DRAWING_NURBS_H
