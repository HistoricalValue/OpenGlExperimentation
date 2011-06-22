#ifdef __ANKH__OPEN_GL_ADAPTER__NATIVE_WINDOW__H__BEING_INCLUDED__
#	error "Circular inclusion"
#endif
#define __ANKH__OPEN_GL_ADAPTER__NATIVE_WINDOW__H__BEING_INCLUDED__

#ifndef __ANKH__OPEN_GL_ADAPTER__NATIVE_WINDOW__H__INCLUDED__
#define __ANKH__OPEN_GL_ADAPTER__NATIVE_WINDOW__H__INCLUDED__

#include "ankh/config.h"
#include "ankh/OpenGlAdapterDSO.h"

namespace ankh { namespace opengl { namespace adapter {
	
	class ANKH__OPENGL_ADAPTER__API NativeWindow {
	public:
		typedef struct {} *								MainLoopCallbackClosure;
		typedef void									(*MainLoopCallback) (MainLoopCallbackClosure const&);

		typedef LPCSTR									CommandLineArguments;
		typedef struct { int command; }					ShowCommand;

		typedef TCHAR									ErrorReasonText[1 << 10]; //1 KiB

#if defined( __ANKH__WIN32__ )
		typedef WNDCLASSEX								WindowClassInfo;
		typedef HINSTANCE								ModuleHandle;
		typedef HICON									IconHandle;
		typedef HCURSOR									CursorHandle;
		typedef HBRUSH									BrushHandle;
		typedef ATOM									Atom;
		typedef HWND									WindowHandle;
#elif defined( __ANKH__LINUX__ )
#	error "Unconfigured!"
#else
#	error "What platform?"
#endif

		struct ConstructorArguments {
			ModuleHandle								moduleHandle;
			CommandLineArguments						commandLineArguments;
			ShowCommand									showCommand;
		};
														NativeWindow (ConstructorArguments const&);
														~NativeWindow (void);

		bool											IsValid (void) const;
		ErrorReasonText const&							GetErrorReason (void) const;

		WindowHandle const&								GetWindow (void) const;


		void											SetMainLoopCallback (MainLoopCallback const&);
		MainLoopCallback const&							GetMainLoopCallback (void) const;
		void											SetMainLoopCallbackClosure (MainLoopCallbackClosure const&);
		MainLoopCallbackClosure const&					GetMainLoopCallbackClosure (void) const;

		void											MainLoop (void);

	private:
		ModuleHandle const 								moduleHandle;
		IconHandle	 const 								applicationIconHandle;
		CursorHandle const 								applicationCursorHandle;
		BrushHandle	 const 								backgroundBrushHandle;

		WindowClassInfo									windowClassInfo;
		Atom											windowClass;
		WindowHandle									window;

		MainLoopCallback								mainLoopCallback;
		MainLoopCallbackClosure							mainLoopCallbackClosure;

		ErrorReasonText									errorReasonText;

		CommandLineArguments							commandLineArguments;
		ShowCommand										showCommand;

		void											StoreNativeError (LPCTSTR);
	}; // class NativeWindow


}}} // namespace ankh::opengl::adapter


#endif //	__ANKH__OPEN_GL_ADAPTER__NATIVE_WINDOW__H__INCLUDED__
#undef		__ANKH__OPEN_GL_ADAPTER__NATIVE_WINDOW__H__BEING_INCLUDED__

