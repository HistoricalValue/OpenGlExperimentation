#ifndef __MY_UTIL__MY_UTILS__H__
#define __MY_UTIL__MY_UTILS__H__

#include <PAssert.h>
#pragma warning( push, 0 )
#	include <stdio.h>
#	include <stdarg.h>
#	include "utypes.h"
#	include "DDebug.h"
#	include "ufunctors.h"
#	include "utypes.h"
#	include <fstream>
#pragma warning( pop )

///////////////////////////////////////////////////////////
typedef unsigned char		uchar;
typedef unsigned short int	ushort;
typedef unsigned int		uint;
typedef unsigned long int	ulong;

typedef uchar const			cuchar;
typedef ushort const		cushort;
typedef uint const			cuint;
typedef ulong const			culong;

typedef char const			cchar;
typedef short const			cshort;
typedef int const			cint;
typedef long const			clong;

typedef float const			cfloat;
typedef double const		cdouble;

typedef size_t const		csize_t;

typedef void*				voidp;
typedef voidp const			voidcp;

///////////////////////////////////////////////////////////
#define NMUDECLARE_ABSTRACT_CLONE(CLASS)								\
		virtual CLASS *	Clone (void* at, size_t bytesize) const = 0;	\
		virtual CLASS * Clone (void) const = 0;

#define NMUINLINE_DEFINE_CLONE_VIA_COPY_CONSTRUCTOR(CLASS)										\
		CLASS *	Clone (void* at, size_t bytesize) const											\
					{ return bytesize >= sizeof(*this)? callconstructor(at, *this) : NULL; }	\
		CLASS *	Clone (void) const																\
					{ return DNEWCLASS(CLASS, (*this)); }

///////////////////////////////////////////////////////////
#define NMUINLINE_DEFINE_VOID_RESET_VIA_DEFAULT_CONSTRUCTOR		\
		void	Reset (void) { reset(this); }

#define NMUINLINE_DEFINE_RESET_VIA_DEFAULT_CONSTRUCTOR(CLASS)	\
		CLASS &	Reset (void) { return *reset(this); }

///////////////////////////////////////////////////////////
#define NMUDECLARE_WRITE_LINES														\
		std::list<std::string>&	WriteLinesTo (std::list<std::string>& into) const;	\
		void					operator >> (std::list<std::string>& into) const	\
									{ WriteLinesTo(into); }

///////////////////////////////////////////////////////////
#define NMUSTANDARD_STATELESS_OBJECT_METHODS(CLASS)							\
		CLASS (void) {}														\
		CLASS ( CLASS const& ) {}											\
		~CLASS (void) {}													\
		CLASS & operator = ( CLASS const&) { return *this; }				\
		CLASS * Clone (voidcp at, csize_t bs) const							\
					{ return bs >= sizeof(*this)? new(at) CLASS : NULL; }	\
		CLASS *	Clone (void) const { return DNEW( CLASS ); }

///////////////////////////////////////////////////////////
template <typename Type>	struct nmuconst_of				{ typedef Type const						T; };
template <typename Type>	struct nmuconst_of<Type const>	{ typedef typename nmuconst_of<Type>::T		T; };
template <typename Type>	struct nmuconst_of<Type&>		{ typedef typename nmuconst_of<Type>::T&	T; };

///////////////////////////////////////////////////////////
template <typename Type1, typename Type2, typename Type3>
struct nmutripletypes {
	typedef Type1 T1;
	typedef Type2 T2;
	typedef Type3 T3;
};
template <typename TripleTypes>
struct nmuconsttripletypes {
	typedef typename nmuconst_of<typename TripleTypes::T1>::T	T1;
	typedef typename nmuconst_of<typename TripleTypes::T2>::T	T2;
	typedef typename nmuconst_of<typename TripleTypes::T3>::T	T3;
};

