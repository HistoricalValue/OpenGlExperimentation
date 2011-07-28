#ifndef __MY_UTIL__MY__GL__SHAPES__VERTEX__H__
#define __MY_UTIL__MY__GL__SHAPES__VERTEX__H__

#include "MyUtil.h"
#include "my/gl/math/Vector4.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Vertex: public Transformable {
	public:
		explicit										Vertex (math::Vector4 const& position);
														Vertex (Vertex const&);
		virtual											~Vertex (void);

		void											operator = (Vertex const&);
		Vertex											operator + (Vertex const&) const;

		float											x (void) const;
		float											y (void) const;
		float											z (void) const;
		float											w (void) const;
		math::Vector4 const&							xyzw (void) const;

		// Transformable
		void											Apply (math::Matrix4x4 const&);
		void											Adjust (math::Vector4 const&);
	private:

		math::Vector4									vec;
	}; // class Vertex

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__VERTEX__H__
