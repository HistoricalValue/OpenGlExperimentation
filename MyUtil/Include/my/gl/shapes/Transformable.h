#ifndef __MY_UTIL__MY__GL__SHAPES__TRANSFORMABLE__H__
#define __MY_UTIL__MY__GL__SHAPES__TRANSFORMABLE__H__

#include "MyUtil.h"
#include "my/gl/math/Matrix4x4.h"
#include "my/gl/math/Vector4.h"

namespace my { namespace gl { namespace shapes {

	class MYUTIL_API Transformable {
	public:

		void											Scale (float factor);
		void											ScaleX (float factor);
		void											ScaleY (float factor);
		void											ScaleZ (float factor);

		void											RotateX (float radians);
		void											RotateY (float radians);
		void											RotateZ (float radians);

		void											TranslateX (float d);
		void											TranslateY (float d);
		void											TranslateZ (float d);


		virtual											~Transformable (void);

		virtual void									Apply (math::Matrix4x4 const&) = 0;
		virtual void									Adjust (math::Vector4 const&) = 0;

	protected:
														Transformable (void);
														Transformable (Transformable const&);
	private:
		void											operator = (Transformable const&);

	}; // class Transformable

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__TRANSFORMABLE__H__
