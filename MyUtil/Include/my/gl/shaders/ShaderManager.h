#ifndef __MY__GL__SHADER_MANAGER__H__
#define __MY__GL__SHADER_MANAGER__H__

#include <MyUtil.h>
#include <my/gl/shaders/ProgramBuilder.h>

#include <PStaticCString.h>

namespace my { namespace gl { namespace shaders {

    class MYUTIL_API ShaderManager {
    public:
        typedef codeshare::utilities::StaticCString<ProgramBuilder::MAX_SOURCE_TEXT_LENGTH>    SourceText;

        char const*                                        GetVertexShaderSource (void) const;
        char const*                                        GetFragmentShaderSource (void) const;


                                                        ShaderManager (void);
                                                        ~ShaderManager (void);

        bool                                            LoadShadersSources (void);
        bool                                            AreShaderSourcesLoaded (void) const;
        char const*                                        GetLoadingErrorMessage (void) const;
    private:
#        pragma warning( push )
#        pragma warning( disable: 4251 ) // SourceTexts need to have dll-linkage to be used by clients of ShaderManager
        SourceText                                        vertexShaderSource;
        SourceText                                        fragmentShaderSource;
        SourceText                                        loadingErrorMessage;
#        pragma warning( pop )

        void                                            ResetTexts (void);

    private:
        ShaderManager (ShaderManager const&);
        void operator = (ShaderManager const&);
    }; // class ShaderManager

}}} // namespace my::gl::shaders

#endif // __MY__GL__SHADER_MANAGER__H__

