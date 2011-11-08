#ifndef DRAWING_UTILS_H
#define DRAWING_UTILS_H

#include <PAssert.h>
#include <SurfacesTools.h>
#include <my/gl/adapters/BufferManager.h>
#include <my/gl/shapes/Line.h>
#include <my/gl/shapes/Triangle.h>
#include <MySafeCast.h>
#include <MyUtils.h>
#pragma warning( push, 0 )
#	include <cstdio>
#	include <ComputeMeshAmbientOcclusion.h>
#	include <functional>
#	include <usystem.h>
#pragma warning( pop )

namespace {

template <typename SurfaceType> static inline std::vector<ankh::nurbs::Unit> const KnotsI (SurfaceType const& s)
	{ return std::vector<ankh::nurbs::Unit>(s.GetKnotsIBegin(), s.GetKnotsIEnd()); }

static inline
my::gl::math::Vector4 makevector4 (ankh::math::trig::vec4 const& v)
	{ return my::gl::math::Vector4::New(v.x, v.y, v.z, v.w); }

static inline
my::gl::math::Vector4 makevector4fromvertexwithnormal (ankh::nurbs::VertexWithNormal const& vwn)
	{ return my::gl::math::Vector4::New(vwn.xyz.x, vwn.xyz.y, vwn.xyz.z, 1.0f); }


static inline
my::gl::shapes::Vertex makevertex (ankh::math::types::Vertex const& v)
	{ return my::gl::shapes::Vertex(my::gl::math::Vector4::New(v.x, v.y, v.z, 1.0f)); }

static inline
my::gl::shapes::Line makenormallineformeshelementvertex (ankh::shapes::MeshElement const& v, size_t const i) {
	using namespace ankh::math::trig;

	vec3 const line_begin(v.GetVertex(psafecast<util_ui8>(i)));
	vec3 n(v.GetNormal(psafecast<util_ui8>(i)));
	normalise(&n);
	n *= 0.07f;
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

	return my::gl::shapes::Triangle(c)	.SetA(makevertex(e.a))
										.SetB(makevertex(e.b))
										.SetC(makevertex(e.c))
										.SetNormals(
												makevector4(e.GetNormal(0)),
												makevector4(e.GetNormal(1)),
												makevector4(e.GetNormal(2)))
										.SetAO(0, e.HasAmbientOcclusion()? e.GetAmbientOcclusion(0) : 1.0f)
										.SetAO(1, e.HasAmbientOcclusion()? e.GetAmbientOcclusion(1) : 1.0f)
										.SetAO(2, e.HasAmbientOcclusion()? e.GetAmbientOcclusion(2) : 1.0f)
										.SetTextCoord(0, e.HasTextureCoords()? e.GetTextureCoords(0).x : 0.0f, e.HasTextureCoords()? e.GetTextureCoords(0).y : 0.0f)
										.SetTextCoord(1, e.HasTextureCoords()? e.GetTextureCoords(1).x : 1.0f, e.HasTextureCoords()? e.GetTextureCoords(1).y : 0.0f)
										.SetTextCoord(2, e.HasTextureCoords()? e.GetTextureCoords(2).x : 0.0f, e.HasTextureCoords()? e.GetTextureCoords(2).y : 1.0f)
										;
}

template <typename T>
struct elcollector {
	std::list<T> els;
	elcollector& operator , (T const& el) { els.push_back(el); return *this; }
};

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

struct IneffectiveBufferEntryDeleter: ::my::gl::adapters::Buffer::Deleter {
	void operator () (GLvoid*) {}
};

} //

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
