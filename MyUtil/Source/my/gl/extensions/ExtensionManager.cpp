#include "stdafx.h"

#define __WITH_GL_DEPTH_RANGE
//#define __WITH_GL_IS_TEXTURE

// Convention:
//   GL function "glSomething" is of type "glSomethingFunc" (-"Func" prefix)
namespace _ {
	// util vars
	static bool											initialised(false);

	// typedefs
	typedef GLuint										(APIENTRY * glCreateProgramFunc) (void);
	typedef GLuint										(APIENTRY * glCreateShaderFunc) (GLenum);
	typedef void										(APIENTRY * glCompileShaderFunc) (GLuint);
	typedef void										(APIENTRY * glGetShaderivFunc) (GLuint, GLenum, GLint*);
	typedef void										(APIENTRY * glGetShaderInfoLogFunc) (GLuint shader, GLsizei maxLength, GLsizei* actuallyWritten, GLchar* infolog);
	typedef GLboolean									(APIENTRY * glIsShaderFunc) (GLuint shader);
	typedef void										(APIENTRY * glAttachShaderFunc) (GLuint program, GLuint shader);
	typedef void										(APIENTRY * glShaderSourceFunc) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
	typedef void										(APIENTRY * glLinkProgramFunc) (GLuint program);
	typedef void										(APIENTRY * glGetProgramivFunc) (GLuint program, GLenum pname, GLint* params);
	typedef void										(APIENTRY * glGetProgramInfoLogFunc) (GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
	typedef void										(APIENTRY * glDetachShaderFunc) (GLuint program, GLuint shader);
	typedef void										(APIENTRY * glDeleteShaderFunc) (GLuint shader);
	typedef void										(APIENTRY * glGenVertexArraysFunc) (GLsizei n, GLuint* arrays);
	typedef void										(APIENTRY * glBindVertexArrayFunc) (GLuint array);
	typedef void										(APIENTRY * glBufferDataFunc) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
	typedef void										(APIENTRY * glDeleteVertexArraysFunc) (GLsizei n, const GLuint* arrays);
	typedef void										(APIENTRY * glGenBuffersFunc) (GLsizei n, GLuint* buffers);
	typedef void										(APIENTRY * glDeleteBuffersFunc) (GLsizei n, const GLuint* buffers);
	typedef void										(APIENTRY * glBindBufferFunc) (GLenum target, GLuint buffer);
	typedef void										(APIENTRY * glBindAttribLocationFunc) (GLuint program, GLuint index, const GLchar* name);
	typedef void										(APIENTRY * glVertexAttribPointerFunc) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	typedef void										(APIENTRY * glEnableVertexAttribArrayFunc) (GLuint index);
	typedef void										(APIENTRY * glDisableVertexAttribArrayFunc) (GLuint index);
	typedef void										(APIENTRY * glUseProgramFunc) (GLuint program);
	typedef void										(APIENTRY * glVertexAttrib1fFunc) (GLuint index, GLfloat v0);
	typedef void										(APIENTRY * glVertexAttrib4fFunc) (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void										(APIENTRY * glDepthRangefFunc) (GLclampf nearVal, GLclampf farVal);
	typedef const GLubyte*								(APIENTRY * glGetStringiFunc) (GLenum name, GLuint index);
	typedef GLboolean									(APIENTRY * glIsBufferFunc) (GLuint bufferId);
	typedef GLboolean									(APIENTRY * glIsVertexArrayFunc) (GLuint arrayId);
	typedef GLboolean									(APIENTRY * glIsTextureFunc) (GLuint textureId);
	typedef void										(APIENTRY * glActiveTextureFunc) (GLenum textureUnit);


	// pointers
	static glCreateProgramFunc							glCreateProgram				(NULL);
	static glCreateShaderFunc							glCreateShader				(NULL);
	static glCompileShaderFunc							glCompileShader				(NULL);
	static glGetShaderivFunc							glGetShaderiv				(NULL);
	static glGetShaderInfoLogFunc						glGetShaderInfoLog			(NULL);
	static glIsShaderFunc								glIsShader					(NULL);
	static glAttachShaderFunc							glAttachShader				(NULL);
	static glShaderSourceFunc							glShaderSource				(NULL);
	static glLinkProgramFunc							glLinkProgram				(NULL);
	static glGetProgramivFunc							glGetProgramiv				(NULL);
	static glGetProgramInfoLogFunc						glGetProgramInfoLog			(NULL);
	static glDetachShaderFunc							glDetachShader				(NULL);
	static glDeleteShaderFunc							glDeleteShader				(NULL);
	static glGenVertexArraysFunc						glGenVertexArrays			(NULL);
	static glBindVertexArrayFunc						glBindVertexArray			(NULL);
	static glBufferDataFunc								glBufferData				(NULL);
	static glDeleteVertexArraysFunc						glDeleteVertexArrays		(NULL);
	static glGenBuffersFunc								glGenBuffers				(NULL);
	static glDeleteBuffersFunc							glDeleteBuffers				(NULL);
	static glBindBufferFunc								glBindBuffer				(NULL);
	static glBindAttribLocationFunc						glBindAttribLocation		(NULL);
	static glVertexAttribPointerFunc					glVertexAttribPointer		(NULL);
	static glEnableVertexAttribArrayFunc				glEnableVertexAttribArray	(NULL);
	static glDisableVertexAttribArrayFunc				glDisableVertexAttribArray	(NULL);
	static glUseProgramFunc								glUseProgram				(NULL);
	static glVertexAttrib1fFunc							glVertexAttrib1f			(NULL);
	static glVertexAttrib4fFunc							glVertexAttrib4f			(NULL);
	static glDepthRangefFunc							glDepthRangef				(NULL);
	static glGetStringiFunc								glGetStringi				(NULL);
	static glIsBufferFunc								glIsBuffer					(NULL);
	static glIsVertexArrayFunc							glIsVertexArray				(NULL);
	static glIsTextureFunc								glIsTexture					(NULL);
	static glActiveTextureFunc							glActiveTexture				(NULL);

	typedef int (*DoesNotExistFunc)(int);
	static DoesNotExistFunc DoesNotExist(NULL);

	template <typename T>
	bool SetPointerOrLogError (T*& ptr, T* const src, LPCTSTR const funcname) {
		bool result = src != NULL;
		TCHAR buf[1024];

		_sntprintf_s(buf, sizeof(buf)/sizeof(buf[0]), _T("Could not get GL function %s"), funcname);

		if (result)
			ptr = src;
		else
			my::global::logger::Get().Warning(my::String(buf));

		return result;
	}
} // namespace _


namespace my { namespace gl { namespace extensions {

	// --------------------------------
//#	define __GET_FUNC_AND_IS_SUCCESS(FUNC)		\
//		&& ( _:: FUNC = reinterpret_cast<_:: FUNC##Func>(wglGetProcAddress(#FUNC)) ) != NULL
#	define __GET_FUNC_AND_IS_SUCCESS(FUNC)		\
		&&  _::SetPointerOrLogError(_:: FUNC , reinterpret_cast<_:: FUNC##Func>(wglGetProcAddress(#FUNC)), _T(#FUNC))

	// --------------------------------
	namespace ExtensionManager {

		// ---------------------------
		bool Initialise (void) {
			PASSERT(!_::initialised)

			_::initialised = true
					__GET_FUNC_AND_IS_SUCCESS(glCreateProgram)
					__GET_FUNC_AND_IS_SUCCESS(glCreateShader)
					__GET_FUNC_AND_IS_SUCCESS(glCompileShader)
					__GET_FUNC_AND_IS_SUCCESS(glGetShaderiv)
					__GET_FUNC_AND_IS_SUCCESS(glGetShaderInfoLog)
					__GET_FUNC_AND_IS_SUCCESS(glIsShader)
					__GET_FUNC_AND_IS_SUCCESS(glAttachShader)
					__GET_FUNC_AND_IS_SUCCESS(glShaderSource)
					__GET_FUNC_AND_IS_SUCCESS(glLinkProgram)
					__GET_FUNC_AND_IS_SUCCESS(glGetProgramiv)
					__GET_FUNC_AND_IS_SUCCESS(glGetProgramInfoLog)
					__GET_FUNC_AND_IS_SUCCESS(glDetachShader)
					__GET_FUNC_AND_IS_SUCCESS(glDeleteShader)
					__GET_FUNC_AND_IS_SUCCESS(glGenVertexArrays)
					__GET_FUNC_AND_IS_SUCCESS(glBindVertexArray)
					__GET_FUNC_AND_IS_SUCCESS(glBufferData)
					__GET_FUNC_AND_IS_SUCCESS(glDeleteVertexArrays)
					__GET_FUNC_AND_IS_SUCCESS(glGenBuffers)
					__GET_FUNC_AND_IS_SUCCESS(glDeleteBuffers)
					__GET_FUNC_AND_IS_SUCCESS(glBindBuffer)
					__GET_FUNC_AND_IS_SUCCESS(glBindAttribLocation)
					__GET_FUNC_AND_IS_SUCCESS(glVertexAttribPointer)
					__GET_FUNC_AND_IS_SUCCESS(glEnableVertexAttribArray)
					__GET_FUNC_AND_IS_SUCCESS(glDisableVertexAttribArray)
					__GET_FUNC_AND_IS_SUCCESS(glUseProgram)
					__GET_FUNC_AND_IS_SUCCESS(glVertexAttrib1f)
					__GET_FUNC_AND_IS_SUCCESS(glVertexAttrib4f)
#ifdef __WITH_GL_DEPTH_RANGE
					__GET_FUNC_AND_IS_SUCCESS(glDepthRangef)
#endif
//					__GET_FUNC_AND_IS_SUCCESS(DoesNotExist)
					__GET_FUNC_AND_IS_SUCCESS(glGetStringi)
					__GET_FUNC_AND_IS_SUCCESS(glIsBuffer)
					__GET_FUNC_AND_IS_SUCCESS(glIsVertexArray)
#ifdef __WITH_GL_IS_TEXTURE
					__GET_FUNC_AND_IS_SUCCESS(glIsTexture)
#endif
					__GET_FUNC_AND_IS_SUCCESS(glActiveTexture)
					;

			if (!_::initialised)
				CleanUp();

			return _::initialised;
		}

		// ---------------------------
		void CleanUp (void) {
			_::glActiveTexture = NULL;
			_::glIsTexture = NULL;
			_::glIsVertexArray = NULL;
			_::glIsBuffer = NULL;
			_::glGetStringi = NULL;
			_::glDepthRangef = NULL;
			_::glVertexAttrib4f = NULL;
			_::glVertexAttrib1f = NULL;
			_::glUseProgram = NULL;
			_::glDisableVertexAttribArray = NULL;
			_::glEnableVertexAttribArray = NULL;
			_::glVertexAttribPointer =NULL;
			_::glBindAttribLocation = NULL;
			_::glBindBuffer = NULL;
			_::glDeleteBuffers = NULL;
			_::glGenBuffers = NULL;
			_::glDeleteVertexArrays = NULL;
			_::glBufferData = NULL;
			_::glBindVertexArray = NULL;
			_::glGenVertexArrays = NULL;
			_::glDeleteShader = NULL;
			_::glDetachShader = NULL;
			_::glGetProgramInfoLog = NULL;
			_::glGetProgramiv = NULL;
			_::glLinkProgram = NULL;
			_::glShaderSource = NULL;
			_::glAttachShader = NULL;
			_::glIsShader = NULL;
			_::glGetShaderInfoLog = NULL;
			_::glGetShaderiv = NULL;
			_::glCompileShader = NULL;
			_::glCreateShader = NULL;
			_::glCreateProgram = NULL;
			_::initialised = false;
		}

		// ---------------------------
		bool IsInitialised (void) {
			return _::initialised;
		}


		// ---------------------------
		// Wrappers
		GLuint glCreateProgram (void) {
			PASSERT(IsInitialised())
			return (*_::glCreateProgram)();
		}

		GLuint glCreateShader (GLenum const type) {
			PASSERT(IsInitialised())
			return (*_::glCreateShader)(type);
		}

		void glCompileShader (GLuint const shader) {
			PASSERT(IsInitialised())
			(*_::glCompileShader)(shader);
		}

		void glGetShaderiv (GLuint const shader, GLenum const propertyName, GLint* const parameters) {
			PASSERT(IsInitialised())
			(*_::glGetShaderiv)(shader, propertyName, parameters);
		}

		void glGetShaderInfoLog (GLuint const shader, GLsizei const maxLength, GLsizei* const actuallyWritten, GLchar* const infolog) {
			PASSERT(IsInitialised())
			(*_::glGetShaderInfoLog)(shader, maxLength, actuallyWritten, infolog);
		}

		GLboolean glIsShader (GLuint const shader) {
			PASSERT(IsInitialised())
			return (*_::glIsShader)(shader);
		}

		void glAttachShader (GLuint const program, GLuint const shader) {
			PASSERT(IsInitialised())
			(*_::glAttachShader)(program, shader);
		}

		void glShaderSource (GLuint const shader, GLsizei const count, const GLchar** const strings, const GLint* const lengths) {
			PASSERT(IsInitialised())
			(*_::glShaderSource)(shader, count, strings, lengths);
		}

		void glLinkProgram (GLuint const program) {
			PASSERT(IsInitialised())
			(*_::glLinkProgram)(program);
		}

		void glGetProgramiv (GLuint const program, GLenum const pname, GLint* const params) {
			PASSERT(IsInitialised())
			(*_::glGetProgramiv)(program, pname, params);
		}

		void glGetProgramInfoLog (GLuint const program, GLsizei const maxLength, GLsizei* const length, GLchar* const infoLog) {
			PASSERT(IsInitialised())
			(*_::glGetProgramInfoLog)(program, maxLength, length, infoLog);
		}

		void glDetachShader (GLuint const program, GLuint const shader) {
			PASSERT(IsInitialised())
			(*_::glDetachShader)(program, shader);
		}

		void glDeleteShader (GLuint const shader) {
			PASSERT(IsInitialised())
			(*_::glDeleteShader)(shader);
		}

		void glGenVertexArrays (GLsizei const n, GLuint* const arrays) {
			PASSERT(IsInitialised())
			(*_::glGenVertexArrays)(n, arrays);
		}

		void glBindVertexArray (GLuint const array) {
			PASSERT(IsInitialised())
			(*_::glBindVertexArray)(array);
		}

		void glBufferData (GLenum const target, GLsizeiptr const size, GLvoid const* const data, GLenum const usage) {
			PASSERT(IsInitialised())
			(*_::glBufferData)(target, size, data, usage);
		}

		void glDeleteVertexArrays (GLsizei const n, GLuint const* arrays) {
			PASSERT(IsInitialised())
			(*_::glDeleteVertexArrays)(n, arrays);
		}

		void glGenBuffers (GLsizei const n, GLuint* const buffers) {
			PASSERT(IsInitialised())
			(*_::glGenBuffers)(n, buffers);
		}

		void glDeleteBuffers (GLsizei const n, GLuint const* const buffers) {
			PASSERT(IsInitialised())
			(*_::glDeleteBuffers)(n, buffers);
		}

		void glBindBuffer (GLenum const target, GLuint const buffer) {
			PASSERT(IsInitialised())
			(*_::glBindBuffer)(target, buffer);
		}

		void glBindAttribLocation (GLuint const program, GLuint const index, GLchar const* const name) {
			PASSERT(IsInitialised())
			(*_::glBindAttribLocation)(program, index, name);
		}

		void glVertexAttribPointer (GLuint const index, GLint const size, GLenum const type, GLboolean const normalized, GLsizei const stride, GLvoid const* const pointer) {
			PASSERT(IsInitialised())
			(*_::glVertexAttribPointer)(index, size, type, normalized, stride, pointer);
		}

		void glEnableVertexAttribArray (GLuint const index) {
			PASSERT(IsInitialised())
			(*_::glEnableVertexAttribArray)(index);
		}

		void glDisableVertexAttribArray (GLuint const index) {
			PASSERT(IsInitialised())
			(*_::glDisableVertexAttribArray)(index);
		}

		void glUseProgram (GLuint const program) {
			PASSERT(IsInitialised())
			(*_::glUseProgram)(program);
		}

		void glVertexAttrib1f (GLuint const index, GLfloat const v0) {
			PASSERT(IsInitialised())
			(*_::glVertexAttrib1f)(index, v0);
		}

		void glVertexAttrib4f (GLuint const index, GLfloat const v0, GLfloat const v1, GLfloat const v2, GLfloat const v3) {
			PASSERT(IsInitialised())
			(*_::glVertexAttrib4f)(index, v0, v1, v2, v3);
		}

#ifdef __WITH_GL_DEPTH_RANGE
		void glDepthRangef (GLclampf const nearVal, GLclampf const farVal) {
			PASSERT(IsInitialised())
			(*_::glDepthRangef)(nearVal, farVal);
		}
#endif

		const GLubyte* glGetStringi (GLenum const name, GLuint const index) {
			PASSERT(IsInitialised())
			return (*_::glGetStringi)(name, index);
		}

		GLboolean glIsBuffer (GLuint const bufferId) {
			PASSERT(IsInitialised())
			return (*_::glIsBuffer)(bufferId);
		}

		GLboolean glIsVertexArray (GLuint const arrayId) {
			PASSERT(IsInitialised())
			return (*_::glIsVertexArray)(arrayId);
		}

#ifdef __WITH_GL_IS_TEXTURE
		GLboolean glIsTexture (GLuint const textureId) {
			PASSERT(IsInitialised())
			return (*_::glIsTexture)(textureId);
		}
#endif

		void glActiveTexture (GLenum const textureUnit) {
			PASSERT(IsInitialised())
			(*_::glActiveTexture)(textureUnit);
		}

	} // namespace ExtensionManager

}}} // namespace my::gl::extensions


