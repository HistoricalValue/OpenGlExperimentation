#include <stdafx.h>


#define __UNLESS_GL_ERROR	\
		if (_::GlErrorsHandled(&_::ErrorHandler)) {} else

namespace _ {

	using my::openglutil::GlErrorsHandled;

	static void ErrorHandler (LPCTSTR const error) {
		PASSERT(false)
	}

	static GLenum TranslateShaderType (my::gl::shaders::ShaderCompiler::ShaderType const& type) {
		GLenum result(-1);
		if (type == my::gl::shaders::ShaderCompiler::ShaderType_FragmentShader)
			result = GL_FRAGMENT_SHADER;
		else
		if (type == type == my::gl::shaders::ShaderCompiler::ShaderType_VertexShader)
			result = GL_VERTEX_SHADER;
		else
			{}

		PASSERT(result != GLenum(-1))
		return result;
	}

} // namespace _

namespace my { namespace gl { namespace shaders {


	using my::gl::extensions::ExtensionManager::glCreateShader;
	using my::gl::extensions::ExtensionManager::glCompileShader;
	using my::gl::extensions::ExtensionManager::glGetShaderiv;
	using my::gl::extensions::ExtensionManager::glGetShaderInfoLog;
	using my::gl::extensions::ExtensionManager::glIsShader;
	using my::gl::extensions::ExtensionManager::glShaderSource;

	bool ShaderCompiler::Compile (char const* source, ShaderType const type) {
		PASSERT(!HasCompiled())
		hasCompiled = true;

		bool result (false);
		shader = glCreateShader(_::TranslateShaderType(type));
		__UNLESS_GL_ERROR {
			GLint length = strlen(source);
			glShaderSource(shader, 1, &source, &length);
			__UNLESS_GL_ERROR {
				glCompileShader(shader);
				__UNLESS_GL_ERROR {
					// Check for compilation success
					GLint compilationSuccessful(GL_FALSE);
					glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccessful);
					__UNLESS_GL_ERROR {
						if (compilationSuccessful == GL_TRUE) {
							result = true;
							errorMessage = "NO ERROR. Compilation successful.";
							errorMessage += openglutil::GetShaderInfoLog(shader);
							openglutil::ReleaseShaderInfoLog();
						}
						else {
							// Get compilation error reason
							GLint _infologLength(0);
							glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &_infologLength);
							__UNLESS_GL_ERROR {
								PASSERT(_infologLength > 0)
								GLsizei const infologLength(_infologLength);
								PASSERT(infologLength > 0)
								GLsizei actuallyWritten(0);
								GLchar* const infolog = new GLchar[infologLength + 1];
								glGetShaderInfoLog(shader, infologLength, &actuallyWritten, &infolog[0]);
								__UNLESS_GL_ERROR {
									errorMessage = std::string("Compilation failed. Reason: ")
										+ infolog;
									result = false;
								}
							}
						}
					}
				}
			}
		}

		return compilationSuccessful = result;
	}

	bool ShaderCompiler::IsCompilationSuccessful (void) const {
		return compilationSuccessful;
	}

	bool ShaderCompiler::HasCompiled (void) const {
		return hasCompiled;
	}

	GLuint ShaderCompiler::GetShader (void) const {
		PASSERT(HasCompiled())
		PASSERT(IsCompilationSuccessful())
		PASSERT(glIsShader(shader))
		return shader;
	}

	std::string const& ShaderCompiler::GetErrorMessage (void) const {
		return errorMessage;
	}

	ShaderCompiler::ShaderCompiler (void):
		errorMessage("Hasn't compiled yet"),
		compilationSuccessful(false),
		shader(),
		hasCompiled(false)
		{ }

	ShaderCompiler::ShaderCompiler (ShaderCompiler const& other):
		errorMessage(other.errorMessage),
		compilationSuccessful(other.compilationSuccessful),
		shader(other.shader),
		hasCompiled(other.hasCompiled)
		{ }

	ShaderCompiler::~ShaderCompiler (void) {
	}


}}} // namespace my::gl::shaders