#define NMUTRIPLE(CLASS, TYPES, N1, N2, N3)								\
		struct CLASS {													\
			typedef TYPES ::T1		T1;									\
			typedef TYPES ::T2		T2;									\
			typedef TYPES ::T3		T3;									\
			typedef urefto<T1>::t	T1ref;								\
			typedef urefto<T2>::t	T2ref;								\
			typedef urefto<T3>::t	T3ref;								\
			T1 N1; T2 N2; T3 N3;										\
			CLASS (														\
					T1ref _##N1,										\
					T2ref _##N2,										\
					T3ref _##N3):										\
				N1 ( _##N1 ), N2 ( _##N2 ), N3 ( _##N3 ) {}				\
			CLASS ( uconstref_of< CLASS >::t other ):					\
				N1 (other. N1 ), N2 (other. N2 ), N3 (other. N3 ) {}	\
			~CLASS (void) {}											\
			UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR( CLASS )		\
		};																\

#define NMUTRIPLES(CLASS, TYPES, N1, N2, N3)								\
		NMUTRIPLE(CLASS, TYPES, N1, N2, N3)									\
		NMUTRIPLE(Const##CLASS, nmuconsttripletypes< TYPES >, N1, N2, N3)	\

///////////////////////////////////////////////////////////

template <typename Type1, typename Type2>
struct nmutuple2types: public nmutripletypes<Type1, Type2, int> {};

#define NMUTUPLE2(CLASS, TYPES, N1, N2)								\
		struct CLASS {												\
			typedef TYPES ::T1		T1;								\
			typedef TYPES ::T2		T2;								\
			typedef urefto<T1>::t	T1ref;							\
			typedef urefto<T2>::t	T2ref;							\
			T1 N1; T2 N2;											\
			CLASS (													\
					T1ref _##N1,									\
					T2ref _##N2):									\
				N1 ( _##N1 ), N2 ( _##N2 ) {}						\
			CLASS ( uconstref_of< CLASS >::t other):				\
				N1 (other. N1 ), N2 (other. N2 ) {}					\
			~CLASS (void) {}										\
			UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR( CLASS )	\
		};

#define NMUTUPLE2S(CLASS, TYPES, N1, N2)								\
		NMUTUPLE2(CLASS, TYPES, N1, N2)									\
		NMUTUPLE2(Const##CLASS, nmuconsttripletypes< TYPES >, N1, N2)	\

///////////////////////////////////////////////////////////

#define USE(VAL)	static_cast<void>(VAL)

///////////////////////////////////////////////////////////

template <typename T>
static inline
T* callconstructor (T* const ptr)
	{ return new(ptr) T; }

template <typename T>
static inline
T* callconstructor (void* const ptr, T const& arg)
	{ return new(ptr) T(arg); }

template <typename T>
static inline
T* reset (T* const ptr)
	{ ucalldestructor(ptr); return callconstructor(ptr); }

///////////////////////////////////////////////////////////

template <typename T>
static inline
T const* castconst (T* const ptr)
	{ return ptr; }

///////////////////////////////////////////////////////////

template <typename T>
static inline
void* voidcast (T* const ptr)
	{ return ptr; }

template <typename T>
static inline
void const* voidcast (T const* const ptr)
	{ return ptr; }

///////////////////////////////////////////////////////////

template <typename t>
struct TypeOf { typedef t T; };

///////////////////////////////////////////////////////////

template <typename T, const size_t N> static
typename TypeOf<char (*)[N]>::T CountOfHelper ( UNALIGNED T (& arr)[N] );

#define countof(ARR)	( sizeof(*CountOfHelper(ARR)) )

///////////////////////////////////////////////////////////

template <typename T1, typename T2> static inline
void ofequaltypes (T1&, T2& o2)
	{ utypecheck<T1>(o2); }

///////////////////////////////////////////////////////////

template <typename C, typename F> static inline
void foreach (C const& c, F const& f)
	{ std::for_each(c.begin(), c.end(), f); }

template <typename C, typename F> static inline
void foreach (C& c, F const& f)
	{ std::for_each(c.begin(), c.end(), f); }

///////////////////////////////////////////////////////////

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
CharType (&format (CharType (&buf)[N], CharType const* const fmt, ...))[N] {
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

///////////////////////////////////////////////////////////

template <const size_t N>
class cstring {
	char	str[N];
	size_t	i;
public:
// extensions:
	operator	std::string (void) const
					{ return operator const char*(); }

	char*		c_str (void)
					{ return operator char*(); }
	char const*	c_str (void) const
					{ return operator char*(); }

	bool		fits (size_t const length) const
					{ return length <= available(); }
	bool		fits (char const* const _str) const
					{ return fits(strlen(_str)); }

	void		clear (void)
					{ PASSERT(InvariantsHold()) reset(); }

	template <typename StringType>
	cstring&	operator += (StringType const& other)
					{ return append(other); }

	template <typename StringType>
	void		operator = (StringType const& other) {
					if (voidcast(this) != voidcast(&other)) {
						ucalldestructor(this);
						new(this) cstring(other);
					}
				}

// core:
	static const size_t		Length = N;

	cstring&	format (char const* const fmt, ...) {
					{
						va_list args; va_start(args, fmt); {
							vformat(str, fmt, args);
						} va_end(args);
					}

					sync();
					PASSERT(InvariantsHold())
					return *this;
				}

	operator	char* (void)
					{ PASSERT(InvariantsHold()) return &str[0]; }
	operator	char const*	(void) const
					{ PASSERT(InvariantsHold()) return &str[0]; }

	size_t		available (void) const		// always 1 less than Length, to save a place for EOS
					{ PASSERT(InvariantsHold()) return Length - i - 1; }

	cstring&	reset (void)
					{ ucalldestructor(this); return *new(this) cstring; }

	bool		encloses (char const* const ptr) const
					{ PASSERT(InvariantsHold()) return &str[0] <= ptr && ptr <= &str[Length - 1]; }

	bool		InvariantsHold (void) const
					{ return str[i] == '\0' && isinsync(); }

	template <typename StringType>
	cstring&	append (StringType const& other) {
					char const* const	_str	(ucstringarg(other));
					size_t const		_str_len(strlen(_str));
					PASSERT(!encloses(_str) && !encloses(_str + _str_len - 1) && fits(_str_len))

					strncpy_s(&str[i], available() + 1, _str, available() + 1);
					i += _str_len;
					str[i] = '\0';

					PASSERT(InvariantsHold())
					return *this;
				}

	template <typename StringType>
	cstring (StringType const& _str): str(), i(0u) { append(ucstringarg(_str)); }
	cstring (void): str(), i(0u) { PASSERT(InvariantsHold()) }
	~cstring (void) { PASSERT(InvariantsHold()) }

private:
	// Used to restore object in correct state and to query about it.
	// Cannot assert Invariants here because assertion would fail
	// before object is restored to validity or there would be
	// an infinite recursion while querying about object validity.
	void		sync (void)
					{ i = strlen(str); PASSERT(InvariantsHold()) }
	bool		isinsync (void) const
					{ return i == strlen(str); }
};

///////////////////////////////////////////////////////////

#define FOREACH(ARRT, ARR, VARNAME)	\
	for (UPTR( ARRT ) VARNAME (( & ARR [0] )) ; VARNAME < & ARR [ countof( ARR ) ] ; ++ VARNAME )
#define IFOREACH(IT_TYPE, CONT, VARNAME) \
	for (IT_TYPE VARNAME (( CONT ).begin()) ; VARNAME != ( CONT ).end() ; ++ VARNAME)

///////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////

template <typename T>
struct dptr {
	typedef dptr<T>	Self;

	T* ptr;

	T*		operator -> (void) const	{ return DPTR(DNULLCHECK(ptr)); }
	T&		operator * (void) const		{ return *operator ->(); }

	T*		native (void) const			{ return operator ->(); }
	void	Delete (void)				{ udelete(ptr); }
	void	nullify (void)				{ DASSERTPTR(DNULLCHECK(ptr)); unullify(ptr); }
	Self&	New (void)					{ unew(ptr); }
	bool	isnull (void) const			{ return ptr == NULL; }
	T*		discard (void)				{ T* const result(ptr); nullify(); return result; }

	template <void (*Deleter)(T*)>
	void	Delete (void)				{ (*Deleter)(discard()); }
	template <typename Deleter>
	void	Delete (Deleter const& d)	{ d(discard()); }

	template <typename Operation>
	void	UseUp (Operation const& op)	{ op(discard()); }

	explicit dptr (void): ptr(NULL) {}
	explicit dptr (T* const _ptr): ptr(NULL) { operator =(_ptr); }
	dptr (dptr const& p): ptr(p.ptr) {}
	~dptr (void) { DASSERT(ptr == NULL); }

	void operator = (T* const _ptr) { DASSERT(ptr == NULL); ptr = DPTR(DNULLCHECK(_ptr)); }
	UOVERLOADED_VOID_ASSIGN_VIA_COPY_CONSTRUCTOR(dptr)
};

template <typename T>
struct tmpdptr: public dptr<T> {
	typedef dptr<T>		Base;
	typedef tmpdptr<T>	Self;

	explicit tmpdptr (void): Base() {}
	explicit tmpdptr (T* const ptr): Base(ptr) {}
	tmpdptr (Self const& o): Base(o) {}
	tmpdptr (Base const& o): Base(o) {}
	~tmpdptr (void) { DASSERTPTR(DNULLCHECK(discard())); }
};

///////////////////////////////////////////////////////////

static inline std::ostream& WriteToStream (std::ostream& o, std::string const& s)
	{ return o << s << std::endl; }

static inline std::ostream& WriteToStream (std::ostream& o, std::list<std::string> const& lines) {
	foreach(lines, ubind1st(uptr_fun(static_cast<std::ostream& (*const) (std::ostream&, std::string const&)>(&WriteToStream)), o));
	return o;
}

static inline void WriteToFile (char const* const path, std::list<std::string> const& lines) {
	std::ofstream fout(path, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
	PASSERT(fout.good());
	WriteToStream(fout, lines);
}

template <typename LineWriter>
static inline std::ostream& WriteToStream (std::ostream& o, LineWriter const& lw) {
	std::list<std::string> lines;
	return WriteToStream(o, lw.WriteLinesTo(lines));
}

///////////////////////////////////////////////////////////

#endif // __MY_UTIL__MY_UTILS__H__
