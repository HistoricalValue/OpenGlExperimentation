#ifndef __MY_UTIL__MY__GL__SHAPES__N_SHAPES_COMPOSITION__H__
#define __MY_UTIL__MY__GL__SHAPES__N_SHAPES_COMPOSITION__H__

#include <MyUtil.h>
#include <my/gl/shapes/SpecificNumberOfShapesComposition.h>

namespace my { namespace gl { namespace shapes {

template <const size_t N>
// abstract
class NShapesComposition: public SpecificNumberOfShapesComposition {
public:
	virtual size_t					GetSizeOf (void) const;
	virtual NShapesComposition*		Clone (void*, size_t) const;	// Clone and GetSizeOf() added for enabling making of tmp objects.
																	// They are not allowed for this class, results in runtime failure.

									NShapesComposition (void);
	virtual							~NShapesComposition (void);

// TODO add in-object storage for Shape*[N]
}; // class NShapesComposition

}}} // namespace my::gl::shapes

#endif // __MY_UTIL__MY__GL__SHAPES__N_SHAPES_COMPOSITION__H__
