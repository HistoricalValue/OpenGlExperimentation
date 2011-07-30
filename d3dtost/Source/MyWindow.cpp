#include "stdafx.h"


#define __LOG_WIN_ERROR(ERRMSG)												\
	{																		\
		DWORD const		errcode(GetLastError());							\
		LPTSTR const	errstring(my::winutil::ErrorToString(errcode));		\
		my::global::logger::Get().Error(my::String(_T(ERRMSG)) + errstring);\
		my::winutil::ReleaseErrorString(errstring);							\
	}																		\


namespace {

	static void (*__main_loop_callback) (void*) = NULL;
	static void* __main_loop_callback_env = NULL;
	static bool __window_destroyed = false;

	// http://msdn.microsoft.com/en-us/library/ms633573(v=VS.85).aspx
	// WindowProc
	// MSDN Library/Windows Development/Windows Application UI Development/Windows and Messages/
	//     Window Procedures/Window Procedure Reference/Window Procedure Functions/WindowProc
	static LRESULT CALLBACK MyWindowMessageProcessor (
		__in  HWND hWnd,
		__in  UINT uMsg,
		__in  WPARAM wParam,
		__in  LPARAM lParam
	) {
		LRESULT result;
		enum my::winutil::MessageType msg(static_cast<enum my::winutil::MessageType>(uMsg));
		PASSERT(static_cast<UINT>(msg) == uMsg);

		if (my::winutil::IsUnknownMessageType(uMsg)) {
			int nothing(0xbeac0ul);
		}


		switch (msg) {
			case my::winutil::MY_WM_DESTROY: {
				PostQuitMessage(0);
				__window_destroyed = true;
				result = 0;
				break;
			}
			case my::winutil::MY_WM_PAINT: {
				if (__main_loop_callback)
					(*__main_loop_callback)(__main_loop_callback_env);

				result = 0;
				break;
			}
			case my::winutil::MY_WM_ERASEBKGND: {
				result = 0;
				break;
			}
			default: {
				result = DefWindowProc(hWnd, uMsg, wParam, lParam);
				break;
			}
		}

		return result;
	}

	namespace _ {
		static void										FillMyWindowClass (my::Window::WindowClassInfo* wclass);
		static void										RegisterMyWindowClass (my::Window::WindowClassInfo* wclass);
		static void										CreateMyWindow (
																my::Window::Atom const _window_class,
																my::Window::ModuleHandle const _module_handle,
																my::Window::WindowHandle* const _window_handle_ptr);
		static bool										DestroyMyWindow (my::Window::WindowHandle* const window_handle_tr);

		static my::Window::IconHandle					GetApplicationIcon (my::Window::ModuleHandle const&);
		static my::Window::CursorHandle					GetApplicationCursor (my::Window::ModuleHandle const&);
		static my::Window::BrushHandle					GetBackgroundBrush (void);
	}

	namespace _ {
	void FillMyWindowClass (
			my::Window::WindowClassInfo* const	wclass,
			my::Window::ModuleHandle const&		hInstance,
			my::Window::IconHandle const&		hIcon,
			my::Window::CursorHandle const&		hCursor,
			my::Window::BrushHandle const&		hBackgroundBrush) {
		PASSERT(sizeof(*wclass) == sizeof(WNDCLASSEX));

		wclass->cbSize = sizeof(*wclass);
		// http://msdn.microsoft.com/en-us/library/ff729176(v=VS.85).aspx
		// Window Class Styles
		// MSDN Library/Windows Development/Windows Application UI Development/
		//     Windows and Messages/Window Classes/Window Class Reference/Window Class Styles
		wclass->style = 0x00
				| CS_DROPSHADOW // Enables the drop shadow effect on a window. The effect is turned on and off through SPI_SETDROPSHADOW. Typically, this is enabled for small, short-lived windows such as menus to emphasize their Z order relationship to other windows.
				| CS_OWNDC // Allocates a unique device context for each window in the class.
			//	| CS_HREDRAW // Redraws the entire window if a movement or size adjustment changes the width of the client area.
			//	| CS_VREDRAW // Redraws the entire window if a movement or size adjustment changes the height of the client area.
			;
		wclass->lpfnWndProc = &MyWindowMessageProcessor; // A pointer to the window procedure.
		wclass->cbClsExtra = 0; // The number of extra bytes to allocate following the window-class structure.
		wclass->cbWndExtra = 0; // The number of extra bytes to allocate following the window instance.
		wclass->hInstance = hInstance;
		wclass->hIcon = hIcon;
		wclass->hCursor = hCursor;
		wclass->hbrBackground = hBackgroundBrush;
		wclass->lpszMenuName = NULL; // default menu
		wclass->lpszClassName = _T("d3dtost main window");
		wclass->hIconSm = NULL;
	}

