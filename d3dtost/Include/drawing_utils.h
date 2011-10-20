#ifndef DRAWING_UTILS_H
#define DRAWING_UTILS_H

#include <PAssert.h>
#include <SurfacesTools.h>
#include <my/gl/adapters/BufferManager.h>
#include <my/gl/shapes/Line.h>
#include <my/gl/shapes/Triangle.h>
#include <MySafeCast.h>
#pragma warning( push, 0 )
#	include <cstdio>
#	include <ComputeMeshAmbientOcclusion.h>
#	include <functional>
#	include <usystem.h>
#pragma warning( pop )

namespace {

template <typename SurfaceType> static inline std::vector<ankh::nurbs::Unit> const KnotsI (SurfaceType const& s)
	{ return std::vector<ankh::nurbs::Unit>(s.GetKnotsIBegin(), s.GetKnotsIEnd()); }

template <typename T>
T const* castconst (T* ptr) { return static_cast<T const* const>(ptr); }

template <typename t>
struct TypeOf { typedef t T; };

template <typename T, const size_t N>
typename TypeOf<char (*)[N]>::T CountOfHelper ( UNALIGNED T (& arr)[N] );

#define countof(ARR)	sizeof(*CountOfHelper(ARR))

template <typename T1, typename T2> static inline
void ofequaltypes (T1&, T2& o2)
	{ utypecheck<T1>(o2); }

template <typename C, typename F> static
void foreach (C const& c, F const& f)
	{ std::for_each(c.begin(), c.end(), f); }

static inline
my::gl::math::Vector4 makevector4 (ankh::math::trig::vec4 const& v)
	{ return my::gl::math::Vector4::New(v.x, v.y, v.z, v.w); }

static inline
my::gl::math::Vector4 makevector4fromvertexwithnormal (ankh::nurbs::VertexWithNormal const& vwn)
	{ return my::gl::math::Vector4::New(vwn.v.x, vwn.v.y, vwn.v.z, 1.0f); }


static inline
my::gl::shapes::Vertex makevertex (ankh::math::types::Vertex const& v)
	{ return my::gl::shapes::Vertex(my::gl::math::Vector4::New(v.x, v.y, v.z, 1.0f)); }

static inline
my::gl::shapes::Line makenormallineformeshelementvertex (ankh::shapes::MeshElement const& v, size_t const i) {
	using namespace ankh::math::trig;

	vec3 const line_begin(v.GetVertex(psafecast<util_ui8>(i)));
	vec3 n(v.GetNormal(psafecast<util_ui8>(i)));
	normalise(&n);
	n *= 0.0025f;
	vec3 const line_end(line_begin + n);

	return my::gl::shapes::Line(makevertex(line_begin), makevertex(line_end),
		my::gl::shapes::ColourFactory::Dimmer(
			my::gl::shapes::ColourFactory::Red()
		)
	);
}

static inline
my::gl::shapes::Triangle maketriangle (ankh::shapes::MeshElement const& e, my::gl::shapes::Colour const& c) {
	using ankh::math::types::Vertex;
	DASSERT(e.HasNormals());
	DASSERT(e.HasAmbientOcclusion());

	return my::gl::shapes::Triangle(c)	.SetA(makevertex(e.a))
										.SetB(makevertex(e.b))
										.SetC(makevertex(e.c))
										.SetNormals(
												makevector4(e.GetNormal(0)),
												makevector4(e.GetNormal(1)),
												makevector4(e.GetNormal(2)))
										.SetAO(0, e.GetAmbientOcclusion(0))
										.SetAO(1, e.GetAmbientOcclusion(1))
										.SetAO(2, e.GetAmbientOcclusion(2))
										;
}

template <typename T>
struct elcollector {
	std::list<T> els;
	elcollector& operator , (T const& el) { els.push_back(el); return *this; }
};

template <typename CharType>
static
CharType const* format (CharType const* const fmt, ...) {
	static CharType buf[1024];

	va_list args;
	va_start(args, fmt);

	int const retval(_vsntprintf_s(&buf[0], countof(buf), countof(buf), fmt, args));
	PASSERT(retval > 1);

	va_end(args);

	return &buf[0];
}

struct timer {
	char const* const what;
	unsigned long int start, diff;
	bool done;
	struct Callback { virtual void operator () (timer const&) const = 0; virtual ~Callback (void){} };
	Callback const* const callback;

	timer (char const* const _what, Callback const* const _callback = NULL):
		what(_what),
		start(ugettime()),
		diff(unsigned(-1)),
		done(false),
		callback(_callback) {}

