#ifndef __MY_UTIL__MY__GL__SHAPES__NOTHING_INL__H__
#define __MY_UTIL__MY__GL__SHAPES__NOTHING_INL__H__

#include <my/gl/shapes/Nothing.h>

#include <PConfigurations.h>
#include <PStaticAssert.h>
#include <PPointerUtilities_inl.h>
#include <my/gl/shapes/ColourFactory.h>

namespace my { namespace gl { namespace shapes {

#if defined(P_INLINING) || defined(__MY_UTIL__MY__GL__SHAPES__NOTHING__DEFINING)

	P_INLINE
	Nothing::Nothing (void):
		Shape(ColourFactory::Red())
		{ P_STATIC_ASSERT(sizeof(Nothing) == sizeof(Shape)) }

	P_INLINE
	Nothing::Nothing (Nothing const& other):
		Shape(other)
		{ P_STATIC_ASSERT(sizeof(Nothing) == sizeof(Shape)) }

	P_INLINE
	Nothing::~Nothing (void) {
	}

	P_INLINE
	VertexData* Nothing::GetVertexData (void* const memory, size_t const bytesize) const {
		return reinterpret_cast<VertexData* const>(memory);
	}

	P_INLINE
	Nothing* Nothing::Clone (void* const memory, size_t const bytesize) const {
		Nothing* result(NULL);

		if (bytesize >= sizeof(*this)) {
			result = codeshare::utilities::pointer_utilities::reinterpret_assign(result, memory);
			new(result) Nothing(*this);
		}

		return result;
	}

	P_INLINE
	size_t Nothing::GetSizeOf (void) const {
		return sizeof(Nothing);
	}

	P_INLINE
	size_t Nothing::GetNumberOfVertices (void) const {
		return 0;
	}

	P_INLINE
	void Nothing::Apply (math::Matrix4x4 const&) {
	}

	P_INLINE
	void Nothing::Adjust (math::Vector4 const&) {
	}

#endif

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__NOTHING_INL__H__
