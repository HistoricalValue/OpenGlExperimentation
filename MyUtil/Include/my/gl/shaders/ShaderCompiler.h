#ifndef __MY__GL__SHADRES__SHADER_COMPILER__H__
#define __MY__GL__SHADRES__SHADER_COMPILER__H__

#include <MyUtil.h>

namespace my { namespace gl { namespace shaders {


#pragma warning( push )
#pragma warning( disable: 4251 ) // std::string<> needs dll-interface for clients of ShaderCompiler

	class MYUTIL_API ShaderCompiler {
	public:
		typedef enum {									ShaderType_VertexShader		= 0x0001u,
														ShaderType_FragmentShader	= 0x0004u }
														ShaderType;

		bool											HasCompiled (void) const;
														// => "success?"
		bool											Compile (char const* source, ShaderType type);
		bool											IsCompilationSuccessful (void) const;
		std::string const&								GetErrorMessage (void) const;

		GLuint											GetShader (void) const;

														ShaderCompiler (void);
														ShaderCompiler (ShaderCompiler const&);
														~ShaderCompiler (void);
	private:
		std::string										errorMessage;
		bool											compilationSuccessful;
		GLuint											shader;
		bool											hasCompiled;
	}; // class ShaderCompiler

#pragma warning( pop ) // 4251

}}} // namespace my::gl::shaders

#endif // __MY__GL__SHADRES__SHADER_COMPILER__H__
