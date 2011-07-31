#include <stdafx.h>

namespace my {
	namespace winutil {

		static TCHAR			buff[1024];

		LPTSTR ErrorToString (DWORD const& error) {
			DWORD const			flags(0x00
										| FORMAT_MESSAGE_FROM_SYSTEM
										| FORMAT_MESSAGE_IGNORE_INSERTS);
			LPTSTR const		pBuff(reinterpret_cast<LPTSTR>(&buff));
			LPCVOID const		source(NULL);
			DWORD const			msgId(error);
			DWORD const			lang(MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
			DWORD const			size(sizeof(buff)/sizeof(buff[0]));
			va_list* const		args(NULL);

			DWORD retval(FormatMessage(flags, source, msgId, lang, buff, size, args));
			if (retval == 0)
				// failure
				return NULL;
			return buff;
		}

		void ReleaseErrorString (LPTSTR const str) {
			assert(str == buff);
		}

		LPCTSTR LastErrorString (void) {
			DWORD const error(GetLastError());
			return ErrorToString(error);
		}

		bool IsUnknownMessageType (UINT const& message_type) {
			bool const result(true
				&& message_type != WM_NULL
				&& message_type != WM_CREATE
				&& message_type != WM_DESTROY
				&& message_type != WM_MOVE
				&& message_type != WM_SIZE
				&& message_type != WM_ACTIVATE
				&& message_type != WM_SETFOCUS
				&& message_type != WM_KILLFOCUS
				&& message_type != WM_ENABLE
				&& message_type != WM_PAINT

				&& message_type != WM_CLOSE
				&& message_type != WM_QUERYENDSESSION
				&& message_type != WM_QUIT
				&& message_type != WM_QUERYOPEN
				&& message_type != WM_ERASEBKGND
				&& message_type != WM_SYSCOLORCHANGE
				&& message_type != WM_ENDSESSION

				&& message_type != WM_SHOWWINDOW
				&& message_type != WM_ACTIVATEAPP
				&& message_type != WM_SETCURSOR
				&& message_type != WM_MOUSEACTIVATE
				&& message_type != WM_GETMINMAXINFO
				&& message_type != WM_WINDOWPOSCHANGING
				&& message_type != WM_WINDOWPOSCHANGED

				&& message_type != WM_NOTIFY
				&& message_type != WM_INPUTLANGCHANGEREQUEST
				&& message_type != WM_INPUTLANGCHANGE
				&& message_type != WM_TCARD
				&& message_type != WM_HELP
				&& message_type != WM_USERCHANGED
				&& message_type != WM_NOTIFYFORMAT

				&& message_type != WM_CONTEXTMENU
				&& message_type != WM_STYLECHANGING
				&& message_type != WM_STYLECHANGED
				&& message_type != WM_DISPLAYCHANGE
				&& message_type != WM_GETICON
				&& message_type != WM_SETICON

				&& message_type != WM_NCCREATE
				&& message_type != WM_NCDESTROY
				&& message_type != WM_NCCALCSIZE
				&& message_type != WM_NCHITTEST
				&& message_type != WM_NCPAINT
				&& message_type != WM_NCACTIVATE
				&& message_type != WM_GETDLGCODE
				&& message_type != WM_SYNCPAINT

				&& message_type != WM_NCMOUSEMOVE
				&& message_type != WM_NCLBUTTONDOWN
				&& message_type != WM_NCLBUTTONUP
				&& message_type != WM_NCLBUTTONDBLCLK

				&& message_type != WM_KEYDOWN
				&& message_type != WM_KEYUP
				&& message_type != WM_CHAR
				&& message_type != WM_DEADCHAR
				&& message_type != WM_SYSKEYDOWN
				&& message_type != WM_SYSKEYUP
				&& message_type != WM_SYSCHAR
				&& message_type != WM_SYSDEADCHAR
				&& message_type != WM_KEYLAST
				&& message_type != WM_UNICHAR

				&& message_type != WM_INITDIALOG
				&& message_type != WM_COMMAND
				&& message_type != WM_SYSCOMMAND
				&& message_type != WM_TIMER
				&& message_type != WM_HSCROLL
				&& message_type != WM_VSCROLL
				&& message_type != WM_INITMENU
				&& message_type != WM_INITMENUPOPUP
				&& message_type != WM_GESTURE
				&& message_type != WM_GESTURENOTIFY
				&& message_type != WM_MENUSELECT
				&& message_type != WM_MENUCHAR
				&& message_type != WM_ENTERIDLE
				&& message_type != WM_MENURBUTTONUP
				&& message_type != WM_MENUDRAG
				&& message_type != WM_MENUGETOBJECT
				&& message_type != WM_UNINITMENUPOPUP
				&& message_type != WM_MENUCOMMAND
				&& message_type != WM_CHANGEUISTATE
				&& message_type != WM_UPDATEUISTATE
				&& message_type != WM_QUERYUISTATE

				&& message_type != WM_MOUSEFIRST
				&& message_type != WM_LBUTTONDOWN
				&& message_type != WM_LBUTTONUP
				&& message_type != WM_LBUTTONDBLCLK
				&& message_type != WM_RBUTTONDOWN
				&& message_type != WM_RBUTTONUP
				&& message_type != WM_RBUTTONDBLCLK
				&& message_type != WM_MBUTTONDOWN
				&& message_type != WM_MBUTTONUP
				&& message_type != WM_MBUTTONDBLCLK

				&& message_type != WM_PARENTNOTIFY
				&& message_type != WM_ENTERMENULOOP
				&& message_type != WM_EXITMENULOOP
				&& message_type != WM_NEXTMENU
				&& message_type != WM_SIZING
				&& message_type != WM_CAPTURECHANGED
				&& message_type != WM_MOVING
				&& message_type != WM_POWERBROADCAST

				&& message_type != WM_DEVICECHANGE
				&& message_type != WM_MDICREATE
				&& message_type != WM_MDIDESTROY
				&& message_type != WM_MDIACTIVATE
				&& message_type != WM_MDIRESTORE
				&& message_type != WM_MDINEXT
				&& message_type != WM_MDIMAXIMIZE
				&& message_type != WM_MDITILE
				&& message_type != WM_MDICASCADE
				&& message_type != WM_MDIICONARRANGE
				&& message_type != WM_MDIGETACTIVE
				&& message_type != WM_MDISETMENU
				&& message_type != WM_ENTERSIZEMOVE
				&& message_type != WM_EXITSIZEMOVE
				&& message_type != WM_DROPFILES
				&& message_type != WM_MDIREFRESHMENU
				&& message_type != WM_TOUCH

				&& message_type != WM_IME_SETCONTEXT
				&& message_type != WM_IME_NOTIFY

				&& message_type != WM_NCMOUSEHOVER
				&& message_type != WM_NCMOUSELEAVE

				&& message_type != WM_DWMNCRENDERINGCHANGED

				&& message_type != 0x0090
				&& message_type != 0x0093
				&& message_type != 0x0094
				&& message_type != 0xc0ad
				&& message_type != 0xc22b
				&& message_type != 0xc148
				);
			return result;
		}

		// nullifiers
		//
		// - unsigned short int
		template <>
		MYUTIL_API unsigned short int null (void) {
			return 0x0000;
		}

		template <>
		MYUTIL_API void nullify<unsigned short int> (unsigned short int& n) {
			n = 0x0000;
		}

		template <>
		MYUTIL_API bool isnull<unsigned short int> (unsigned short int const& n) {
			return n == 0x0000;
		}

		// - HWND
		template <>
		MYUTIL_API void nullify<HWND> (HWND& h) {
			h = NULL;
		}

		// - void* (for HANDLE)
		template <>
		MYUTIL_API bool isnull<void*> (void* const& p) {
			return p == NULL;
		}

	} // namespace winutil
} // namespace my
