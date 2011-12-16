#include "stdafx.h"
#include <drawing_globalstate.h>

///////////////////////////////////////////////////////////

namespace _	{

///////////////////////////////////////////////////////////

GlobalState& getglobalstate (DrawData& dd)
	{ return dd.gstate; }

void setmesh (DrawData& dd, ankh::shapes::Mesh* const mesh)
	{ assign(getglobalstate(dd).mesh, dptr(mesh).native()); }

ankh::shapes::Mesh& getmesh (DrawData& dd)
	{ return *dptr(getglobalstate(dd).mesh); }

ankh::shapes::Mesh::Elements const& getmeshelements (DrawData& dd)
	{ return getmesh(dd).GetElements(); }

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////