	HICON GetApplicationIcon (my::Window::ModuleHandle const& hInstance) {
		LPTSTR iconId(MAKEINTRESOURCE(IDI_ISILAL));
		HICON const result(LoadIcon(hInstance, iconId));

		if (result == NULL)
			my::global::logger::Get().Error(_T("Could not load icon IDI_ISI from resources"));
		PASSERT(result);

		return result;
	}

	HCURSOR GetApplicationCursor (my::Window::ModuleHandle const& hInstance) {
		UNREFERENCED_PARAMETER(hInstance);
		LPTSTR const		cursorId(MAKEINTRESOURCE(IDC_IBEAM));
		HCURSOR const		result(LoadCursor(NULL, cursorId));

		if (result == NULL)
			my::global::logger::Get().Error(_T("Could not load cursor IDC_IBEAM"));
		PASSERT(result);

		return result;
	}

	HBRUSH GetBackgroundBrush (void) {
		return HBRUSH(COLOR_WINDOW + 1);
	}

	void RegisterMyWindowClass (
			my::Window::WindowClassInfo const* const	wclassinfo,
			my::Window::Atom* const						wclass) {
		ATOM result(RegisterClassEx(wclassinfo));

		if (result == 0)
			__LOG_WIN_ERROR("Could not register window class. Error by windows: ")
//		PASSERT(result);

		*wclass = result;
	}

	void UnregisterMyWindowClass (
			my::Window::WindowClassInfo const* const	wclassinfo,
			my::Window::Atom* const						wclass) {
		BOOL result(UnregisterClass(wclassinfo->lpszClassName, wclassinfo->hInstance));

		if (!result)
			__LOG_WIN_ERROR("Could not unregister window class. Error by windows: ")
//		PASSERT(result)

		my::winutil::nullify(*wclass);
	}

