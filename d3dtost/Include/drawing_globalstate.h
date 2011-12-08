#ifndef __DRAWING__GLOBAL_STATE__H__
#define __DRAWING__GLOBAL_STATE__H__

#include <drawing.h>

///////////////////////////////////////////////////////////

namespace _	{

///////////////////////////////////////////////////////////

static inline GlobalState& getglobalstate (DrawData& dd)
	{ return dd.gstate; }

static inline void setmesh (DrawData& dd, ankh::shapes::Mesh* const mesh)
	{ assign(getglobalstate(dd).mesh, dptr(mesh).native()); }

static inline ankh::shapes::Mesh& getmesh (DrawData& dd)
	{ return *dptr(getglobalstate(dd).mesh); }

static inline ankh::shapes::Mesh::Elements const& getmeshelements (DrawData& dd)
	{ return getmesh(dd).GetElements(); }

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////

#endif // __DRAWING__GLOBAL_STATE__H__
