#pragma once

#include "MyUtil.h"
#include "MyWindows.h"

namespace my {

	namespace winutil {

		LPTSTR MYUTIL_API ErrorToString (DWORD const& error);
		void MYUTIL_API ReleaseErrorString (LPTSTR);
		LPCTSTR MYUTIL_API LastErrorString (void);

		enum MessageType {
			MY_WM_NULL						= WM_NULL,						// 0x0000
			MY_WM_CREATE					= WM_CREATE,					// 0x0001
			MY_WM_DESTROY					= WM_DESTROY,					// 0x0002
			MY_WM_MOVE						= WM_MOVE,						// 0x0003
			MY_WM_SIZE						= WM_SIZE,						// 0x0005
			MY_WM_ACTIVATE					= WM_ACTIVATE,					// 0x0006
			MY_WM_SETFOCUS					= WM_SETFOCUS,					// 0x0007
			MY_WM_KILLFOCUS					= WM_KILLFOCUS,					// 0x0008
			MY_WM_ENABLE					= WM_ENABLE,					// 0x000a
			MY_WM_PAINT						= WM_PAINT,						// 0x000f
			//
			MY_WM_CLOSE						= WM_CLOSE,						// 0x0010
			MY_WM_QUERYENDSESSION			= WM_QUERYENDSESSION,			// 0x0011
			MY_WM_QUIT						= WM_QUIT,						// 0x0012
			MY_WM_QUERYOPEN					= WM_QUERYOPEN,					// 0x0013
			MY_WM_ERASEBKGND				= WM_ERASEBKGND,				// 0x0014
			MY_WM_SYSCOLORCHANGE			= WM_SYSCOLORCHANGE,			// 0x0015
			MY_WM_ENDSESSION				= WM_ENDSESSION,				// 0x0016
			MY_WM_SHOWWINDOW				= WM_SHOWWINDOW,				// 0x0018
			MY_WM_ACTIVATEAPP				= WM_ACTIVATEAPP,				// 0x001c
			MY_WM_SETCURSOR					= WM_SETCURSOR,					// 0x0020
			MY_WM_MOUSEACTIVATE				= WM_MOUSEACTIVATE,				// 0x0021
			MY_WM_GETMINMAXINFO				= WM_GETMINMAXINFO,				// 0x0024
			//
			MY_WM_WINDOWPOSCHANGING			= WM_WINDOWPOSCHANGING,			// 0x0046
			MY_WM_WINDOWPOSCHANGED			= WM_WINDOWPOSCHANGED,			// 0x0047
			//
			MY_WM_NOTIFY					= WM_NOTIFY,					// 0x004E
			MY_WM_INPUTLANGCHANGEREQUEST	= WM_INPUTLANGCHANGEREQUEST,	// 0x0050
			MY_WM_INPUTLANGCHANGE			= WM_INPUTLANGCHANGE,			// 0x0051
			MY_WM_TCARD						= WM_TCARD,						// 0x0052
			MY_WM_HELP						= WM_HELP,						// 0x0053
			MY_WM_USERCHANGED				= WM_USERCHANGED,				// 0x0054
			MY_WM_NOTIFYFORMAT				= WM_NOTIFYFORMAT,				// 0x0055
			//
			MY_WM_CONTEXTMENU				= WM_CONTEXTMENU,				// 0x007B
			MY_WM_STYLECHANGING				= WM_STYLECHANGING,				// 0x007C
			MY_WM_STYLECHANGED				= WM_STYLECHANGED,				// 0x007D
			MY_WM_DISPLAYCHANGE				= WM_DISPLAYCHANGE,				// 0x007E
			MY_WM_GETICON					= WM_GETICON,					// 0x007F
			MY_WM_SETICON					= WM_SETICON,					// 0x0080
			//
			MY_WM_NCCREATE					= WM_NCCREATE,					// 0x0081
			MY_WM_NCDESTROY					= WM_NCDESTROY,					// 0x0082
			MY_WM_NCCALCSIZE				= WM_NCCALCSIZE,				// 0x0083
			MY_WM_NCHITTEST					= WM_NCHITTEST,					// 0x0084
			MY_WM_NCPAINT					= WM_NCPAINT,					// 0x0085
			MY_WM_NCACTIVATE				= WM_NCACTIVATE,				// 0x0086
			MY_WM_GETDLGCODE				= WM_GETDLGCODE,				// 0x0087
			MY_WM_SYNCPAINT					= WM_SYNCPAINT,					// 0x0088
			//
			MY_WM_NCMOUSEMOVE				= WM_NCMOUSEMOVE,				// 0x00A0
			MY_WM_NCLBUTTONDOWN				= WM_NCLBUTTONDOWN,				// 0x00A1
			MY_WM_NCLBUTTONUP				= WM_NCLBUTTONUP,				// 0x00A2
			MY_WM_NCLBUTTONDBLCLK			= WM_NCLBUTTONDBLCLK,			// 0x00A3
			//
			MY_WM_KEYFIRST_KEYDOWN			= WM_KEYDOWN,					// 0x0100
			MY_WM_KEYUP						= WM_KEYUP,						// 0x0101
			MY_WM_CHAR						= WM_CHAR,						// 0x0102
			MY_WM_DEADCHAR					= WM_DEADCHAR,					// 0x0103
			MY_WM_SYSKEYDOWN				= WM_SYSKEYDOWN,				// 0x0104
			MY_WM_SYSKEYUP					= WM_SYSKEYUP,					// 0x0105
			MY_WM_SYSCHAR					= WM_SYSCHAR,					// 0x0106
			MY_WM_SYSDEADCHAR				= WM_SYSDEADCHAR,				// 0x0107
			MY_WM_KEYLAST					= WM_KEYLAST,					// 0x0108
			MY_WM_KEYLAST_UNICHAR			= WM_UNICHAR,					// 0x0109
			//
			MY_WM_INITDIALOG				= WM_INITDIALOG,				// 0x0110
			MY_WM_COMMAND					= WM_COMMAND,					// 0x0111
			MY_WM_SYSCOMMAND				= WM_SYSCOMMAND,				// 0x0112
			MY_WM_TIMER						= WM_TIMER,						// 0x0113
			MY_WM_HSCROLL					= WM_HSCROLL,					// 0x0114
			MY_WM_VSCROLL					= WM_VSCROLL,					// 0x0115
			MY_WM_INITMENU					= WM_INITMENU,					// 0x0116
			MY_WM_INITMENUPOPUP				= WM_INITMENUPOPUP,				// 0x0117
			MY_WM_GESTURE					= WM_GESTURE,					// 0x0119
			MY_WM_GESTURENOTIFY				= WM_GESTURENOTIFY,				// 0x011A
			MY_WM_MENUSELECT				= WM_MENUSELECT,				// 0x011F
			MY_WM_MENUCHAR					= WM_MENUCHAR,					// 0x0120
			MY_WM_ENTERIDLE					= WM_ENTERIDLE,					// 0x0121
			MY_WM_MENURBUTTONUP				= WM_MENURBUTTONUP,				// 0x0122
			MY_WM_MENUDRAG					= WM_MENUDRAG,					// 0x0123
			MY_WM_MENUGETOBJECT				= WM_MENUGETOBJECT,				// 0x0124
			MY_WM_UNINITMENUPOPUP			= WM_UNINITMENUPOPUP,			// 0x0125
			MY_WM_MENUCOMMAND				= WM_MENUCOMMAND,				// 0x0126
			MY_WM_CHANGEUISTATE				= WM_CHANGEUISTATE,				// 0x0127
			MY_WM_UPDATEUISTATE				= WM_UPDATEUISTATE,				// 0x0128
			MY_WM_QUERYUISTATE				= WM_QUERYUISTATE,				// 0x0129
			//
			MY_WM_MOUSEFIRST_MOUSEMOVE		= WM_MOUSEFIRST,				// 0x0200
			MY_WM_LBUTTONDOWN				= WM_LBUTTONDOWN,				// 0x0201
			MY_WM_LBUTTONUP					= WM_LBUTTONUP,					// 0x0202
			MY_WM_LBUTTONDBLCLK				= WM_LBUTTONDBLCLK,				// 0x0203
			MY_WM_RBUTTONDOWN				= WM_RBUTTONDOWN,				// 0x0204
			MY_WM_RBUTTONUP					= WM_RBUTTONUP,					// 0x0205
			MY_WM_RBUTTONDBLCLK				= WM_RBUTTONDBLCLK,				// 0x0206
			MY_WM_MBUTTONDOWN				= WM_MBUTTONDOWN,				// 0x0207
			MY_WM_MBUTTONUP					= WM_MBUTTONUP,					// 0x0208
			MY_WM_MBUTTONDBLCLK				= WM_MBUTTONDBLCLK,				// 0x0209
			//
			MY_WM_PARENTNOTIFY				= WM_PARENTNOTIFY,				// 0x0210
			MY_WM_ENTERMENULOOP				= WM_ENTERMENULOOP,				// 0x0211
			MY_WM_EXITMENULOOP				= WM_EXITMENULOOP,				// 0x0212
			MY_WM_NEXTMENU					= WM_NEXTMENU,					// 0x0213
			MY_WM_SIZING					= WM_SIZING,					// 0x0214
			MY_WM_CAPTURECHANGED			= WM_CAPTURECHANGED,			// 0x0215
			MY_WM_MOVING					= WM_MOVING,					// 0x0216
			MY_WM_POWERBROADCAST			= WM_POWERBROADCAST,			// 0x0218
			//
			MY_WM_DEVICECHANGE				= WM_DEVICECHANGE,				// 0x0219
			MY_WM_MDICREATE					= WM_MDICREATE,					// 0x0220
			MY_WM_MDIDESTROY				= WM_MDIDESTROY,				// 0x0221
			MY_WM_MDIACTIVATE				= WM_MDIACTIVATE,				// 0x0222
			MY_WM_MDIRESTORE				= WM_MDIRESTORE,				// 0x0223
			MY_WM_MDINEXT					= WM_MDINEXT,					// 0x0224
			MY_WM_MDIMAXIMIZE				= WM_MDIMAXIMIZE,				// 0x0225
			MY_WM_MDITILE					= WM_MDITILE,					// 0x0226
			MY_WM_MDICASCADE				= WM_MDICASCADE,				// 0x0227
			MY_WM_MDIICONARRANGE			= WM_MDIICONARRANGE,			// 0x0228
			MY_WM_MDIGETACTIVE				= WM_MDIGETACTIVE,				// 0x0229
			MY_WM_MDISETMENU				= WM_MDISETMENU,				// 0x0230
			MY_WM_ENTERSIZEMOVE				= WM_ENTERSIZEMOVE,				// 0x0231
			MY_WM_EXITSIZEMOVE				= WM_EXITSIZEMOVE,				// 0x0232
			MY_WM_DROPFILES					= WM_DROPFILES,					// 0x0233
			MY_WM_MDIREFRESHMENU			= WM_MDIREFRESHMENU,			// 0x0234
			MY_WM_TOUCH						= WM_TOUCH,						// 0x0240
			//
			MY_WM_IME_SETCONTEXT			= WM_IME_SETCONTEXT,			// 0x0281
			MY_WM_IME_NOTIFY				= WM_IME_NOTIFY,				// 0x0282
			//
			MY_WM_NCMOUSEHOVER				= WM_NCMOUSEHOVER,				// 0x02A0
			MY_WM_NCMOUSELEAVE				= WM_NCMOUSELEAVE,				// 0x02A2
			//
			MY_WM_DWMNCRENDERINGCHANGED		= WM_DWMNCRENDERINGCHANGED,		// 0x031f
			//
			MY_WM_UNKNOWN_0090				= 0x0090,
			MY_WM_UNKNOWN_0093				= 0x0093,
			MY_WM_UNKNOWN_0094				= 0x0094,
			MY_WM_UNKNOWN_C0AD				= 0xc0ad,
			MY_WM_UNKNOWN_C22B				= 0xc22b,
			MY_WM_UNKNOWN_C148				= 0xc148,
			//
			__wm__invalid					= 0xffffffff
		};

		bool MYUTIL_API IsUnknownMessageType (UINT const& message_type);


		template <typename T>
		T null (void);

		template <typename T>
		void nullify (T&);

		template <typename T>
		bool isnull (T const&);
	} // namespace winutil

} // namespace my