	void restart (void) { start = ugettime(); }

	~timer (void) {
		done = true;
		diff = ugettime() - start;

		char buf[1024];
		_snprintf_s(&buf[0], _countof(buf), _countof(buf), "%s took %ld milliseconds\n",
				what, diff);
		my::global::log::infoA(buf);

		if (callback)
			(*callback)(*this);
	}

private:
	void operator = (timer const&);
};

template <const bool inv, typename T> struct inverser;

template <typename T> struct inverser<false, T>	{
	T const& a, b;
	inverser (T const& _a, T const& _b): a(_a), b(_b) {}
	inverser (inverser<false, T> const& o): a(o.a), b(o.b) {}
private:
	void operator = (inverser<false, T> const&);
};

template <typename T> struct inverser<true, T>	{
	T const& a, b;
	inverser (T const& _a, T const& _b): a(_b), b(_a) {}
	inverser (inverser<true, T> const& o): a(o.a), b(o.b) {}
private:
	void operator = (inverser<true, T> const&);
};

template <const bool inv, typename T> inverser<inv, T> const makeinverser (T const& a, T const& b) { return inverser<inv,T>(a, b); }

template <typename T>
class Range {
public:
						Range (void) {}
						Range (Range const&) {}
	virtual				~Range (void) {}

	virtual	bool		lt (T const& a, T const& b) const	{ return a < b; }
	virtual bool		eq (T const& a, T const& b) const	{ return a == b; }
			bool		le (T const& a, T const& b) const	{ return lt(a, b) || eq(a, b); }
			bool		gt (T const& a, T const& b) const	{ return !le(a, b); }
			bool		ge (T const& a, T const& b) const	{ return gt(a, b) || eq(a, b); }
};

template <typename T>
class InverseRange: public Range<T> {
public:
	typedef	Range<T>	Base;
						InverseRange (void): Base() {}
						InverseRange (InverseRange const& o): Base(o) {}
	virtual				~InverseRange (void) {}

	virtual	bool		lt (T const& a, T const& b) const	{ return Base::lt(b, a); }
};


struct IneffectiveBufferEntryDeleter: ::my::gl::adapters::Buffer::Deleter {
	void operator () (GLvoid*) {}
};

} //

#define FOREACH(ARRT, ARR, VARNAME)	\
	for (UPTR( ARRT ) VARNAME (( & ARR [0] )) ; VARNAME < & ARR [ countof( ARR ) ] ; ++ VARNAME )

namespace {
static inline void ExtractAmbientOcclusions (std::vector<float>& into, ankh::shapes::Mesh const& m) {
	DASSERT(into.empty());
	into.reserve(m.GetElements().size());

	ankh::shapes::Mesh::Elements::const_iterator const elements_end(m.GetElements().end());

	for (ankh::shapes::Mesh::Elements::const_iterator i(m.GetElements().begin()); i != elements_end; ++i) {
		DASSERT(i->HasAmbientOcclusion());
		into.push_back(i->GetAmbientOcclusion(0u));
		into.push_back(i->GetAmbientOcclusion(1u));
		into.push_back(i->GetAmbientOcclusion(2u));
	}
}

template <typename FloatIterType>
class EqualityAsserterAOCreator: public ankh::shapes::Mesh::AmbientOcclusionCreator {
public:
	virtual void operator () (ankh::shapes::MeshElement const& el, float (*&ao)[3]) const {
		delegatee(el, ao);
		DASSERT(aos != aos_end && std::equal_to<float>()((*ao)[0], *aos++));
		DASSERT(aos != aos_end && std::equal_to<float>()((*ao)[1], *aos++));
		DASSERT(aos != aos_end && std::equal_to<float>()((*ao)[2], *aos++));
	}

	EqualityAsserterAOCreator (ankh::shapes::Mesh::AmbientOcclusionCreator const* const _delegatee, FloatIterType const& _aos, FloatIterType const& _aos_end):
		delegatee	(*DNULLCHECK(_delegatee)),
		aos			(_aos),
		aos_end		(_aos_end)
		{}

	EqualityAsserterAOCreator*	Clone (void) const
		{ return DNEWCLASS(EqualityAsserterAOCreator, (&delegatee, aos, aos_end)); }

private:
	ankh::shapes::Mesh::AmbientOcclusionCreator const&	delegatee;
	mutable FloatIterType								aos;
	FloatIterType const									aos_end;

	EqualityAsserterAOCreator (EqualityAsserterAOCreator const&);
	void operator = (EqualityAsserterAOCreator const&);
};

} //

#endif
