#include "stdafx.h"
#include "PCString_inl.h"

namespace my { namespace gl { namespace shaders {



	// --------------------------------
#	define __UNLESS_GL_ERROR	\
		if (!O_O::GlErrorsHandled(&O_O::DefaultGlErrorHandler))



	// --------------------------------
	typedef char ErrorMessage[MY__GL__SHADERS__PROGRAM_BUILDER__MAX_SOURCE_TEXT_LENGTH];



	// --------------------------------
	using extensions::ExtensionManager::glCreateProgram;
	using extensions::ExtensionManager::glAttachShader;
	using extensions::ExtensionManager::glLinkProgram;
	using extensions::ExtensionManager::glGetProgramiv;
	using extensions::ExtensionManager::glDetachShader;
	using extensions::ExtensionManager::glDeleteShader;
	using extensions::ExtensionManager::glBindAttribLocation;
	using extensions::ExtensionManager::glUseProgram;


	// --------------------------------
	// Local functions
	namespace O_O {
		static void ConstructErrorMessageWithSuberrorSubmessage (char* dst, size_t dstLength, char const* myMessage, char const* causeMessage);
		static bool CompileShader (
				GLuint&							shader,
				char const* const				source,
				ErrorMessage					errorMessage,
				char const						myShaderType[9],
				ShaderCompiler::ShaderType,
				ProgramBuilder::SourceText		infoLog,
				size_t&							infoLogHead);
		using openglutil::GlErrorsHandled;
		static void DefaultGlErrorHandler (LPCTSTR);
		static bool CreateProgramAndLinkShaders (GLuint const program, GLuint vertexShader, GLuint fragmentShader, ErrorMessage);

		using codeshare::csconcat;
	} // namespace O_O



	// --------------------------------
	// ProgramBuilder::* implementations
#	pragma warning( push )
#	pragma warning( disable: 4351 ) // elements of vertex/fragmentShaderSource will be default-initialised
	ProgramBuilder::ProgramBuilder (void):
		vertexShaderSource(),
		fragmentShaderSource(),
		hasBeenBuilt(false),
		buildSuccessful(false),
		errorMessage(),
		infoLog(),
		infoLogHead(),
		program(glCreateProgram())
		{ PASSERT(program) }
#	pragma warning( pop ) // 4351

	ProgramBuilder::~ProgramBuilder (void) {
		PASSERT(HasBeenBuilt()) // no pointless program-builders
	}

	void ProgramBuilder::SetVertexAttributeBindings (VertexAttributeBindings const& bindings) {
		PASSERT(!HasBeenBuilt())

		VertexAttributeBindings::const_iterator			ite(bindings.begin());
		VertexAttributeBindings::const_iterator const	end(bindings.end());
		for (; ite != end; ++ite)
			__UNLESS_GL_ERROR
				glBindAttribLocation(program, ite->first, ite->second.c_str());
	}

	void ProgramBuilder::SetFragmentShaderSource (char const* const fss) {
		if (strlen(fss) >= sizeof(fragmentShaderSource)/sizeof(fragmentShaderSource[0]))
			; // ignore
		else
			strncpy_s(fragmentShaderSource, fss, sizeof(fragmentShaderSource)/sizeof(fragmentShaderSource[0]));
	}

	void ProgramBuilder::SetVertexShaderSource (char const* const vss) {
		if (strlen(vss) >= sizeof(vertexShaderSource)/sizeof(vertexShaderSource[0]))
			; // ignore
		else
			strncpy_s(vertexShaderSource, vss, sizeof(vertexShaderSource)/sizeof(vertexShaderSource[0]));
	}

	bool ProgramBuilder::Build (void) {
		PASSERT(HasVertexShaderSourceBeenSet())
		PASSERT(HasFragmentShaderSourcebeenSet())
		PASSERT(!HasBeenBuilt())
		PASSERT(!IsBuildSuccessful())

		hasBeenBuilt = true;
		
		GLuint vertexShader;
		GLuint fragmentShader;

		// Compile vertex shader
		{
			char const vertexShaderType[9] = "Vertex  ";
			buildSuccessful = O_O::CompileShader(
					vertexShader,
					vertexShaderSource,
					errorMessage,
					vertexShaderType,
					ShaderCompiler::ShaderType_VertexShader,
					infoLog,
					infoLogHead);
		}
		
		// Compile fragment shader
		if (buildSuccessful) {
			char const fragmentShaderType[9] = "Fragment";
			buildSuccessful = O_O::CompileShader(
					fragmentShader,
					fragmentShaderSource,
					errorMessage,
					fragmentShaderType,
					ShaderCompiler::ShaderType_FragmentShader,
					infoLog,
					infoLogHead);
		}

		// Attach to program
		if (buildSuccessful)
			buildSuccessful = O_O::CreateProgramAndLinkShaders(program, vertexShader, fragmentShader, errorMessage);

		return buildSuccessful;
	}

