#include "Private/stdafx.h"

#include "PFunctors_inl.h"

namespace u = codeshare::utilities;
namespace uptr = codeshare::utilities::pointer_utilities;

namespace _ {

	typedef ankh::opengl::adapter::Context::DeviceContextHandle		DeviceContextHandle;
	typedef ankh::opengl::adapter::Context::ResourceContextHandle	ResourceContextHandle;

	using ankh::opengl::adapter::NativeWindow;
	using ankh::opengl::adapter::ContextOptions;

	struct InitialisationArguments {
		NativeWindow const*		window;
		ContextOptions const*	contextOptions;
	}; // struct InitialisationArguments

	static bool CreateContext (DeviceContextHandle const& device, ResourceContextHandle* const out_context) {
		ResourceContextHandle& context(*out_context);

		PIXELFORMATDESCRIPTOR pfd = { 
			sizeof(PIXELFORMATDESCRIPTOR),	// size of this pfd  
			1,								// version number  
			PFD_DRAW_TO_WINDOW |			// support window  
				PFD_SUPPORT_OPENGL |		// support OpenGL  
				PFD_DOUBLEBUFFER,			// double buffered  
			PFD_TYPE_RGBA,					// RGBA type  
			24,								// 24-bit color depth  
			0, 0, 0, 0, 0, 0,				// color bits ignored  
			0,								// no alpha buffer  
			0,								// shift bit ignored  
			0,								// no accumulation buffer  
			0, 0, 0, 0,						// accum bits ignored  
			32,								// 32-bit z-buffer					// !!! GLUT_DEPTH  (used for depth testing)
			0,								// no stencil buffer  
			0,								// no auxiliary buffer  
			PFD_MAIN_PLANE,					// main layer  
			0,								// reserved  
			0, 0, 0							// layer masks ignored  
		}; 
		int  iPixelFormat;
		BOOL bresult;

		// get the best available match of pixel format for the device context   
		iPixelFormat = ChoosePixelFormat(device, &pfd); 
 

		// make that the pixel format of the device context  
		bresult = SetPixelFormat(device, iPixelFormat, &pfd);

		if (bresult == FALSE)
			return false;

		context = wglCreateContext(device);

		// Try to create a 4.1 context
		{
			typedef HGLRC (*wglCreateContextAttribsARB_func)(HDC hDC, HGLRC hShareContext, const int *attribLis);
			wglCreateContextAttribsARB_func wglCreateContextAttribsARB(reinterpret_cast<wglCreateContextAttribsARB_func>(
						wglGetProcAddress("wglCreateContextAttribsARB")));
			// TODO: needs more research, wglCreateContextAttribsARB is NULL or request of 4.1 fails (it's done wrong, aparently)
			if (wglCreateContextAttribsARB) {
				int attribs[] = {WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0};
				context = wglCreateContextAttribsARB(device, context, &attribs[0]);
			}
		}

		bresult = wglMakeCurrent(device, context);
		if (bresult == FALSE)
			return false;

		return true;
	}
} // namespace _

namespace ankh { namespace opengl { namespace adapter {

	///////////////////////////////////

	Context::Context (void):
		context	()
	{
		Initialisable* initable = this;
		initable->Initialise();

		{
			size_t const initialisable	(sizeof(Initialisable));
			size_t const context		(sizeof(Context));
			size_t const contextH		(sizeof(context));

			P_STATIC_ASSERT(context ==  initialisable + contextH)
		}
	}

	///////////////////////////////////

	Context::~Context (void) {
	}

	///////////////////////////////////

	bool Context::Initialise (NativeWindow const& window, ContextOptions const&) {
		_::InitialisationArguments args;
		
		args.window = &window;
		args.contextOptions = NULL;

		initialisationArguments = &args;

		return Initialisable::Initialise();
	}

	///////////////////////////////////

	bool Context::PerformInitialisation (void) {
		PASSERT(initialisationArguments)

		_::InitialisationArguments& args(*reinterpret_cast<_::InitialisationArguments* const>(initialisationArguments));

		return _::CreateContext(GetDC(args.window->GetWindow()), &context);
	}

	///////////////////////////////////

	void Context::PerformCleanUp (void) {
		PASSERT( wglGetCurrentContext() == context )
		wglDeleteContext(context);
	}

}}} // namespace ankh::opengl::adapter
