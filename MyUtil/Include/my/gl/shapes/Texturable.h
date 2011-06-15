#ifndef __MY_UTIL__MY__GL__SHAPES__TEXTURABLE__H__
#define __MY_UTIL__MY__GL__SHAPES__TEXTURABLE__H__

#include "MyUtil.h"

namespace my { namespace gl { namespace shape {

	class MYUTIL_API Texturable {
	public:
		virtual											~Texturable (void);

	protected:
														Texturable (void);
	}; // class Texturable

}}} // namespace my::gl::shape

#endif // __MY_UTIL__MY__GL__SHAPES__TEXTURABLE__H__
