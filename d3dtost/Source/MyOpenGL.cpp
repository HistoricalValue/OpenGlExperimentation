#include "stdafx.h"

#include "PCString_inl.h"

#define WGL_CONTEXT_PROFILE_MASK_ARB   0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001


#define __NE()	PASSERT(!my::openglutil::GlErrorsHandled(&_::GlErrorHandler))

namespace my {

	namespace _ {
		///////////////////////////////////
		static _T_STR GlErrorMessage;
		static inline void GlErrorHandler (LPCTSTR const msg) {
			GlErrorMessage = msg;
		}

		///////////////////////////////////
		static void DrawChar (TCHAR c, GLfloat x, GLfloat y);
		namespace gl {
			static bool CreateContext (my::OpenGL::DeviceContextHandle& device, my::OpenGL::ResourceContextHandle& context);
		}
		static GLuint sampler0location(-1);
		static GLuint sampler1location(-1);
		static GLuint sampler2location(-1);
		static GLuint sampler3location(-1);
		static GLuint sampler4location(-1);
		static GLuint textureZLocation(-1);

		static bool InstallShaders (
				my::gl::shaders::ProgramBuilder&	programBuilder,
				my::gl::shaders::ShaderManager&		shaderManager) {

			shaderManager.LoadShadersSources();
// 			PASSERT(shaderManager.AreShaderSourcesLoaded())

			bool result(shaderManager.AreShaderSourcesLoaded());
			if (result) {
				programBuilder.SetVertexShaderSource(shaderManager.GetVertexShaderSource());
				programBuilder.SetFragmentShaderSource(shaderManager.GetFragmentShaderSource());

				my::gl::shaders::ProgramBuilder::VertexAttributeBindings bindings;
				bindings.push_back(std::make_pair(OpenGL::VAI_POSITION, VAN_POSITION));
				bindings.push_back(std::make_pair(OpenGL::VAI_COLOUR, VAN_COLOUR));
				bindings.push_back(std::make_pair(OpenGL::VAI_AXYC, VAN_AXYC));
				bindings.push_back(std::make_pair(OpenGL::VAI_TEXCOORD, VAN_TEXCOORD));

				programBuilder.SetVertexAttributeBindings(bindings);
			}
			else {
				my::global::logger::Get().Error(_T("Could not load shaders: "));
				my::global::logger::Get().Error(d3dtost::ConvertErrorMessage(shaderManager.GetLoadingErrorMessage()));
			}

			return result;
		}

		static void InfologAllExtensions (void) {
			using my::gl::extensions::ExtensionManager::glGetStringi;

			char buf[1<<17]; // 128KiB
			size_t off(0);

			{ // first log number of texture units
				GLint iCombinedUnits(-1);
				GLint iUnits(-1);
				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &iCombinedUnits); __NE()

				int result(_snprintf_s(&buf[off], sizeof(buf) - off * sizeof(buf[0]), 70,
						"number of texture units: %d\nnumber of combined texture unuts: %d\n", iUnits, iCombinedUnits));
				PASSERT(result >= 64)
				off += result;
			}

			GLint nNumExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &nNumExtensions); __NE()

			for(GLint i = 0; i < nNumExtensions && off < sizeof(buf)/sizeof(buf[0]); i++)
				buf[(off += codeshare::utilities::csconcat(
							&buf[off],
							sizeof(buf)/sizeof(buf[0]) - off,
							reinterpret_cast<char const* const>(glGetStringi(GL_EXTENSIONS, i))
						) + 1) - 1] = '\n';

