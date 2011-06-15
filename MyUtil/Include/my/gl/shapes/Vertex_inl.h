#ifndef __MY_UTIL__MY__GL__SHAPES__VERTEX_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__VERTEX_INL__H__

#include "my/gl/shapes/Vertex.h"
#include "PConfigurations.h"
#include "my/gl/math/Vector4_inl.h"

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__VERTEX__DEFINING)


namespace my { namespace gl { namespace shapes {

	P_INLINE
	Vertex::Vertex (math::Vector4 const& position):
		Transformable(),
		vec(position)
		{ P_STATIC_ASSERT(sizeof(Vertex) == sizeof(Transformable) + sizeof(vec)) }

	P_INLINE
	Vertex::Vertex (Vertex const& other):
		Transformable(other),
		vec(other.vec)
		{ P_STATIC_ASSERT(sizeof(Vertex) == sizeof(Transformable) + sizeof(vec)) }
		
	P_INLINE
	Vertex::~Vertex (void) {
	}

	P_INLINE
	float Vertex::x (void) const {
		return vec.x();
	}

	P_INLINE
	float Vertex::y (void) const {
		return vec.y();
	}

	P_INLINE
	float Vertex::z (void) const {
		return vec.z();
	}

	P_INLINE
	float Vertex::w (void) const {
		return vec.z();
	}

	P_INLINE
	math::Vector4 const& Vertex::xyzw (void) const {
		return vec;
	}

	// Transformable
	P_INLINE
	void Vertex::Apply (math::Matrix4x4 const& mat4) {
		vec = mat4 * vec;
	}

	P_INLINE
	void Vertex::Adjust (math::Vector4 const& off) {
		vec = vec + off;
	}
	
	P_INLINE
	void Vertex::operator = (Vertex const& other) {
		this->~Vertex();
		new(this) Vertex(other);
	}
	
	P_INLINE
	Vertex Vertex::operator + (Vertex const& other) const {
		return Vertex(vec - other.vec);
	}

}}} // namespace my::gl::shapes


#endif

#endif // __MY_UTIL__MY__GL__SHAPES__VERTEX_INL__H__
