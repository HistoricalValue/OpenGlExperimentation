#ifdef __ANKH__OPEN_GL_ADAPTER__CONTEXT__H__BEING_INCLUDED__
#	error "Circular inclusion"
#endif
#define __ANKH__OPEN_GL_ADAPTER__CONTEXT__H__BEING_INCLUDED__

#ifndef __ANKH__OPEN_GL_ADAPTER__CONTEXT__H__INCLUDED__
#define __ANKH__OPEN_GL_ADAPTER__CONTEXT__H__INCLUDED__

#include "ankh/opengl/adapter/Context_fwd.h"

#include "PInitialisation.h"

#include "ankh/opengl/adapter/NativeWindow.h"
#include "ankh/opengl/adapter/ContextOptions.h"

namespace ankh { namespace opengl { namespace adapter {

	using codeshare::utilities::Initialisable;
	
	class ANKH__OPENGL_ADAPTER__API Context: private Initialisable {
	public:
#if defined(__ANKH__WIN32__)
		typedef HGLRC									ResourceContextHandle;
		typedef HDC										DeviceContextHandle;
#elif defined(__ANKH__LINUX__)
#	error "Unconfigured!"
#else
#	error "What platform?"
#endif

														Context (void);
		virtual											~Context (void);
													

		bool											Initialise (NativeWindow const&, ContextOptions const&);
		void											CleanUp (void);

	protected:
		virtual bool									PerformInitialisation (void);
		virtual void									PerformCleanUp (void);

	private:
		ResourceContextHandle							context;
	}; // class Context


}}} // namespace ankh::opengl::adapter

#endif //		__ANKH__OPEN_GL_ADAPTER__CONTEXT__H__INCLUDED__
#undef			__ANKH__OPEN_GL_ADAPTER__CONTEXT__H__BEING_INCLUDED__
