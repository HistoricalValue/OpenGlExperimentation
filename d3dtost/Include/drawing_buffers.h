#ifndef __DRAWING__BUFFERS__H__
#define __DRAWING__BUFFERS__H__

#include <drawing.h>

///////////////////////////////////////////////////////////

namespace _	{

///////////////////////////////////////////////////////////

template <const size_t N> static inline
void GenBuffers (my::gl::adapters::Buffer* (&buffers)[N])
	{ my::gl::adapters::BufferManager::GetSingleton().Create(buffers); }

template <const size_t N> static inline
void DeleteBuffers (my::gl::adapters::Buffer* (&buffers)[N])
	{ my::gl::adapters::BufferManager::GetSingleton().Release(buffers); }

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////

#endif // __DRAWING__BUFFERS__H__
