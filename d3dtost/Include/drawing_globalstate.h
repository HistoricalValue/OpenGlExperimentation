#ifndef __DRAWING__GLOBAL_STATE__H__
#define __DRAWING__GLOBAL_STATE__H__

#include <drawing.h>

///////////////////////////////////////////////////////////

namespace _	{

///////////////////////////////////////////////////////////

extern GlobalState&							getglobalstate (DrawData& dd);
extern void									setmesh (DrawData& dd, ankh::shapes::Mesh* const mesh);
extern ankh::shapes::Mesh&					getmesh (DrawData& dd);
extern ankh::shapes::Mesh::Elements const&	getmeshelements (DrawData& dd);

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////

#endif // __DRAWING__GLOBAL_STATE__H__
