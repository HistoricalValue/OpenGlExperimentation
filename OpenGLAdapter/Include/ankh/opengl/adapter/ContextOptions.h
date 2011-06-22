#ifdef __ANKH__OPEN_GL_ADAPTER__CONTEXT_OPTIONS__H__BEING_INCLUDED__
#	error "Circular inclusion"
#endif
#define __ANKH__OPEN_GL_ADAPTER__CONTEXT_OPTIONS__H__BEING_INCLUDED__

#ifndef __ANKH__OPEN_GL_ADAPTER__CONTEXT_OPTIONS__H__INCLUDED__
#define __ANKH__OPEN_GL_ADAPTER__CONTEXT_OPTIONS__H__INCLUDED__

#include "ankh/OpenGlAdapterDSO.h"

namespace ankh { namespace opengl { namespace adapter {

	class ANKH__OPENGL_ADAPTER__API ContextOptions {
	public:
														ContextOptions (void);
		virtual											~ContextOptions (void);

		// Nothing, placeholder. Default options are used internally in Context.
	}; // class Context

}}} // namespace ankh::opengl::adapter


#endif //	__ANKH__OPEN_GL_ADAPTER__CONTEXT_OPTIONS__H__INCLUDED__
#undef		__ANKH__OPEN_GL_ADAPTER__CONTEXT_OPTIONS__H__BEING_INCLUDED__

