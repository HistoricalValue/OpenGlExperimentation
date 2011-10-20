#ifndef __MY_UTIL__MY_UTILS__H__
#define __MY_UTIL__MY_UTILS__H__

#include <PAssert.h>
#pragma warning( push, 0 )
#	include <stdio.h>
#	include <stdarg.h>
#pragma warning( pop )

template <typename T> static inline
T const* castconst (T* ptr) { return static_cast<T const* const>(ptr); }

template <typename t>
struct TypeOf { typedef t T; };

template <typename T, const size_t N> static
typename TypeOf<char (*)[N]>::T CountOfHelper ( UNALIGNED T (& arr)[N] );

#define countof(ARR)	sizeof(*CountOfHelper(ARR))

template <typename T1, typename T2> static inline
void ofequaltypes (T1&, T2& o2)
	{ utypecheck<T1>(o2); }

template <typename C, typename F> static inline
void foreach (C const& c, F const& f)
	{ std::for_each(c.begin(), c.end(), f); }

template <typename CharType>
struct VsnPrintfer;
template <>
struct VsnPrintfer<char> {
	static const int (*vsnprint) (char* buffer, size_t sizeOfBuffer, size_t count, const char* format, va_list argptr) = &vsnprintf_s;
};
template <typename wchar_t>
struct VsnPrintfer {
	static const int (*vsnprint) (wchar_t* buffer, size_t sizeOfBuffer, size_t count, const wchar_t	* format, va_list argptr) = &_vsnwprintf_s;
};

template <typename CharType>
static
CharType const* format (CharType const* const fmt, ...) {
	static CharType buf[1024];

	va_list args;
	va_start(args, fmt);

	int const retval((*VsnPrintfer<CharType>::vsnprint)(&buf[0], countof(buf), countof(buf), fmt, args));
	PASSERT(retval > 1);

	va_end(args);

	return &buf[0];
}

#define FOREACH(ARRT, ARR, VARNAME)	\
	for (UPTR( ARRT ) VARNAME (( & ARR [0] )) ; VARNAME < & ARR [ countof( ARR ) ] ; ++ VARNAME )
#define IFOREACH(IT_TYPE, CONT, VARNAME) \
	for (IT_TYPE VARNAME (( CONT ).begin()) ; VARNAME != ( CONT ).end() ; ++ VARNAME)

#endif // __MY_UTIL__MY_UTILS__H__
