#include "Private/stdafx.h"

namespace {

	namespace u = codeshare::utilities;
	namespace wu = codeshare::utilities::windows_utilities;
	using ankh::opengl::adapter::NativeWindow;

	static NativeWindow::MainLoopCallback			__mainLoopCallback			(NULL);
	static NativeWindow::MainLoopCallbackClosure	__mainLoopCallbackClosure	(NULL);
	static bool										__window_destroyed			(false);


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
		using codeshare::utilities::windows_utilities::MessageType;
		using codeshare::utilities::windows_utilities::IsUnknownMessageType;
		using codeshare::utilities::windows_utilities::MY_WM_DESTROY;
		using codeshare::utilities::windows_utilities::MY_WM_PAINT;
		using codeshare::utilities::windows_utilities::MY_WM_ERASEBKGND;


		LRESULT result;
		enum MessageType msg(static_cast<enum MessageType>(uMsg));
		PASSERT(static_cast<UINT>(msg) == uMsg);

		if (IsUnknownMessageType(uMsg)) {
			int nothing(0xbeac0ul);
		}


		switch (msg) {
			case MY_WM_DESTROY: {
				PostQuitMessage(0);
				__window_destroyed = true;
				result = 0;
				break;
			}
			case MY_WM_PAINT: {
				result = 0;
				break;
			}
			case MY_WM_ERASEBKGND: {
				result = 0;
				break;
			}
			default: {
				result = DefWindowProc(hWnd, uMsg, wParam, lParam);
				break;
			}
		}

		if (__mainLoopCallback)
				(*__mainLoopCallback)(__mainLoopCallbackClosure);

