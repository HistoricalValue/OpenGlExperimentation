#include <stdafx.h>

#include <PCString_inl.h>

#include <my/gl/shaders/ShaderManager.h>

namespace _ {

const char* const VERTEX_SHADER_SOURCE_PATH        =    //    "../shaders/vertex.c"
                                                        "../shaders/vertex2.txt"
                                                    ;
const char* const FRAGMENT_SHADER_SOURCE_PATH    =        "../shaders/fragment.c"
                                                    ;

}


namespace my { namespace gl { namespace shaders {
    ///////////////////////////////////////////////////////////////////////////
    // -- File private
    ///////////////////////////////////////////////////////////////////////////
    namespace _ {
        static bool LoadSource (
                char const* const                shaderSourceFilePath,
                ShaderManager::SourceText&        shaderSource,
                ShaderManager::SourceText&        loadingErrorMessage
            ) {
            bool result(false);

            {
                FILE* finp(NULL);
                errno_t fopen_error(fopen_s(&finp, shaderSourceFilePath, "r"));
                if (fopen_error == 0) {
                    PASSERT(finp != NULL)
                    for (bool done(false); !done; ) {
                        char buf[1024];
                        char* fgets_result(fgets(&buf[0], sizeof(buf)/sizeof(buf[0]), finp));
                        if (fgets_result != NULL) {
                            if (!shaderSource.Append(buf)) {
                                done = true; // error: source doesn't fit
                                loadingErrorMessage.Append("\n- Source doesn't fit");
                            }
                        }
                        else { // error or eof
                            done = true;
                            errno_t fgets_error(errno);
                            if (feof(finp))
                                result = true;
                            else {
                                PASSERT(ferror(finp))
                                char buf[1024];
                                errno_t strerror_error(strerror_s(buf, fgets_error));
                                PASSERT(strerror_error == 0)
                                loadingErrorMessage.Append(buf);
                            }
                        }
                    }
                }
                else {
                    char buf[1024];
                    errno_t strerror_error(strerror_s(buf, fopen_error));
                    PASSERT(strerror_error == 0)
                    loadingErrorMessage.Append(buf);
                }
            }

            return result;
        }
    } // namespace _

    ///////////////////////////////////////////////////////////////////////////
    // -- Public
    ///////////////////////////////////////////////////////////////////////////


    //-------------------------------------------------------------------------
    // -- Constructors/Destructor
    ShaderManager::ShaderManager (void):
        vertexShaderSource(),
        fragmentShaderSource(),
        loadingErrorMessage() {
        ResetTexts();
    }

    ///////////////////////////////////

    ShaderManager::~ShaderManager (void) {
        ResetTexts();
    }

    //-------------------------------------------------------------------------
    // -- Modifiers

    bool ShaderManager::LoadShadersSources (void) {
        bool result(true
            && _::LoadSource(::_::VERTEX_SHADER_SOURCE_PATH, vertexShaderSource, loadingErrorMessage)
            && _::LoadSource(::_::FRAGMENT_SHADER_SOURCE_PATH, fragmentShaderSource, loadingErrorMessage)
            );

        return result;
    }


    //-------------------------------------------------------------------------
    // -- Accessors
    ///////////////////////////////////

    char const* ShaderManager::GetVertexShaderSource (void) const {
        return vertexShaderSource.GetValue();
    }

    ///////////////////////////////////

    char const* ShaderManager::GetFragmentShaderSource (void) const {
        return fragmentShaderSource.GetValue();
    }

    ///////////////////////////////////

    bool ShaderManager::AreShaderSourcesLoaded (void) const {
        PASSERT(vertexShaderSource.GetValue()[0] == '\0' || fragmentShaderSource.GetValue()[0] != '\0')
        PASSERT(fragmentShaderSource.GetValue()[0] == '\0' || loadingErrorMessage.GetValue()[0] == '\0')
        PASSERT(loadingErrorMessage.GetValue()[0] != '\0' || vertexShaderSource.GetValue()[0] != '\0')
        return loadingErrorMessage.GetValue()[0] == '\0';
    }

    ///////////////////////////////////

    char const* ShaderManager::GetLoadingErrorMessage (void) const {
        return loadingErrorMessage.GetValue();
    }

    ///////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    // -- Private
    ///////////////////////////////////////////////////////////////////////////
    void ShaderManager::ResetTexts (void) {
        vertexShaderSource.Reset();
        fragmentShaderSource.Reset();
        loadingErrorMessage.Reset();
    }

}}} // namespace my::gl::shaders
