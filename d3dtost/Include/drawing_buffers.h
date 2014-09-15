#ifndef __DRAWING__BUFFERS__H__
#define __DRAWING__BUFFERS__H__

#include <drawing.h>

///////////////////////////////////////////////////////////

namespace _            {

///////////////////////////////////////////////////////////

template <const size_t N> static inline
void Gen (gl::adapt::Vertexbuffer* (&buffers)[N])
    { gl::adapt::VertexbufferManager::GetSingleton().Create(buffers); }

template <const size_t N> static inline
void Del (gl::adapt::Vertexbuffer* (&buffers)[N])
    { gl::adapt::VertexbufferManager::GetSingleton().Release(buffers); }

///////////////////////////////////////////////////////////

extern
gl::adapt::Framebuffer* Gen (gl::adapt::Framebuffer*& fb);

template <const size_t N> static inline
void Gen (gl::adapt::Framebuffer* (&buffers)[N])
    { gl::adapt::FramebufferManager::GetSingleton().Create(buffers); }

extern
void Del (gl::adapt::Framebuffer* const fb);

template <const size_t N> static inline
void Del (gl::adapt::Framebuffer* (&buffers)[N])
    { gl::adapt::FramebufferManager::GetSingleton().Release(buffers); }

///////////////////////////////////////////////////////////

extern
gl::adapt::Renderbuffer* Gen (gl::adapt::Renderbuffer*& rb);

template <const size_t N> static inline
void Gen (gl::adapt::Renderbuffer* (&buffers)[N])
    { gl::adapt::RenderbufferManager::GetSingleton().Create(buffers); }

extern
void Del (gl::adapt::Renderbuffer* const rb);

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

}    //    _

///////////////////////////////////////////////////////////

#endif // __DRAWING__BUFFERS__H__