	bool ProgramBuilder::HasBeenBuilt (void) const {
		return hasBeenBuilt;
	}

	bool ProgramBuilder::IsBuildSuccessful (void) const { 
		return buildSuccessful;
	}

	bool ProgramBuilder::HasFragmentShaderSourcebeenSet (void) const {
		return fragmentShaderSource[0] != '\0';
	}

	bool ProgramBuilder::HasVertexShaderSourceBeenSet (void) const {
		return vertexShaderSource[0] != '\0';
	}

	void ProgramBuilder::Use (void) const {
		PASSERT(HasBeenBuilt() && IsBuildSuccessful())
		glUseProgram(program);
		__UNLESS_GL_ERROR {}
	}

	char const* ProgramBuilder::GetInfoLog (void) const {
		return infoLog;
	}

	char const* ProgramBuilder::GetErrorMessage (void) const {
		return errorMessage;
	}

	// --------------------------------
	// O_O implementations
	namespace O_O {
		void ConstructErrorMessageWithSuberrorSubmessage (
				char* const			dst,
				size_t const		dstLength,
				char const* const	myMessage,
				char const* const	causeMessage) {
			csconcat(dst, dstLength, myMessage, "[", causeMessage, "]");
		}

		bool CompileShader (
				GLuint&						shader,
				char const* const			source,
				ErrorMessage				errorMessage,
				char const					myShaderType[9],
				ShaderCompiler::ShaderType	shaderType,
				ProgramBuilder::SourceText	infoLog,
				size_t&						infoLogHead) {
			bool result(true); // invalidated if needed
			{
				ShaderCompiler shaderCompiler;
				shaderCompiler.Compile(source, shaderType);
				if (!shaderCompiler.IsCompilationSuccessful()) {
					result = false;
					char myErrorMessage[] = "         shader failed to compile. Reason: ";
					strncpy_s(&myErrorMessage[0], 9, myShaderType, 8);
					myErrorMessage[8] = ' ';
					ConstructErrorMessageWithSuberrorSubmessage(
							&errorMessage[0],
							sizeof(ErrorMessage)/sizeof(errorMessage[0]),
							&myErrorMessage[0],
							shaderCompiler.GetErrorMessage().c_str());
				}
				else {
					shader = shaderCompiler.GetShader();
					size_t available(sizeof(ProgramBuilder::SourceText)/sizeof(infoLog[0]) - infoLogHead);
					infoLogHead += codeshare::csconcat(infoLog, available, myShaderType, " log:\n", shaderCompiler.GetErrorMessage().c_str(), "\n");
					PASSERT(infoLogHead <= sizeof(ProgramBuilder::SourceText)/sizeof(infoLog[0]))
				}
			}
			return result;
		}

		void DefaultGlErrorHandler (LPCTSTR const errorMessage) {
			PASSERT(false)
		}

		bool CreateProgramAndLinkShaders (GLuint const program, GLuint const vertexShader, GLuint const fragmentShader, ErrorMessage errorMessage) {
			bool result(false);
			PASSERT(program != 0)

			__UNLESS_GL_ERROR {
				glAttachShader(program, vertexShader);
				__UNLESS_GL_ERROR {
					glAttachShader(program, fragmentShader);
					__UNLESS_GL_ERROR {
						glLinkProgram(program);
						__UNLESS_GL_ERROR {
							GLint linkingSucceeded(GL_FALSE);
							glGetProgramiv(program, GL_LINK_STATUS, &linkingSucceeded);
							__UNLESS_GL_ERROR {
								if (linkingSucceeded == GL_TRUE) {
									glDetachShader(program, vertexShader);
									__UNLESS_GL_ERROR {
										glDetachShader(program, fragmentShader);
										__UNLESS_GL_ERROR {
											glDeleteShader(vertexShader);
											__UNLESS_GL_ERROR {
												glDeleteShader(fragmentShader);
												__UNLESS_GL_ERROR {
													result = true;
												}
											}
										}
									}
								}
								else {
									// linking failure
									ConstructErrorMessageWithSuberrorSubmessage(
											&errorMessage[0],
											sizeof(ErrorMessage)/sizeof(errorMessage[0]),
											"Linking program failed, because: ",
											openglutil::GetProgramInfoLog(program));
								}
							}
						}
					}
				}
			}

			return result;
		}

	} // namespace O_O


}}} // namespace my::gl::shaders
