#ifndef __MY_OPEN_GL__H__
#define __MY_OPEN_GL__H__

#include "MyWindow.h"
#include "MyGL.h"

#define VAN_POSITION	"position"
#define VAN_COLOUR		"colour"
#define VAN_AXYC		"axyc"
#define VAN_TEXCOORD	"texcoord"

namespace my {

	class OpenGL {
	public:
		typedef HGLRC									ResourceContextHandle;
		typedef HDC										DeviceContextHandle;

		explicit										OpenGL (Window const&);
														~OpenGL (void);

		bool											Initialise (void);
		bool											IsInitialised (void) const;
		void											CleanUp (void); // called at destruction time, if needed

		void											PutChar (TCHAR);
		void											PutString (LPCTSTR);

		void											Reset (void);

		bool											IsValid (void) const;

		void											SwapBuffers (void);

		static struct _VAI_POSITION { operator GLuint (void) const; } const	VAI_POSITION;
		static struct _VAI_COLOUR	{ operator GLuint (void) const; } const	VAI_COLOUR;
		static struct _VAI_AXYC		{ operator GLuint (void) const; } const	VAI_AXYC;
		static struct _VAI_TEXCOORD	{ operator GLuint (void) const; } const	VAI_TEXCOORD;
		
		static struct _VUL_SAMPLER0	{ operator GLuint (void) const; } const VUL_SAMPLER0;
		static struct _VUL_SAMPLER1	{ operator GLuint (void) const; } const VUL_SAMPLER1;
		static struct _VUL_SAMPLER2	{ operator GLuint (void) const; } const VUL_SAMPLER2;
		static struct _VUL_SAMPLER3	{ operator GLuint (void) const; } const VUL_SAMPLER3;
		static struct _VUL_SAMPLER4	{ operator GLuint (void) const; } const VUL_SAMPLER4;

	private:
		DeviceContextHandle								device;
		ResourceContextHandle							context;

		int												x;
		int												y;

		bool											initialised;
	};

} // namespace my

#endif // __MY_OPEN_GL__H__
