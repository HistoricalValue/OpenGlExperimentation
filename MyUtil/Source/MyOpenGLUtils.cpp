#include <stdafx.h>
#include <nmutil/OpenGlExtensionManager.h>

#ifndef P_INLINING
#	define __MY_OPEN_GL_UTILS__DEFINING_CONST_STRINGS
#endif
#include <MyOpenGLUtils_inl.h>

namespace my { namespace openglutil {

	using ::gl::ext::glGetProgramiv;
	using ::gl::ext::glGetProgramInfoLog;
	using ::gl::ext::glGetShaderiv;
	using ::gl::ext::glGetShaderInfoLog;

#define __GL_CONST_CASE(KONST) case MY_##KONST: return GlConstString<MY_##KONST>();


	TCHAR const* GlConstString (enum GlConst const& konst) {
		switch (konst) {
			__GL_CONST_CASE(GL_ALPHA_TEST)
			__GL_CONST_CASE(GL_AUTO_NORMAL)
			__GL_CONST_CASE(GL_BLEND)


			__GL_CONST_CASE(GL_RGB)
			__GL_CONST_CASE(GL_BGR)
			__GL_CONST_CASE(GL_BGRA)
			__GL_CONST_CASE(GL_LUMINANCE)


			default: PASSERT(false);
		}
		return NULL;
	}


#define __GL_ERROR_CASE(GLERR) case GLERR: return GlErrorString<GLERR>();


	TCHAR const* GlErrorString (enum GlError const& glerr) {
		switch (glerr) {
			__GL_ERROR_CASE(MY_GL_INVALID_ENUM)
			__GL_ERROR_CASE(MY_GL_INVALID_VALUE)
			__GL_ERROR_CASE(MY_GL_INVALID_OPERATION)
			__GL_ERROR_CASE(MY_GL_NO_ERROR)
			__GL_ERROR_CASE(MY_GL_STACK_OVERFLOW)
			__GL_ERROR_CASE(MY_GL_STACK_UNDERFLOW)
			__GL_ERROR_CASE(MY_GL_OUT_OF_MEMORY)
		}
		return NULL;
	}



	bool GlErrorsHandled (void (* const handler) (TCHAR const* errors)) {
		GLenum errors[1024];
		int i(0);

		for (; (errors[i] = ::gl::ext::glGetError()) != GL_NO_ERROR && i < sizeof(errors)/sizeof(errors[0]) && i < 16; ++i)
			;

		bool messageHandled(i > 0);
		if (messageHandled) {
			// Find out total message length
			unsigned int messageLength(0);
			{
				for (int j(0); j < i; ++j)
					messageLength += _tcslen(GlErrorString(static_cast<GlError>(errors[j])));
			}

			TCHAR const* const prefix(_T("Errors by OpenGL:\n"));
			unsigned int const prefixLength(_tcsclen(prefix));
			unsigned int available(
					prefixLength +
					messageLength +
					(i * (1 + 1))	// \t and \n
					+ 1);			// EOS
			// Get static buffer for string
			TCHAR* message(codeshare::utilities::GlobalSingleAllocationBuffer::Get().AsArrayOf<TCHAR>(available));

			// EOS reserved
			--available;
			int j(0);
			_tcsncpy_s(message, available, prefix, prefixLength + 1);
			j += prefixLength;

			for (int k(0); k < i; ++k) {
				TCHAR const* const errorString(GlErrorString(static_cast<GlError>(errors[k])));
				size_t const errorStringLength(_tcsclen(errorString));
				PASSERT(available - j > 1)
				message[j] = '\t';
				++j;
				_tcsncpy_s(message + j, available - j, errorString, errorStringLength + 1);
				j += errorStringLength;
				PASSERT(available - j >= 1)
				message[j] = '\n';
				++j;
			}

			(*handler)(message);
		}

		return messageHandled;
	}


	namespace _ {
		static size_t lastInfoLogLength(0);
		static P_INLINE GLchar* GetInfoLog (
				GLuint const object,
				void (* const paramGetter) (GLuint object, GLenum pname, GLint* params),
				GLenum const infoLogLengthParameterName,
				void (* const infoLogGetter) (GLuint object, GLsizei maxLength, GLsizei* length, GLchar* infoLog)) {
			GLsizei infoLogLength(0);
			(*paramGetter)(object, infoLogLengthParameterName, &infoLogLength);
			PASSERT(infoLogLength > 0)
			GLchar* const infoLog(codeshare::utilities::GlobalSingleAllocationBuffer::Get().AsArrayOf<GLchar>(psafecast<size_t>(infoLogLength + 1)));
			psafecast(lastInfoLogLength, infoLogLength);
			GLsizei actuallyWritten(0);
			(*infoLogGetter)(object, infoLogLength + 1, &actuallyWritten, &infoLog[0]);
			return infoLog;
		}

		static void ReleaseInfoLog (void) {
			codeshare::utilities::GlobalSingleAllocationBuffer::Get().ReleaseArrayOf<GLchar>(lastInfoLogLength + 1);
		}
	}

	GLchar* GetProgramInfoLog (GLuint const program) {
		return _::GetInfoLog(program, &glGetProgramiv, GL_INFO_LOG_LENGTH, &glGetProgramInfoLog);
	}

	GLchar* GetShaderInfoLog (GLuint const shader) {
		return _::GetInfoLog(shader, &glGetShaderiv, GL_INFO_LOG_LENGTH, &glGetShaderInfoLog);
	}

	void ReleaseShaderInfoLog (void) {
		_::ReleaseInfoLog();
	}

}} // namespace my::openglutil
