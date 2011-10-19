#include "stdafx.h"
#include "TheCursed.h"

#include "PWindowsUtilities.h"
using namespace codeshare::utilities::windows_utilities;

#if defined (UNICODE) || defined(_UNICODE)
#	define CONVERT_ERROR_MESSAGE(MSG)	ConvertErrorMessage((MSG))
#else
#	define CONVERT_ERROR_MESSAGE(MSG)	MSG
#endif

namespace d3dtost {

	using codeshare::utilities::ErrorHolder;

	static ErrorHolder*	pErrorHolder(NULL);
	static char			ErrorHolder_mem[sizeof(*pErrorHolder)];
	namespace { namespace _ {

		static void ErrorHolder_Set (codeshare::utilities::ErrorHolder* _pErrorHolder) {
			pErrorHolder = _pErrorHolder;
			my::global::errorHolder::Set(_pErrorHolder);
		}

		static void ErrorHolder_Initialise (void){
			ErrorHolder_Set(new(&ErrorHolder_mem[0]) ErrorHolder());
		}

		static void ErrorHolder_CleanUp (void) {
			ErrorHolder* erho = pErrorHolder;
			ErrorHolder_Set(NULL);
			erho->~ErrorHolder();
			memset(erho, 0x00, sizeof(*pErrorHolder));
		}
	} } // namespace <anonymous>::_

	static my::GenericLogger*		pLogger(NULL);
	static char						pLogger_mem[sizeof(*pLogger)];
	namespace { namespace _ {
		static void OnError_SetLogger (my::GenericLogger* _pLogger) {
			pLogger = _pLogger;
			my::global::logger::Set(_pLogger);
		}

		static my::GenericLogger* OnError_Initialise (void) {
			OnError_SetLogger(new(&pLogger_mem[0]) my::GenericLogger);
			PASSERT(static_cast<void*>(pLogger) == static_cast<void*>(&pLogger_mem[0]))
			return pLogger;
		}

		static void OnError_CleanUp (void) {
			my::GenericLogger* loga = pLogger;
			OnError_SetLogger(NULL);
			loga->~GenericLogger();
			memset(loga, 0x00, sizeof(*pLogger));
		}
	}} // namepace <anonymous>::_


	static LPWSTR ConvertErrorMessage_WhichCausesMemoryCorruptionInTheAllocationHeap (char const* const msg) {
		LPWSTR wmsg(NULL);
		{
			int const requiredLength(MultiByteToWideChar(
					CP_UTF8, //  __in   UINT CodePage,
					0, //  MB_COMPOSITE | MB_USEGLYPHCHARS, //  __in   DWORD dwFlags,
					msg, //  __in   LPCSTR lpMultiByteStr,
					-1, //  __in   int cbMultiByte,
					NULL, //  __out  LPWSTR lpWideCharStr,
					0 //  __in   int cchWideChar
			));

			if (requiredLength == 0) {
				wmsg = _T("Failed in translating an error message from MB to Unicode");
				TCHAR* const errorString(ErrorToString(GetLastError()));
				_assert(false);
				ReleaseErrorString(errorString);
			}
			else {
				LPVOID const mem(HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, psafecast<SIZE_T>(requiredLength + 1)));

				if (mem == NULL) {
					wmsg = _T("Failed in allocating enough memory for error message conversion");
					TCHAR* const errorString(ErrorToString(GetLastError()));
					_assert(false);
					ReleaseErrorString(errorString);
				}
				else {
					wmsg = reinterpret_cast<LPWSTR>(mem);

					int const writenChars(MultiByteToWideChar(
							CP_UTF8, //  __in   UINT CodePage,
							0, //  MB_COMPOSITE | MB_USEGLYPHCHARS, //  __in   DWORD dwFlags,
							msg, //  __in   LPCSTR lpMultiByteStr,
							-1, //  __in   int cbMultiByte,
							wmsg, //  __out  LPWSTR lpWideCharStr,
							requiredLength + 1 //  __in   int cchWideChar
					));

					if (writenChars == 0) {
						wmsg = _T("Could not conver MB message to Unicode");
						TCHAR* const errorString(ErrorToString(GetLastError()));
						_assert(false);
						ReleaseErrorString(errorString);
					}
					else {
						_assert(writenChars == requiredLength);
					}
				}
			}
		}
		return wmsg;
	}
	LPWSTR ConvertErrorMessage (char const* const msg) {
		static LPWSTR const COULD_NOT_ALLOCATE_MEMORY_MESSAGE(L"Could not allocate memory for string conversion");

		size_t const msg_length((strlen(msg) + 1));

		LPWSTR result(reinterpret_cast<LPWSTR>(
				HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, sizeof(wchar_t) * msg_length)));

		if (result == NULL)
			result = COULD_NOT_ALLOCATE_MEMORY_MESSAGE;
		else
			for (size_t i = 0; i < msg_length; ++i)
				psafecast(result[i], msg[i]);
		_assert(result == COULD_NOT_ALLOCATE_MEMORY_MESSAGE || result[msg_length - 1] == L'\0');

		return result;
	}

	static void OnError (char const* const msg) {
		TCHAR* wmsg(CONVERT_ERROR_MESSAGE(msg));
		pLogger->Error(wmsg);
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, wmsg);
		wmsg = NULL;
	}

	void Initialise (void) {
		// 0
		my::Initialise();
		// 1
		codeshare::utilities::Initialise();
		// 2
		_::ErrorHolder_Initialise();
		// 3
		_::OnError_Initialise();
	}

	void CleanUp (void) {
		// 3
		_::OnError_CleanUp();
		// 2
		_::ErrorHolder_CleanUp();
		// 1
		codeshare::utilities::CleanUp();
		// 0
		my::CleanUp();
	}

} // namespace d3dtost

