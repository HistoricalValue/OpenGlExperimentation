#ifndef __DRAWING__SETUP_FRAMEBUFFERS__H__
#define __DRAWING__SETUP_FRAMEBUFFERS__H__

#include <drawing.h>
#include <drawing_buffers.h>

///////////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////////

static
void	SetupFramebuffers (DrawData& dd) {
			Gen(dd.framebuffer);
			Gen(dd.depthbuffer);
			dd.colourbuffers.Create();

			size_t const W(800), H(600);
			dd.depthbuffer->AllocateDepth(W, H);
			dd.colourbuffers.Allocate(W, H);

			dd.framebuffer->AttachColour(COLOUR_CHANNEL_RED_INDEX	, dd.colourbuffers.Red()	);
			dd.framebuffer->AttachColour(COLOUR_CHANNEL_GREEN_INDEX	, dd.colourbuffers.Green()	);
			dd.framebuffer->AttachColour(COLOUR_CHANNEL_BLUE_INDEX	, dd.colourbuffers.Blue()	);
			dd.framebuffer->AttachColour(COLOUR_CHANNEL_ALL_INDEX	, dd.colourbuffers.All()	);
			dd.framebuffer->AttachDepth(dd.depthbuffer);

			using gl::adapt::Framebuffer;

			Framebuffer::RenderbufferAttachmentParameters ps[4];
			dd.framebuffer->GetColourAttachmentParameters(ps[0], COLOUR_CHANNEL_RED_INDEX);
			dd.framebuffer->GetColourAttachmentParameters(ps[1], COLOUR_CHANNEL_GREEN_INDEX);
			dd.framebuffer->GetColourAttachmentParameters(ps[2], COLOUR_CHANNEL_BLUE_INDEX);
			dd.framebuffer->GetColourAttachmentParameters(ps[3], COLOUR_CHANNEL_ALL_INDEX);

			PASSERT((dd.framebuffer->BindForDrawing(), dd.framebuffer->IsComplete()))
			PASSERT((dd.framebuffer->BindForReading(), dd.framebuffer->IsComplete()))

			dd.framebuffer->BindForDrawing();
			dd.framebuffer->Unbind();

			dd.framebuffer->MapToShaderOutputs();
		}

static
void	CleanUpFrameBuffers (DrawData& dd) {
			dd.colourbuffers.Release();
			Del(dd.depthbuffer);
			Del(dd.framebuffer);
		}

///////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////

#endif // __DRAWING__SETUP_FRAMEBUFFERS__H__
