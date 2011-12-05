#ifndef __DRAWING__SETUP_FRAMEBUFFERS__H__
#define __DRAWING__SETUP_FRAMEBUFFERS__H__

#include <drawing.h>

///////////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////////

static
void	SetupFramebuffers (DrawData& dd) {
			dd.framebuffer = gl::adapt::FramebufferManager::GetSingleton().Create();
			dd.depthbuffer = gl::adapt::RenderbufferManager::GetSingleton().Create();
			dd.colourbuffers.Create();
		}

static
void	CleanUpFrameBuffers (DrawData& dd) {
			dd.colourbuffers.Release();
			gl::adapt::RenderbufferManager::GetSingleton().Release(dd.depthbuffer);
			gl::adapt::FramebufferManager::GetSingleton().Release(dd.framebuffer);
		}

///////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////

#endif // __DRAWING__SETUP_FRAMEBUFFERS__H__
