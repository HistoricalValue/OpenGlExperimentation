#ifndef __MY_UTIL__MY_UTILS__H__
#define __MY_UTIL__MY_UTILS__H__

#include <PAssert.h>
#pragma warning( push, 0 )
#	include <stdio.h>
#	include <stdarg.h>
#	include "utypes.h"
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




template <typename T, typename K>
static inline
T const& umapget (std::map<K, T> const& m, K const& k) {
	typedef std::map<K, T> Map;

	Map::const_iterator const result = m.find(k);
	DASSERT(result != m.end());

	return result->second;
}

template <typename T, typename K, typename Comparator, typename Allocator>
static inline
T& umapadd (std::map<K, T, Comparator, Allocator>& m, K const& k, T const& v = T()) {
	typedef std::map<K, T, Comparator, Allocator>	Map;
	typedef std::pair<Map::iterator, bool>			Insertion;
	typedef Map::value_type							Pair;

	Insertion const insertion(m.insert(Pair(k, v)));
	DASSERT(insertion.second);	// fresh addition

	return insertion.first->second;
}

template <typename T>
static inline
T const* ucastconst (T* const ptr) { return ptr; }

template <typename T>
struct dptr {
	typedef dptr<T>	Self;

	T* ptr;

	T*		operator -> (void) const	{ return DPTR(DNULLCHECK(ptr)); }
	T&		operator * (void) const		{ return *operator ->(); }

	T*		native (void) const			{ return operator ->(); }
	void	Delete (void)				{ udelete(ptr); }
	void	nullify (void)				{ unullify(ptr); }
	Self&	New (void)					{ unew(ptr); }
	bool	isnull (void) const			{ return ptr == NULL; }
	T*		discard (void)				{ T* const result(ptr); nullify(); return result; }

	template <void (*Deleter)(T*)>
	void	Delete (void)				{ (*Deleter)(ptr); nullify(); }
	template <typename Deleter>
	void	Delete (Deleter const& d)	{ d(ptr); nullify(); }

	explicit dptr (T* const _ptr = NULL): ptr(_ptr) {}
	dptr (dptr const& p): ptr(p.ptr) {}
	~dptr (void) { DASSERT(ptr == NULL); }

	void operator = (T* const _ptr) { DASSERT(!ptr); ptr = _ptr; }
	UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(dptr)
};

#endif // __MY_UTIL__MY_UTILS__H__
