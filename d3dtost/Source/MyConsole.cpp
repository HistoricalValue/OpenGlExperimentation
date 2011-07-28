#include "stdafx.h"

namespace my {

	Console::Console (void):
		outProxy(),
		errProxy(),
		consoleCreated()
	{
		consoleCreated = AllocConsole() == TRUE;
		if (!consoleCreated)
			_T_STR errstr(my::winutil::LastErrorString());
		PASSERT(consoleCreated)

		outProxy = GetStdHandle(STD_OUTPUT_HANDLE);
		if (my::winutil::isnull(outProxy))
			_T_STR errstr(my::winutil::LastErrorString());
		PASSERT(!my::winutil::isnull(outProxy))

		errProxy = GetStdHandle(STD_ERROR_HANDLE);
		if (my::winutil::isnull(errProxy))
			_T_STR errstr(my::winutil::LastErrorString());
		PASSERT(!my::winutil::isnull(errProxy))
	}

	Console::~Console (void) {
		if (consoleCreated)
			FreeConsole();
	}

	bool Console::IsValid (void) const {
		return true
				&& consoleCreated
				&& outProxy != NULL
				&& errProxy != NULL
				;
	}

	#ifdef _UNICODE
	void Console::WriteToOutputStreamA (char const* const str) {
		PASSERT(IsValid())
		DWORD const	charsToWrite	(strlen(str));
		DWORD		charsWritten	(0);
		BOOL const	worked(WriteConsoleA(outProxy, &str[0], charsToWrite, &charsWritten, NULL));
		PASSERT(worked == FALSE || charsToWrite == charsWritten)
		if (worked == FALSE) {
			DWORD const		error		(GetLastError());
			LPTSTR const	errorstring	(my::winutil::ErrorToString(error));
			my::global::logger::Get().Error(errorstring);
			my::winutil::ReleaseErrorString(errorstring);
		}
		PASSERT(worked == TRUE)
	}
	#endif

	void Console::WriteToOutputStream (LPCTSTR const str) {
		PASSERT(IsValid())
		DWORD const	charsToWrite	(_tcsclen(str));
		DWORD		charsWritten	(0);
		BOOL const	worked(WriteConsole(outProxy, &str[0], charsToWrite, &charsWritten, NULL));
		PASSERT(worked == FALSE || charsToWrite == charsWritten)
		if (worked == FALSE) {
			DWORD const		error		(GetLastError());
			LPTSTR const	errorstring	(my::winutil::ErrorToString(error));
			my::global::logger::Get().Error(errorstring);
			my::winutil::ReleaseErrorString(errorstring);
		}
		PASSERT(worked == TRUE)
	}

	void Console::WriteToErrorStream (LPCTSTR const str) {
		PASSERT(IsValid())
		DWORD const	charsToWrite	(_tcsclen(str));
		DWORD		charsWritten	(0);
		BOOL const	worked(WriteConsole(errProxy, &str[0], charsToWrite, &charsWritten, NULL));
		PASSERT(worked == FALSE || charsToWrite == charsWritten)
		if (worked == FALSE) {
			DWORD const		error		(GetLastError());
			LPTSTR const	errorstring	(my::winutil::ErrorToString(error));
			::n = 12;
			my::global::logger::Get().Error(errorstring);
			my::winutil::ReleaseErrorString(errorstring);
		}
		PASSERT(worked == TRUE)
	}

} // namespace my