	void CreateMyWindow (
			my::Window::Atom const _window_class,
			my::Window::ModuleHandle const _module_handle,
			my::Window::WindowHandle* const _window_handle_ptr) {
		DWORD const		window_extended_styles		(0);
		LPCTSTR const	window_class				(reinterpret_cast<LPCTSTR>(_window_class)); // (_T("d3dtost main window"));
		LPCTSTR const	_MY_WINDOW_NAME				(_T("Universe Control"));
		DWORD const		window_style				(0
														| WS_BORDER // The window has a thin-line border.
														| WS_CLIPCHILDREN // Excludes the area occupied by child windows when drawing occurs within the parent window.
														| WS_CAPTION // => border: The window has a title bar (includes the WS_BORDER style).
													//	| WS_MAXIMIZEBOX // req(WS_SYSMENU): The window has a maximize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.
														| WS_MINIMIZEBOX // req(WS_SYSMENU): The window has a minimize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.
														| WS_SYSMENU // req(WS_CAPTION): The window has a window menu on its title bar. The WS_CAPTION style must also be specified.
													//	| WS_VISIBLE // The window is initially visible.
														); // (WS_OVERLAPPEDWINDOW);
		int const		_MY_WINDOW_X				(100); // (CW_USEDEFAULT);
		int const		_MY_WINDOW_Y				(100); // (0);
		int const		_MY_WINDOW_WIDTH			(800); // (CW_USEDEFAULT);
		int const		_MY_WINDOW_HEIGHT			(600); // (0);
		HWND const		window_parent				(NULL);
		HMENU const		window_menu					(NULL);
		HMODULE const	module_handle				(_module_handle);
		LPVOID const	creation_parameters			(NULL);

		my::Window::WindowHandle window_handle(CreateWindowEx(
				window_extended_styles,
				window_class,
				_MY_WINDOW_NAME,
				window_style,
				_MY_WINDOW_X,
				_MY_WINDOW_Y,
				_MY_WINDOW_WIDTH,
				_MY_WINDOW_HEIGHT,
				window_parent,
				window_menu,
				module_handle,
				creation_parameters));
		DWORD const window_creation_error(GetLastError());
		if (window_handle == NULL)
			__LOG_WIN_ERROR("Could not create window. Windows' error: ")
		PASSERT(window_handle);

		*_window_handle_ptr = window_handle;

		__window_destroyed = window_handle == NULL;
	} // void CreateMyWindow (void)

	bool DestroyMyWindow (my::Window::WindowHandle* const window_handle_ptr) {
		bool result = false;

		{
			BOOL destroyed(DestroyWindow(*window_handle_ptr));
			if (destroyed == FALSE)
				__LOG_WIN_ERROR("Could not destroy my window. Windows' error: ")
			else
				result = true;
			PASSERT(destroyed == TRUE);
		}

		my::winutil::nullify(*window_handle_ptr);

		return result;
	} // DestroyMyWindow

	} // namespace _

} // namespace

namespace my {

	Window::Window (ModuleHandle const hInstance):
		module_handle				(hInstance),
		application_icon_handle		(_::GetApplicationIcon(hInstance)),
		application_cursor_handle	(_::GetApplicationCursor(hInstance)),
		background_brush_handle		(_::GetBackgroundBrush()),
		window_class_info			(),
		window_class				(),
		window						(),
		main_loop_callback			(NULL),
		main_loop_callback_data		(NULL)
		{
		_::FillMyWindowClass(
				&window_class_info,
				hInstance,
				application_icon_handle,
				application_cursor_handle,
				background_brush_handle);

		_::RegisterMyWindowClass(&window_class_info, &window_class);
		_::CreateMyWindow(window_class, hInstance, &window);
	}

	Window::~Window (void) {
		{
//			_::DestroyMyWindow(&window);
//			PASSERT(window == NULL);

			if (!__window_destroyed)
				__window_destroyed = _::DestroyMyWindow(&window);

			if (__window_destroyed)
				_::UnregisterMyWindowClass(&window_class_info, &window_class);

			__main_loop_callback = NULL;
			__main_loop_callback_env = NULL;
		}
	}

	void Window::SetMainLoopCallback (void (*cb)(void*), void* data) {
		main_loop_callback = cb;
		main_loop_callback_data = data;
	}

	void Window::MainLoop (LPTSTR const command_line, int const show_command) {
		MSG msg;
		BOOL b;
		bool done = false;

		ShowWindow(window, show_command);

		// Main message loop:
		while (!done && (b = GetMessage(&msg, NULL, 0, 0)))
			if (b == -1) {
				__LOG_WIN_ERROR("Error in GetMessage(): ")
				done = true;
			}
			else {
				__main_loop_callback = main_loop_callback;
				__main_loop_callback_env = main_loop_callback_data;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
	}

	bool Window::IsValid (void) const {
		bool result(true
			&& module_handle != NULL
			&& application_icon_handle != NULL
			&& application_cursor_handle != NULL
			&& background_brush_handle != NULL
			&& window_class != NULL
			&& window != NULL
			);
		return result;
	}

} // namespace my

