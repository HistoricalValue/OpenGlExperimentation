#ifndef __MY_UTIL__MY_OPEN_GL_UTILS_INL__H__
#define __MY_UTIL__MY_OPEN_GL_UTILS_INL__H__

#include <MyOpenGLUtils.h>
#include <MyUtil.h>

namespace my {

    namespace openglutil {

#if defined( P_INLINING ) && defined( __MY_OPEN_GL_UTILS__DEFINING_CONST_STRINGS )
#    error "Inlining and also defining GLConstString() functions"
#endif

#if defined( P_INLINING ) || defined( __MY_OPEN_GL_UTILS__DEFINING_CONST_STRINGS )
    // Stringification for compile-time constants

#    define __MY_OPEN_GL_UTILS__GL_CONSTANT_STRING_DEFINITION(FNAME, KONST)    \
            template<> TCHAR const* FNAME < KONST > (void) { return _T(#KONST); }





    // GL-Consts
#    define __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(KONST)            \
            __MY_OPEN_GL_UTILS__GL_CONSTANT_STRING_DEFINITION(GlConstString, KONST)

    __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(MY_GL_ALPHA_TEST)
    __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(MY_GL_AUTO_NORMAL)
    __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(MY_GL_BLEND)

    __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(MY_GL_RGB)
    __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(MY_GL_BGR)
    __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(MY_GL_BGRA)
    __MY_OPEN_GL_UTILS__GL_CONST_STRING_DEFINITION(MY_GL_LUMINANCE)





    // GL-Errors
#    define __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(GLERR)            \
            __MY_OPEN_GL_UTILS__GL_CONSTANT_STRING_DEFINITION(GlErrorString, GLERR)

    __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(MY_GL_INVALID_ENUM)
    __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(MY_GL_INVALID_VALUE)
    __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(MY_GL_INVALID_OPERATION)
    __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(MY_GL_NO_ERROR)
    __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(MY_GL_STACK_OVERFLOW)
    __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(MY_GL_STACK_UNDERFLOW)
    __MY_OPEN_GL_UTILS__GL_ERROR_STRiNG_DEFINITION(MY_GL_OUT_OF_MEMORY)



    enum GlConst ToGlConst (GLenum const& glenum) {
        return static_cast<enum GlConst>(glenum);
    }

#endif

    } // namespace openglutil
} // namespace my

#endif // __MY_UTIL__MY_OPEN_GL_UTILS_INL__H__

