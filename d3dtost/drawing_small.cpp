#include "stdafx.h"
#include <nmutil/OpenGlExtensionManager.h>
#include <MyOpenGL.h>
#pragma warning( push, 0 )
#    include <DDebug.h>
#pragma warning( pop )

using namespace ::gl::ext;

namespace _ {
    static inline void NoGlError (void) {
        GLuint error = glGetError();
        DASSERT(error == GL_NO_ERROR);
    }
}

namespace gl { namespace ext {
    void glextClear (GLbitfield bits) {
        ::glClear(bits);
        _::NoGlError();
    }

    void glextEnable (GLenum what) {
        glEnable(what);
        _::NoGlError();
    }

    void glextCullFace (GLenum face) {
        glCullFace(face);
        _::NoGlError();
    }
}}


namespace _ {
    using codeshare::utilities::pointer_utilities::offset;

    struct DD {
        static const size_t w = 12u;
        GLuint vao;
        GLuint vbo;
        unsigned short n;
    };

    namespace setup {
        static inline void clearColor (DD&) { const float c = 0.9f; glClearColor(c, c, c, 1.0f); }
        static inline void vertexArray (DD& dd) {
            glGenVertexArrays(1, &dd.vao);
        }
        static inline void vertexArrayBuffer (DD& dd) {
            glGenBuffers(1, &dd.vbo);
        }
        static inline void vertexData (DD& dd) {
            float vd[] = {
                0.5f, 0.5f, 0.5f, 1.0f,
                0.5f, 0.2f, 0.2f, 1.0f,

                -0.5f, 0.5f, 0.5f, 1.0f,
                0.2f, 0.5f, 0.2f, 1.0f,

                0.5f, -0.5f, 0.5f, 1.0f,
                0.2f, 0.2f, 0.5f, 1.0f,
                ///////////////////////
                0.6f, 0.5f, 0.5f, 1.0f,
                0.5f, 0.2f, 0.2f, 1.0f,

                0.6f, 0.4f, 0.5f, 1.0f,
                0.2f, 0.5f, 0.2f, 1.0f,

                0.7f, 0.5f, 0.5f, 1.0f,
                0.2f, 0.2f, 0.5f, 1.0f,
                ///////////////////////
                0.7f, 0.3f, 0.5f, 1.0f,
                0.5f, 0.2f, 0.2f, 1.0f,

                0.7f, 0.2f, 0.5f, 1.0f,
                0.2f, 0.5f, 0.2f, 1.0f,

                0.6f, 0.3f, 0.5f, 1.0f,
                0.2f, 0.2f, 0.5f, 1.0f,
                ///////////////////////
                0.3f, 0.3f, 0.4f, 1.0f,
                0.2f, 0.2f, 0.2f, 0.7f,

                0.4f, 0.3f, 0.4f, 1.0f,
                0.2f, 0.2f, 0.2f, 0.7f,

                0.4f, 0.4f, 0.4f, 1.0f,
                0.2f, 0.2f, 0.2f, 0.7f
            };

            dd.n = _countof(vd) / 8;
            DASSERT(_countof(vd) % 8 == 0);

            glBindVertexArray(dd.vao);
            glBindBuffer(GL_ARRAY_BUFFER, dd.vbo);

            DASSERT(glIsVertexArray(dd.vao) == GL_TRUE);
            DASSERT(glIsBuffer(dd.vbo) == GL_TRUE);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vd), &vd[0], GL_STATIC_DRAW);

            glVertexAttribPointer(my::OpenGL::VAI_POSITION, 4, GL_FLOAT, GL_TRUE, sizeof(float) * 8, 0);
            glVertexAttribPointer(my::OpenGL::VAI_COLOUR, 4, GL_FLOAT, GL_TRUE, sizeof(float) * 8, offset(sizeof(float) * 4));

            glEnableVertexAttribArray(my::OpenGL::VAI_POSITION);
            glEnableVertexAttribArray(my::OpenGL::VAI_COLOUR);
        }
        static inline void uniforms (DD&) {
            glUniform1ui(my::OpenGL::VUL_COLSELTR, GLuint(2u));
        }
        static inline void settings (DD&) {
            glextEnable(GL_CULL_FACE);
            glextEnable(GL_DEPTH_TEST);
            glextEnable(GL_TEXTURE_3D);

        //    glextCullFace(GL_FRONT);
        }
    }
}

namespace my { namespace drawing {

void draw (void*, void (*) (void*), void*);
void draw (void* const _dd, void (*swap) (void*), void* const swap_data) {
    _::DD* const dd(reinterpret_cast<_::DD*>(_dd));
    DASSERT(dd->w == 12u);

    glextClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(dd->vao);
    glDrawArrays(GL_TRIANGLES, 0, dd->n);

    (*swap)(swap_data);
}

void* setup (void) {
    _::DD* const dd(DNEW(_::DD));

    _::setup::clearColor(*dd);
    _::setup::vertexArray(*dd);
    _::setup::vertexArrayBuffer(*dd);
    _::setup::vertexData(*dd);
    _::setup::uniforms(*dd);
    _::setup::settings(*dd);

    return dd;
}

void cleanup (void*& dd)
    { udelete(reinterpret_cast<_::DD*&>(dd)); }

}} // my::drawing
