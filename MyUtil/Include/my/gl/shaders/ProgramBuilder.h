#ifndef __MY__GL__SHADERS__PROGRAM_BUILDER__H__
#define __MY__GL__SHADERS__PROGRAM_BUILDER__H__

#include <MyUtil.h>
#include <MyGL.h>

namespace my { namespace gl { namespace shaders {

	class MYUTIL_API ProgramBuilder {
	public:
		static const size_t								MAX_SOURCE_TEXT_LENGTH = (1<<16);
		typedef char									SourceText[MAX_SOURCE_TEXT_LENGTH + 1];

		void											SetVertexShaderSource (char const* source);		// copied
		void											SetFragmentShaderSource (char const* source);	// copied
		bool											HasVertexShaderSourceBeenSet (void) const;
		bool											HasFragmentShaderSourcebeenSet (void) const;

		typedef	std::pair<GLuint,std::string>			VertexAttributeBinding;
		typedef std::list<VertexAttributeBinding>		VertexAttributeBindings;
		void											SetVertexAttributeBindings (VertexAttributeBindings const&);

		bool											HasBeenBuilt (void) const;
		bool											Build (void);
		bool											IsBuildSuccessful (void) const;
		char const*										GetErrorMessage (void) const;
		char const*										GetInfoLog (void) const;

		void											Use (void) const;

		GLuint											GetUniformLocation (char const*) const;

														ProgramBuilder (void);
														~ProgramBuilder (void);

	private:
		SourceText										vertexShaderSource;
		SourceText										fragmentShaderSource;
		bool											hasBeenBuilt;
		bool											buildSuccessful;
		SourceText										errorMessage;
		SourceText										infoLog;
		size_t											infoLogHead;
		GLuint											program;

		// No copying or assigning
														ProgramBuilder (ProgramBuilder const&);
		void											operator = (ProgramBuilder const&);
	};

}}} // namespace my::gl::shaders

#endif // __MY__GL__SHADERS__PROGRAM_BUILDER__H__