			__NE()
			my::global::logger::Get().Info(d3dtost::ConvertErrorMessage(buf));
		}
	}


	OpenGL::OpenGL (void):
		device(NULL),
		context(NULL),
		x(0),
		y(0),
		initialised(false)
		{ }


	OpenGL::OpenGL (Window const& window):
		device(GetDC(window.window)),
		context(),
		x(0),
		y(0),
		initialised(false)
		{ PASSERT(device != NULL) }


	bool OpenGL::Initialise (void) {
		PASSERT(!initialised)
		initialised = device && _::gl::CreateContext(device, context) || true;

		if (initialised) {
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  __NE()

			initialised = my::gl::extensions::ExtensionManager::Initialise()
					&& ::gl::ext::Initialise();

			if (initialised) {
				my::gl::shaders::ShaderManager	shaderManager;
				my::gl::shaders::ProgramBuilder	programBuilder;

				initialised = _::InstallShaders(programBuilder, shaderManager);

				if (initialised) {
					initialised = programBuilder.Build();


					if (initialised) {
						my::global::log::info(d3dtost::ConvertErrorMessage(programBuilder.GetInfoLog()));

						programBuilder.Use();
						_::sampler0location = programBuilder.GetUniformLocation("textureUnit0");
						_::sampler1location = programBuilder.GetUniformLocation("textureUnit1");
						_::sampler2location = programBuilder.GetUniformLocation("textureUnit2");
						_::sampler3location = programBuilder.GetUniformLocation("textureUnit3");
						_::sampler4location = programBuilder.GetUniformLocation("textureUnit4");
						_::textureZLocation = programBuilder.GetUniformLocation("textureZ");

						_::InfologAllExtensions();
					}
					else {
						if (context)
							wglDeleteContext(context);
						my::gl::extensions::ExtensionManager::CleanUp();
						my::global::logger::Get().Error(d3dtost::ConvertErrorMessage(programBuilder.GetErrorMessage()));
					}
				}
				else {
					if (context)
						wglDeleteContext(context);
					my::gl::extensions::ExtensionManager::CleanUp();
					// error reported in _::InstallShaders()
				}
			}
			else {
				if (context)
					wglDeleteContext(context);
			}
		}

		//wglDeleteContext(wglGetCurrentContext());
//		PASSERT(initialised)
		return initialised;
	}


	void OpenGL::CleanUp (void) {
		PASSERT(IsInitialised())
		PASSERT( wglGetCurrentContext() == context )
		// destroy shaders
		my::gl::extensions::ExtensionManager::CleanUp();
		::gl::ext::CleanUp();
		if (context)
			wglDeleteContext(context);
		initialised = false;
	}

	bool OpenGL::IsInitialised (void) const {
		return initialised;
	}

	OpenGL::~OpenGL (void) {
		if (IsInitialised())
			CleanUp();
		PASSERT(!initialised)
	}

	bool OpenGL::IsValid (void) const {
		return true;
	}

	void OpenGL::SwapBuffers (void) {
		::SwapBuffers(device);
	}


	void OpenGL::PutChar (TCHAR c) {
		// top-left box corner
		const GLfloat px = -390.f + x * 10.f;
		const GLfloat py = 200.f - y * 10.f;
		++x;
		if (x == 80) {
			x = 0;
			++y;
		}

		_::DrawChar(c, px, py);
	}

	void OpenGL::PutString (LPCTSTR const str) {
		LPCTSTR charptr(&str[0]);
		while ((*charptr) != '\0')
			PutChar(*charptr++);
	}

	void OpenGL::Reset (void) {
		x = y = 0;
	}


	OpenGL::_VAI_POSITION const OpenGL::VAI_POSITION;
	OpenGL::_VAI_COLOUR const OpenGL::VAI_COLOUR;
	OpenGL::_VAI_AXYC const OpenGL::VAI_AXYC;
	OpenGL::_VAI_TEXCOORD const OpenGL::VAI_TEXCOORD;

	OpenGL::_VAI_POSITION::operator GLuint (void) const {
		return 0u;
	}

	OpenGL::_VAI_COLOUR::operator GLuint (void) const {
		return 1u;
	}

	OpenGL::_VAI_AXYC::operator GLuint (void) const {
		return 2u;
	}

	OpenGL::_VAI_TEXCOORD::operator GLuint (void) const {
		return 3u;
	}


	OpenGL::_VUL_SAMPLER0 const OpenGL::VUL_SAMPLER0;
	OpenGL::_VUL_SAMPLER0::operator GLuint (void) const {
		PASSERT(_::sampler0location != -1)
		return _::sampler0location;
	}


	OpenGL::_VUL_SAMPLER1 const OpenGL::VUL_SAMPLER1;
	OpenGL::_VUL_SAMPLER1::operator GLuint (void) const {
		PASSERT(_::sampler1location != -1)
		return _::sampler1location;
	}


	OpenGL::_VUL_SAMPLER2 const OpenGL::VUL_SAMPLER2;
	OpenGL::_VUL_SAMPLER2::operator GLuint (void) const {
		PASSERT(_::sampler2location != -1)
		return _::sampler2location;
	}


	OpenGL::_VUL_SAMPLER3 const OpenGL::VUL_SAMPLER3;
	OpenGL::_VUL_SAMPLER3::operator GLuint (void) const {
		PASSERT(_::sampler3location != -1)
		return _::sampler3location;
	}

	OpenGL::_VUL_SAMPLER4 const OpenGL::VUL_SAMPLER4;
	OpenGL::_VUL_SAMPLER4::operator GLuint (void) const {
		PASSERT(_::sampler4location != -1)
		return _::sampler4location;
	}

	OpenGL::_VUL_TEXTUREZ const OpenGL::VUL_TEXTUREZ;
	OpenGL::_VUL_TEXTUREZ::operator GLuint (void) const {
		PASSERT(_::textureZLocation != -1)
		return _::textureZLocation;
	}






	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// Irrelevant -- drawer stuff

