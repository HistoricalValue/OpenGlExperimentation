#include "stdafx.h"
#include <drawing_setup_framebuffers.h>

///////////////////////////////////////////////////////////

namespace _ {

///////////////////////////////////////////////////////////

static inline bool completeAndIfNotWhy (gl::adapt::Framebuffer& fb) {
    using gl::adapt::Framebuffer;
    bool const c = fb.IsComplete();

    if (!c) {
        Framebuffer::IncompletionReason const r = fb.GetIncompletionReason();

        std::vector<Framebuffer::RenderbufferAttachmentParameters> params;
        for (size_t i(0u); i < gl::prim::Framebuffer::GetMaxColourAttachments(); ++i)
            params.push_back(fb.GetColourAttachmentParameters(i));

        PASSERT(false)
    }

    return c;
}

void    SetupFramebuffers (DrawData& dd) {
            Gen(dd.framebuffer);
            Gen(dd.depthbuffer);
            dd.colourbuffers.Create();

            size_t const W(800), H(600);
            dd.depthbuffer->AllocateDepth(W, H);
            dd.colourbuffers.Allocate(W, H);

            dd.framebuffer->AttachColour(COLOUR_CHANNEL_RED_INDEX    , dd.colourbuffers.Red()    );
            dd.framebuffer->AttachColour(COLOUR_CHANNEL_GREEN_INDEX    , dd.colourbuffers.Green()    );
            dd.framebuffer->AttachColour(COLOUR_CHANNEL_BLUE_INDEX    , dd.colourbuffers.Blue()    );
            dd.framebuffer->AttachColour(COLOUR_CHANNEL_ALL_INDEX    , dd.colourbuffers.All()    );
            dd.framebuffer->AttachDepth(dd.depthbuffer);

            using gl::adapt::Framebuffer;

            Framebuffer::RenderbufferAttachmentParameters ps[4];
            dd.framebuffer->GetColourAttachmentParameters(ps[0], COLOUR_CHANNEL_RED_INDEX);
            dd.framebuffer->GetColourAttachmentParameters(ps[1], COLOUR_CHANNEL_GREEN_INDEX);
            dd.framebuffer->GetColourAttachmentParameters(ps[2], COLOUR_CHANNEL_BLUE_INDEX);
            dd.framebuffer->GetColourAttachmentParameters(ps[3], COLOUR_CHANNEL_ALL_INDEX);

            dd.framebuffer->MapToShaderOutputs();

            PASSERT((
                dd.framebuffer->BindForDrawing(),
                completeAndIfNotWhy(*dd.framebuffer)
            ))
            PASSERT((
                dd.framebuffer->BindForReading(),
                completeAndIfNotWhy(*dd.framebuffer)
            ))

            dd.framebuffer->SetBindingForDrawing();
        }

void    CleanUpFrameBuffers (DrawData& dd) {
            dd.colourbuffers.Release();
            Del(dd.depthbuffer);
            Del(dd.framebuffer);
        }

///////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////
