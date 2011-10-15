#ifndef __MY_OPEN_GL__H__
#define __MY_OPEN_GL__H__

#include <TheCursed.h>

#define VAN_POSITION	"position"
#define VAN_COLOUR		"colour"
#define VAN_AXYC		"axyc"
#define VAN_TEXCOORD	"texcoord"
#define VAN_NORMAL		"normal"
#define VAN_AOFACTOR	"ambientOcclusionFactor"

namespace my {

	class OpenGL {
	public:
		typedef HGLRC									ResourceContextHandle;
		typedef HDC										DeviceContextHandle;

		explicit										OpenGL (void);
		explicit										OpenGL (Window const&);
														~OpenGL (void);

		bool											Initialise (void);
		bool											IsInitialised (void) const;
		void											CleanUp (void); // called at destruction time, if needed

		void											PutChar (TCHAR);
		void											PutString (TCHAR const*);

		void											Reset (void);

		bool											IsValid (void) const;

		void											SwapBuffers (void);

		static struct _VAI_POSITION { operator GLuint (void) const; } const	VAI_POSITION;
		static struct _VAI_COLOUR	{ operator GLuint (void) const; } const	VAI_COLOUR;
		static struct _VAI_AXYC		{ operator GLuint (void) const; } const	VAI_AXYC;
		static struct _VAI_TEXCOORD	{ operator GLuint (void) const; } const	VAI_TEXCOORD;
		static struct _VAI_NORMAL	{ operator GLuint (void) const; } const	VAI_NORMAL;
		static struct _VAI_AOFACTOR	{ operator GLuint (void) const; } const VAI_AOFACTOR;

		static struct _VUL_SAMPLER0	{ operator GLuint (void) const; } const VUL_SAMPLER0;
		static struct _VUL_SAMPLER1	{ operator GLuint (void) const; } const VUL_SAMPLER1;
		static struct _VUL_SAMPLER2	{ operator GLuint (void) const; } const VUL_SAMPLER2;
		static struct _VUL_SAMPLER3	{ operator GLuint (void) const; } const VUL_SAMPLER3;

		static struct _VUL_COLSELTR	{ operator GLuint (void) const; } const VUL_COLSELTR;

		static struct _VUL_TEXTUREZ { operator GLuint (void) const; } const VUL_TEXTUREZ;

		static struct _VUL_CAMERA		{ operator GLuint (void) const; } const	VUL_CAMERA;
		static struct _VUL_PROJECTION	{ operator GLuint (void) const; } const	VUL_PROJECTION;

		static struct _VUL_POOPLIGHT	{ operator GLuint (void) const; } const VUL_POOPLIGHT;

	private:
		DeviceContextHandle								device;
		ResourceContextHandle							context;

		int												x;
		int												y;

		bool											initialised;
	};

} // namespace my

#endif // __MY_OPEN_GL__H__