#define DRAWER(CHAR) \
	static void Draw_##CHAR (GLfloat const x, GLfloat const y)


	// implementations
	namespace _ {

		namespace gl {

			static bool CreateContext (my::OpenGL::DeviceContextHandle& device, my::OpenGL::ResourceContextHandle& context) {
				PIXELFORMATDESCRIPTOR pfd = {
					sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
					1,                     // version number
					PFD_DRAW_TO_WINDOW |   // support window
						PFD_SUPPORT_OPENGL |   // support OpenGL
						PFD_DOUBLEBUFFER,      // double buffered
					PFD_TYPE_RGBA,         // RGBA type
					24,                    // 24-bit color depth
					0, 0, 0, 0, 0, 0,      // color bits ignored
					0,                     // no alpha buffer
					0,                     // shift bit ignored
					0,                     // no accumulation buffer
					0, 0, 0, 0,            // accum bits ignored
					32,                    // 32-bit z-buffer					// !!! GLUT_DEPTH  (used for depth testing)
					0,                     // no stencil buffer
					0,                     // no auxiliary buffer
					PFD_MAIN_PLANE,        // main layer
					0,                     // reserved
					0, 0, 0                // layer masks ignored
				};
				int  iPixelFormat;
				BOOL bresult;

				// get the best available match of pixel format for the device context
				iPixelFormat = ChoosePixelFormat(device, &pfd);


				// make that the pixel format of the device context
				bresult = SetPixelFormat(device, iPixelFormat, &pfd);
				{ LPTSTR msg = my::winutil::ErrorToString(GetLastError());
					my::winutil::ReleaseErrorString(msg); }

				if (bresult == FALSE)
					return false;

				// Try to create a 4.1 context
				typedef HGLRC (*wglCreateContextAttribsARB_func)(HDC hDC, HGLRC hShareContext, const int *attribLis);
				wglCreateContextAttribsARB_func wglCreateContextAttribsARB(reinterpret_cast<wglCreateContextAttribsARB_func>(
							wglGetProcAddress("wglCreateContextAttribsARB")));

				context = wglCreateContext(device);
				if (wglCreateContextAttribsARB) {
					int attribs[] = {WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0};
					context = wglCreateContextAttribsARB(device, context, &attribs[0]);
				}
				{ LPTSTR msg = my::winutil::ErrorToString(GetLastError());
					my::winutil::ReleaseErrorString(msg); }

				if (!wglCreateContextAttribsARB && false)
					return false;

				bresult = wglMakeCurrent(device, context);
				{ LPTSTR msg = my::winutil::ErrorToString(GetLastError());
					my::winutil::ReleaseErrorString(msg); }
				if (bresult == FALSE)
					return false;

				return true;
			}

		} // namespace _::gl

		class __glLinesBlock {
		public:
			__glLinesBlock (void);
			~__glLinesBlock (void);
		};

#define LINES_BLOCK		__glLinesBlock __linesBlock;
#define VERT(X,Y)		glVertex2f(x + float(X), y - float(Y));
#define LINE(X,Y, W,V)	VERT(X,Y) VERT(W,V)

		DRAWER(a) {
			LINES_BLOCK

			glVertex2f(x + 1.f, y - 10.f);
			glVertex2f(x + 1.f, y - 3.f);

			glVertex2f(x + 1.f, y - 3.f);
			glVertex2f(x + 5.f, y - 1.f);

			glVertex2f(x + 5.f, y - 1.f);
			glVertex2f(x + 9.f, y - 3.f);

			glVertex2f(x + 9.f, y - 3.f);
			glVertex2f(x + 9.f, y - 10.f);

			glVertex2f(x + 1.f, y - 5.f);
			glVertex2f(x + 9.f, y - 5.f);
		}

		DRAWER(b) {
			LINES_BLOCK

			glVertex2f(x + 1.f, y - 1.f);
			glVertex2f(x + 1.f, y - 10.f);

			glVertex2f(x + 1.f, y - 10.f);
			glVertex2f(x + 9.f, y - 10.f);

			glVertex2f(x + 9.f, y - 10.f);
			glVertex2f(x + 9.f, y - 5.f);

			glVertex2f(x + 9.f, y - 5.f);
			glVertex2f(x + 1.f, y - 5.f);

		}

		DRAWER(e) {
			LINES_BLOCK
			glVertex2f(x + 1.f, y - 2.f);
			glVertex2f(x + 9.f, y - 2.f);

			glVertex2f(x + 1.f, y - 2.f);
			glVertex2f(x + 1.f, y - 9.f);

			glVertex2f(x + 1.f, y - 9.f);
			glVertex2f(x + 9.f, y - 9.f);

			glVertex2f(x + 5.f, y - 5.f);
			glVertex2f(x + 9.f, y - 5.f);
		}

		DRAWER(g) {
			LINES_BLOCK

			LINE(9, 1, 1, 1)
			LINE(1, 1, 1, 9)
			LINE(1, 9, 9, 9)
			LINE(9, 9, 9, 5)
			LINE(9, 5, 3, 5)
		}

		DRAWER(i) {
			LINES_BLOCK

			glVertex2f(x + 5.f, y - 2.f);
			glVertex2f(x + 5.f, y - 3.f);

			glVertex2f(x + 5.f, y - 5.f);
			glVertex2f(x + 5.f, y - 10.f);

		}

		DRAWER(k) {
			LINES_BLOCK

			glVertex2f(x + 1.f, y - 1.f);
			glVertex2f(x + 1.f, y - 9.f);

			glVertex2f(x + 1.f, y - 5.f);
			glVertex2f(x + 9.f, y - 1.f);

			glVertex2f(x + 1.f, y - 5.f);
			glVertex2f(x + 9.f, y - 10.f);
		}

		DRAWER(l) {
			LINES_BLOCK

			LINE(1,1, 1,9)
			LINE(1,9, 9,9)
		}

		DRAWER(m) {
			LINES_BLOCK

			glVertex2f(x + 1.f, y - 9.f);
			glVertex2f(x + 1.f, y - 2.f);

			glVertex2f(x + 1.f, y - 2.f);
			glVertex2f(x + 9.f, y - 2.f);

			glVertex2f(x + 9.f, y - 2.f);
			glVertex2f(x + 9.f, y - 9.f);

			glVertex2f(x + 5.f, y - 2.f);
			glVertex2f(x + 5.f, y - 9.f);
		}

		DRAWER(o) {
			LINES_BLOCK

			VERT(1, 1)
			VERT(1, 9)

			VERT(1, 1)
			VERT(9, 1)

			VERT(9, 1)
			VERT(9, 9)

			VERT(1, 9)
			VERT(9, 9)
		}

		DRAWER(p) {
			LINES_BLOCK

			LINE(1,1, 1,9)
			LINE(1,1, 9,1)
			LINE(9,1, 9,5)
			LINE(9,5, 1,5)
		}

		DRAWER(r) {
			LINES_BLOCK

			LINE(1, 1, 9, 1)
			LINE(9, 1, 9, 5)
			LINE(9, 5, 1, 5)
			LINE(1, 5, 9, 9)
			LINE(1, 1, 1, 9)
		}

		DRAWER(s) {
			LINES_BLOCK

			LINE(1, 1, 9, 1)
			LINE(1, 1, 1, 5)
			LINE(1, 5, 9, 5)
			LINE(9, 5, 9, 9)
			LINE(9, 9, 1, 9)
		}

		DRAWER(u) {
			LINES_BLOCK

			LINE(1,1, 1,9)
			LINE(1,9, 9,9)
			LINE(9,1, 9,9)
		}

		DRAWER(v) {
			LINES_BLOCK

			LINE(1,1, 5,9)
			LINE(9,1, 5,9)
		}

		DRAWER(y) {
			LINES_BLOCK

			LINE(1, 1, 5, 5)
			LINE(9, 1, 5, 5)
			LINE(5, 5, 5, 9)
		}


		DRAWER(3) {
			LINES_BLOCK

			LINE(1,1, 9,1)
			LINE(9,1, 9,9)
			LINE(1,9, 9,9)
			LINE(1,5, 9,5)
		}


		DRAWER(lt) {
			LINES_BLOCK

			LINE(5,5, 9,1)
			LINE(5,5, 9,9)
		}

		DRAWER(_) {
			LINES_BLOCK

			glVertex2f(x + 1.f, y - 9.f);
			glVertex2f(x + 9.f, y - 9.f);
		}

		DRAWER(space) {
		}

#define DRAWER_CASE(LIT, CHAR) \
		case LIT: { drawer = &Draw_##CHAR; break; }

		void DrawChar (const TCHAR c, const GLfloat x, const GLfloat y) {
			void (*drawer) (GLfloat x, GLfloat y) (NULL);
			switch (c) {
				DRAWER_CASE('a', a)
				DRAWER_CASE('b', b)
				DRAWER_CASE('e', e)
				DRAWER_CASE('g', g)
				DRAWER_CASE('i', i)
				DRAWER_CASE('k', k)
				DRAWER_CASE('l', l)
				DRAWER_CASE('m', m)
				DRAWER_CASE('o', o)
				DRAWER_CASE('p', p)
				DRAWER_CASE('r', r)
				DRAWER_CASE('s', s)
				DRAWER_CASE('u', u)
				DRAWER_CASE('v', v)
				DRAWER_CASE('y', y)

				DRAWER_CASE('3', 3)

				DRAWER_CASE('<', lt)
				DRAWER_CASE(' ', space)
				DRAWER_CASE('_', _)
				default: {
					drawer = &Draw__;
					break;
				}
			}

			(*drawer)(x, y);
		}


		__glLinesBlock::__glLinesBlock (void) {
			glBegin(GL_LINES);
		}

		__glLinesBlock::~__glLinesBlock (void) {
			glEnd();
		}

	} // namespace _

} // namespace my
