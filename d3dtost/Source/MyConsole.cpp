#include "stdafx.h"
#include "TheCursed.h"
#include "PWindowsUtilities.h"

using codeshare::utilities::windows_utilities::LastErrorString;
using codeshare::utilities::windows_utilities::isnull;
using codeshare::utilities::windows_utilities::ErrorToString;
using codeshare::utilities::windows_utilities::ReleaseErrorString;

namespace my {

    Console::Console (void):
        outProxy(),
        errProxy(),
        consoleCreated(),
        log("Console_log.txt", std::ios::out | std::ios::trunc)
    {
        PASSERT(!log.bad())

        consoleCreated = AllocConsole() == TRUE;
        if (!consoleCreated)
            _T_STR errstr(LastErrorString());
        PASSERT(consoleCreated)

        outProxy = GetStdHandle(STD_OUTPUT_HANDLE);
        if (isnull(outProxy))
            _T_STR errstr(LastErrorString());
        PASSERT(!isnull(outProxy))

        errProxy = GetStdHandle(STD_ERROR_HANDLE);
        if (isnull(errProxy))
            _T_STR errstr(LastErrorString());
        PASSERT(!isnull(errProxy))
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
        log << str;

        PASSERT(IsValid())
        DWORD const    charsToWrite    (strlen(str));
        DWORD        charsWritten    (0);
        BOOL const    worked(WriteConsoleA(outProxy, &str[0], charsToWrite, &charsWritten, NULL));
        PASSERT(worked == FALSE || charsToWrite == charsWritten)
        if (worked == FALSE) {
            DWORD const        error        (GetLastError());
            TCHAR* const    errorstring    (ErrorToString(error));
            my::global::logger::Get().Error(errorstring);
            ReleaseErrorString(errorstring);
        }
        PASSERT(worked == TRUE)
    }
    #endif

    void Console::WriteToOutputStream (TCHAR const* const str) {
        PASSERT(IsValid())
        DWORD const    charsToWrite    (_tcsclen(str));
        DWORD        charsWritten    (0);
        BOOL const    worked(WriteConsole(outProxy, &str[0], charsToWrite, &charsWritten, NULL));
        PASSERT(worked == FALSE || charsToWrite == charsWritten)
        if (worked == FALSE) {
            DWORD const        error        (GetLastError());
            TCHAR* const    errorstring    (ErrorToString(error));
            my::global::logger::Get().Error(errorstring);
            ReleaseErrorString(errorstring);
        }
        PASSERT(worked == TRUE)
    }

    void Console::WriteToErrorStream (TCHAR const* const str) {
        PASSERT(IsValid())
        DWORD const    charsToWrite    (_tcsclen(str));
        DWORD        charsWritten    (0);
        BOOL const    worked(WriteConsole(errProxy, &str[0], charsToWrite, &charsWritten, NULL));
        PASSERT(worked == FALSE || charsToWrite == charsWritten)
        if (worked == FALSE) {
            DWORD const        error        (GetLastError());
            TCHAR* const    errorstring    (ErrorToString(error));
            ::n = 12;
            my::global::logger::Get().Error(errorstring);
            ReleaseErrorString(errorstring);
        }
        PASSERT(worked == TRUE)
    }

} // namespace my

