#ifndef DRAWING_UTILS_H
#define DRAWING_UTILS_H

#include <cstdio>
#include <PAssert.h>

#include <SurfacesTools.h>

namespace {

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
	
	vec3 const line_begin(v.GetVertex(i));
	vec3 n(v.GetNormal(i));
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

LPCTSTR format (LPCTSTR const fmt, ...) {
	static TCHAR buf[1024];

	va_list args;
	va_start(args, fmt);

	int const retval(_vsntprintf_s(&buf[0], _countof(buf), _countof(buf), fmt, args));
	PASSERT(retval > 1);

	va_end(args);

	return &buf[0];
}

struct timer {
	char const* const what;
	unsigned long int start;
	timer (char const* const _what): what(_what), start(ugettime()) {}
	void restart (void) { start = ugettime(); }
	~timer (void) {
		char buf[1024];
		_snprintf_s(&buf[0], _countof(buf), _countof(buf), "%s took %ld milliseconds\n",
				what, ugettime() - start);
		my::global::log::infoA(buf);
	}
};

template <const bool inv, typename T> struct inverser;
template <typename T> struct inverser<false, T>	{ T const& a, b; inverser (T const& _a, T const& _b): a(_a), b(_b) {} };
template <typename T> struct inverser<true, T>	{ T const& a, b; inverser (T const& _a, T const& _b): a(_b), b(_a) {} };
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

} //

#endif
