#pragma once

#include <PWindows.h>

namespace my {

	class Window {
	public:
		typedef WNDCLASSEX								WindowClassInfo;
		typedef HINSTANCE								ModuleHandle;
		typedef HICON									IconHandle;
		typedef HCURSOR									CursorHandle;
		typedef HBRUSH									BrushHandle;
		typedef ATOM									Atom;
		typedef HWND									WindowHandle;

														Window (ModuleHandle);
														~Window (void);

		bool											IsValid (void) const;

		void											MainLoop (TCHAR* command_line, int show_command);
		void											SetMainLoopCallback (void (*cb) (void*), void* data = NULL);


	private:
		const ModuleHandle								module_handle;
		const IconHandle								application_icon_handle;
		const CursorHandle								application_cursor_handle;
		const BrushHandle								background_brush_handle;

		WindowClassInfo									window_class_info;
		Atom											window_class;
		WindowHandle									window;

		void											(*main_loop_callback) (void*);
		void*											main_loop_callback_data;

		friend class OpenGL;

	}; // class Window

} // namespace my