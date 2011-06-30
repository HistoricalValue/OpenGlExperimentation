#ifndef __MY_UTIL__MY__GL__SHAPES__NOTHING__H__
#define __MY_UTIL__MY__GL__SHAPES__NOTHING__H__

#include "MyUtil.h"
#include "my/gl/shapes/Shape.h"
#include "PConfigurations.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Nothing: public Shape {
	public:
		P_INLINE 										Nothing (void);
		P_INLINE 										Nothing (Nothing const&);
		P_INLINE virtual								~Nothing (void);

		P_INLINE VertexData*							GetVertexData (void* memory, size_t bytesize) const;
		P_INLINE Nothing*								Clone (void* memory, size_t bytesize) const;
		P_INLINE size_t									GetSizeOf (void) const;
		P_INLINE size_t									GetNumberOfVertices (void) const;
		P_INLINE void									Apply (math::Matrix4x4 const&);
		P_INLINE void									Adjust (math::Vector4 const&);	
	}; // class Nothing

}}} // namespace my::gl::shapes


#endif // __MY_UTIL__MY__GL__SHAPES__NOTHING__H__