		return result;
	}

	using ankh::opengl::adapter::NativeWindow;
	typedef NativeWindow::WindowClassInfo				WindowClassInfo;
	typedef NativeWindow::Atom							Atom;
	typedef NativeWindow::ModuleHandle					ModuleHandle;
	typedef NativeWindow::WindowHandle					WindowHandle;
	typedef NativeWindow::IconHandle					IconHandle;
	typedef NativeWindow::CursorHandle					CursorHandle;
	typedef NativeWindow::BrushHandle					BrushHandle;

	namespace _ {
		static void										FillMyWindowClass (WindowClassInfo* wclass);
		static void										RegisterMyWindowClass (WindowClassInfo* wclass);

		static IconHandle								GetApplicationIcon (ModuleHandle const&);
		static CursorHandle								GetApplicationCursor (ModuleHandle const&);
		static BrushHandle								GetBackgroundBrush (void);
	}

	namespace _ {
	void FillMyWindowClass (
			WindowClassInfo* const	wclass,
			ModuleHandle const&		hInstance,
			IconHandle const&		hIcon,
			CursorHandle const&		hCursor,
			BrushHandle const&		hBackgroundBrush) {
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

	IconHandle GetApplicationIcon (ModuleHandle const& hInstance) {
#if 0
		LPTSTR iconId(MAKEINTRESOURCE(IDI_ISILAL));
		HICON const result(LoadIcon(hInstance, iconId));
		
		if (result == NULL)
			my::global::logger::Get().Error(_T("Could not load icon IDI_ISI from resources"));
		PASSERT(result);

		return result;
#else
		return wu::null<IconHandle>();
#endif
	}

	CursorHandle GetApplicationCursor (ModuleHandle const& hInstance) {
		UNREFERENCED_PARAMETER(hInstance);
		LPTSTR const		cursorId(MAKEINTRESOURCE(IDC_IBEAM));
		HCURSOR const		result(LoadCursor(NULL, cursorId));

		return result;
	}

	BrushHandle GetBackgroundBrush (void) {
		return HBRUSH(COLOR_WINDOW + 1);
	}

	void RegisterMyWindowClass (
					WindowClassInfo const* const	wclassinfo,
			__out	Atom* const						wclass,
			__out	LPCTSTR* const					errorMessage)
	{
		ATOM result(RegisterClassEx(wclassinfo));

		if (result == 0 && errorMessage)
			*errorMessage = _T("Could not register window class. Error by windows: ");

		*wclass = result;
	}

	void UnregisterMyWindowClass (
					WindowClassInfo const* const	wclassinfo,
					Atom* const			 			wclass,
			__out LPCTSTR* const					errorMessage)
	{
		BOOL result(UnregisterClass(wclassinfo->lpszClassName, wclassinfo->hInstance));

		if (!result && errorMessage)
			*errorMessage = _T("Could not unregister window class. Error by windows: ");

		wu::nullify(*wclass);
	}

	void CreateMyWindow (
					Atom const			_window_class,
					ModuleHandle const	_module_handle,
			__out	WindowHandle* const	_window_handle_ptr,
			__out	LPCTSTR* const		errorMessage)
	{
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

		WindowHandle window_handle(CreateWindowEx(
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
		if (window_handle == NULL && errorMessage)
				*errorMessage = _T("Could not create window. Windows' error: ");
		PASSERT(window_handle);

		*_window_handle_ptr = window_handle;

		__window_destroyed = window_handle == NULL;
	} // void CreateMyWindow (void)

	bool DestroyMyWindow (WindowHandle* const window_handle_ptr, __out LPCTSTR* const errorMessage)
	{
		bool result = false;
		
		{
			BOOL destroyed(DestroyWindow(*window_handle_ptr));
			if (destroyed == FALSE && errorMessage)
				*errorMessage = _T("Could not destroy my window. Windows' error: ");
			else
				result = true;
			PASSERT(destroyed == TRUE);
		}

		wu::nullify(*window_handle_ptr);

		return result;
	} // DestroyMyWindow

	} // namespace _

} // namespace


namespace ankh { namespace opengl { namespace adapter {

	NativeWindow::NativeWindow (ConstructorArguments const& cargs):
		moduleHandle			(cargs.moduleHandle),
		applicationIconHandle	(_::GetApplicationIcon(cargs.moduleHandle)),
		applicationCursorHandle	(_::GetApplicationCursor(cargs.moduleHandle)),
		backgroundBrushHandle	(_::GetBackgroundBrush()),
		windowClassInfo			(wu::null<WindowClassInfo>()),
		windowClass				(wu::null<Atom>()),
		window					(wu::null<WindowHandle>()),
		mainLoopCallback		(NULL),
		mainLoopCallbackClosure	(NULL),
#pragma warning( push )
#pragma warning( disable: 4351 )
		errorReasonText			(),
		commandLineArguments	(cargs.commandLineArguments),
		showCommand				(cargs.showCommand)
	{
#pragma warning( pop )
		ModuleHandle const& moduleHandle(cargs.moduleHandle);

		_::FillMyWindowClass(
				&windowClassInfo,
				moduleHandle,
				applicationIconHandle,
				applicationCursorHandle,
				backgroundBrushHandle);
		
		bool hasError = false;
		
		{
			LPCTSTR customMessage(NULL);
			_::RegisterMyWindowClass(&windowClassInfo, &windowClass, &customMessage);
			hasError = windowClass == NULL;
			if (hasError)
				StoreNativeError(customMessage);
		}

		if (!hasError) {
			LPCTSTR customMessage(NULL);
			_::CreateMyWindow(windowClass, moduleHandle, &window, &customMessage);
			hasError = window == NULL;
			if (hasError)
				StoreNativeError(customMessage);
		}
	}

	NativeWindow::~NativeWindow (void) {
		if (!__window_destroyed) {
			LPCTSTR __ignored(NULL);
			__window_destroyed = _::DestroyMyWindow(&window, &__ignored);
		}

		if (__window_destroyed) {
			LPCTSTR __ignored(NULL);
			_::UnregisterMyWindowClass(&windowClassInfo, &windowClass, &__ignored);
		}

		__mainLoopCallback = NULL;
		__mainLoopCallbackClosure = NULL;
	}

	void NativeWindow::SetMainLoopCallback (MainLoopCallback const& callback) {
		mainLoopCallback = callback;
	}

	void NativeWindow::SetMainLoopCallbackClosure (MainLoopCallbackClosure const& closure) {
		mainLoopCallbackClosure = closure;
	}

	NativeWindow::WindowHandle const& NativeWindow::GetWindow (void) const {
		return window;
	}


	void NativeWindow::MainLoop (void) {
		MSG msg;
		BOOL b;
		bool done = false;

		ShowWindow(window, showCommand.command);

		// Main message loop:
		while (!done && (b = GetMessage(&msg, NULL, 0, 0)))
			if (b == -1) {
				StoreNativeError(_T("Error in GetMessage(): "));
				done = true;
			}
			else {
				__mainLoopCallback = mainLoopCallback;
				__mainLoopCallbackClosure = mainLoopCallbackClosure;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
	}

	bool NativeWindow::IsValid (void) const {
		bool result(true
			&& moduleHandle != NULL
			&& applicationIconHandle != NULL
			&& applicationCursorHandle != NULL
			&& backgroundBrushHandle != NULL
			&& windowClass != NULL
			&& window != NULL
			&& errorReasonText[0] == '\0'
			);
		return result;
	}


	void NativeWindow::StoreNativeError (LPCTSTR const customMessage) {
		using codeshare::utilities::windows_utilities::ErrorCode;
		using codeshare::utilities::windows_utilities::ErrorToString;
		using codeshare::utilities::wcsconcat;

		ErrorCode const	lastError		(GetLastError());
		LPCTSTR const	lastErrorText	(ErrorToString(lastError));

		wcsconcat(&errorReasonText[0], sizeof(errorReasonText)/sizeof(errorReasonText[0]), customMessage);
	}

}}} // namespace ankh::opengl::adapter

