#ifndef __DRAWING__SETUP_FRAMEBUFFERS__H__
#define __DRAWING__SETUP_FRAMEBUFFERS__H__

#include <drawing.h>
#include <MyUtils.h>

///////////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////////

static
void	SetupFramebuffers (DrawData& dd) {
			dd.framebuffer = gl::adapt::FramebufferManager::GetSingleton().Create();
			dd.depthbuffer = gl::adapt::RenderbufferManager::GetSingleton().Create();
			dd.colourbuffers.Create();

			size_t const W(800), H(600);
			dd.depthbuffer->AllocateDepth(W, H);
			dd.colourbuffers.Allocate(W, H);

			dd.framebuffer->AttachColour(COLOUR_CHANNEL_RED_INDEX	, dd.colourbuffers.Red()	);
			dd.framebuffer->AttachColour(COLOUR_CHANNEL_GREEN_INDEX	, dd.colourbuffers.Green()	);				
			dd.framebuffer->AttachColour(COLOUR_CHANNEL_BLUE_INDEX	, dd.colourbuffers.Blue()	);
			dd.framebuffer->AttachColour(COLOUR_CHANNEL_ALL_INDEX	, dd.colourbuffers.All()	);
			dd.framebuffer->AttachDepth(dd.depthbuffer);
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
