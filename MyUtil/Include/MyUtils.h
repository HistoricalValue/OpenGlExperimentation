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

static inline int vsntprintf (char* const buffer, size_t const sizeOfBuffer, size_t const count, const char* const format, va_list const argptr)
	{ return vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr); }
static inline int vsntprintf (wchar_t* const buffer, size_t const sizeOfBuffer, size_t const count, const wchar_t* const format, va_list const argptr)
	{ return _vsnwprintf_s(buffer, sizeOfBuffer, count, format, argptr); }

template <typename CharType>
static
CharType* vformat (CharType* const buf, size_t const bufLength, CharType const* const fmt, va_list const args) {
	int const retval(vsntprintf(buf, bufLength, bufLength-1, fmt, args));
	PASSERT(retval > 1);

	buf[bufLength - 1] = '\0';

	return buf;
}

template <typename CharType, const size_t N>
static
CharType* vformat (CharType (&buf)[N], CharType const* const fmt, va_list const args)
	{ return vformat(&buf[0], N, fmt, args); }

template <typename CharType>
static
CharType* format (CharType* const buf, size_t const bufLength, CharType const* const fmt, ...) {
	va_list args;
	va_start(args, fmt);

	vformat(buf, bufLength, fmt, args);

	va_end(args);

	return buf;
}

template <typename CharType, const size_t N>
static
CharType* format (CharType (&buf)[N], CharType const* const fmt, ...) {
	va_list args;
	va_start(args, fmt);

	vformat(buf, fmt, args);

	va_end(args);

	return buf;
}

template <typename CharType>
static
CharType const* format (CharType const* const fmt, ...) {
	static CharType buf[1 << 14]; // 16 KiB

	va_list args;
	va_start(args, fmt);

	vformat(buf, fmt, args);

	va_end(args);

	return buf;
}


#define FOREACH(ARRT, ARR, VARNAME)	\
	for (UPTR( ARRT ) VARNAME (( & ARR [0] )) ; VARNAME < & ARR [ countof( ARR ) ] ; ++ VARNAME )
#define IFOREACH(IT_TYPE, CONT, VARNAME) \
	for (IT_TYPE VARNAME (( CONT ).begin()) ; VARNAME != ( CONT ).end() ; ++ VARNAME)

#endif // __MY_UTIL__MY_UTILS__H__
