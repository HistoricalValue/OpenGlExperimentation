#ifndef __MY__GL__EXTENSIONS__EXTENSION_MANAGER__H__
#define __MY__GL__EXTENSIONS__EXTENSION_MANAGER__H__

#include "MyUtil.h"

// Guard against macros
#if defined( glCreateProgram )
#	error "A Gl extension is defined as a macro"
#endif

namespace my { namespace gl { namespace extensions {


	namespace ExtensionManager {
														// => "sucessfully?"
		MYUTIL_API bool									Initialise (void);
		MYUTIL_API void									CleanUp (void);

		// Wrappers
		MYUTIL_API GLuint								glCreateProgram (void);
		MYUTIL_API GLuint								glCreateShader (GLenum shaderType);
		MYUTIL_API void									glCompileShader (GLuint shader);
		MYUTIL_API void									glGetShaderiv (GLuint shader, GLenum propertyName, GLint* parameters);
		MYUTIL_API void									glGetShaderInfoLog (GLuint shader, GLsizei maxLength, GLsizei* actuallyWritten, GLchar* infolog);
		MYUTIL_API GLboolean							glIsShader (GLuint shader);
		MYUTIL_API void									glAttachShader (GLuint program, GLuint shader);
		MYUTIL_API void									glShaderSource (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
		MYUTIL_API void									glLinkProgram (GLuint program);
		MYUTIL_API void									glGetProgramiv (GLuint program, GLenum pname, GLint* params);
		MYUTIL_API void									glGetProgramInfoLog (GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
		MYUTIL_API void									glDetachShader (GLuint program, GLuint shader);
		MYUTIL_API void									glDeleteShader (GLuint shader);
		MYUTIL_API void									glGenVertexArrays (GLsizei n, GLuint* arrays);
		MYUTIL_API void									glBindVertexArray (GLuint array);
		MYUTIL_API void									glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
		MYUTIL_API void									glDeleteVertexArrays (GLsizei n, const GLuint* arrays);
		MYUTIL_API void									glGenBuffers (GLsizei n, GLuint* buffers);
		MYUTIL_API void									glDeleteBuffers (GLsizei n, const GLuint* buffers);
		MYUTIL_API void									glBindBuffer (GLenum target, GLuint buffer);
		MYUTIL_API void									glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
		MYUTIL_API void									glBindAttribLocation (GLuint program, GLuint index, const GLchar* name);
		MYUTIL_API void									glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
		MYUTIL_API void									glEnableVertexAttribArray (GLuint index);
		MYUTIL_API void									glDisableVertexAttribArray (GLuint index);
		MYUTIL_API void									glUseProgram (GLuint program);
		MYUTIL_API void									glVertexAttrib1f (GLuint index, GLfloat v0);
		MYUTIL_API void									glVertexAttrib4f (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		MYUTIL_API void									glDepthRangef (GLclampf nearVal, GLclampf farVal);
		MYUTIL_API void									glDepthFunc (GLenum func);
		MYUTIL_API const GLubyte*						glGetStringi (GLenum name, GLuint index);
		MYUTIL_API GLboolean							glIsBuffer (GLuint bufferId);
		MYUTIL_API GLboolean							glIsVertexArray (GLuint arrayId);
		MYUTIL_API GLboolean							glIsTexture (GLuint textureId);
		MYUTIL_API void									glActiveTexture (GLenum textureUnit);
	}


}}} // namespace my::gl::extensions

#endif // __MY__GL__EXTENSIONS__EXTENSION_MANAGER__H__

