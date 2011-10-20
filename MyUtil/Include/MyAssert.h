#ifndef __MY_UTIL__MY_ASSERT__H__
#define __MY_UTIL__MY_ASSERT__H__

#pragma warning( push, 0 )
#	include <assert.h>
#pragma warning( pop )

#ifndef NDEBUG
	
	static void __assert (wchar_t const* const expr, wchar_t const* const file, long int line)
		{ _wassert(expr, file, psafecast<unsigned int>(line)); }

#	define _assert(EXPR)	(!!(EXPR) || (__assert(_CRT_WIDE(#EXPR), _CRT_WIDE(__FILE__), __LINE__), 0))
#else
#	define _assert(EXPR)	(static_cast<void>(0x00))
#endif

#endif // __MY_UTIL__MY_ASSERT__H__
