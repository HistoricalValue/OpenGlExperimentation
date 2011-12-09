#ifndef __DRAWING__BUFFERS__H__
#define __DRAWING__BUFFERS__H__

#include <drawing.h>

///////////////////////////////////////////////////////////

namespace _	{

///////////////////////////////////////////////////////////

template <const size_t N> static inline
void Gen (gl::adapt::Vertexbuffer* (&buffers)[N])
	{ gl::adapt::VertexbufferManager::GetSingleton().Create(buffers); }

template <const size_t N> static inline
void Del (gl::adapt::Vertexbuffer* (&buffers)[N])
	{ gl::adapt::VertexbufferManager::GetSingleton().Release(buffers); }

///////////////////////////////////////////////////////////

static inline
gl::adapt::Framebuffer* Gen (gl::adapt::Framebuffer*& fb)
	{ return gl::adapt::FramebufferManager::GetSingleton().Create(fb); }

template <const size_t N> static inline
void Gen (gl::adapt::Framebuffer* (&buffers)[N])
	{ gl::adapt::FramebufferManager::GetSingleton().Create(buffers); }

static inline
void Del (gl::adapt::Framebuffer* const fb)
	{ gl::adapt::FramebufferManager::GetSingleton().Release(fb); }

template <const size_t N> static inline
void Del (gl::adapt::Framebuffer* (&buffers)[N])
	{ gl::adapt::FramebufferManager::GetSingleton().Release(buffers); }

///////////////////////////////////////////////////////////

static inline
gl::adapt::Renderbuffer* Gen (gl::adapt::Renderbuffer*& rb)
	{ return gl::adapt::RenderbufferManager::GetSingleton().Create(rb); }

template <const size_t N> static inline
void Gen (gl::adapt::Renderbuffer* (&buffers)[N])
	{ gl::adapt::RenderbufferManager::GetSingleton().Create(buffers); }

static inline
void Del (gl::adapt::Renderbuffer* const rb)
	{ gl::adapt::RenderbufferManager::GetSingleton().Release(rb); }

template <const size_t N> static inline
void Del (gl::adapt::Renderbuffer* (&buffers)[N])
	{ gl::adapt::RenderbufferManager::GetSingleton().Release(buffers); }

///////////////////////////////////////////////////////////

template <const size_t N> static inline
void Gen (gl::adapt::VertexArray* (&buffers)[N])
	{ gl::adapt::VertexArrayManager::GetSingleton().Create(buffers); }

template <const size_t N> static inline
void Del (gl::adapt::VertexArray* (&buffers)[N])
	{ gl::adapt::VertexArrayManager::GetSingleton().Release(buffers); }

///////////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////////

#endif // __DRAWING__BUFFERS__H__
