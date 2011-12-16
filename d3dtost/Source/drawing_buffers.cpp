#include "stdafx.h"
#include <drawing_buffers.h>

///////////////////////////////////////////////////////////

namespace _			{

///////////////////////////////////////////////////////////

gl::adapt::Framebuffer* Gen (gl::adapt::Framebuffer*& fb)
	{ return gl::adapt::FramebufferManager::GetSingleton().Create(fb); }

void Del (gl::adapt::Framebuffer* const fb)
	{ gl::adapt::FramebufferManager::GetSingleton().Release(fb); }

///////////////////////////////////////////////////////////

gl::adapt::Renderbuffer* Gen (gl::adapt::Renderbuffer*& rb)
	{ return gl::adapt::RenderbufferManager::GetSingleton().Create(rb); }

void Del (gl::adapt::Renderbuffer* const rb)
	{ gl::adapt::RenderbufferManager::GetSingleton().Release(rb); }

///////////////////////////////////////////////////////////

}	// _

///////////////////////////////////////////////////////////