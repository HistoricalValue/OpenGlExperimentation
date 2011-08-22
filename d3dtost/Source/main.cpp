#include "stdafx.h"

// notes: http://www.youtube.com/watch?v=6RbT0zWTb8g&feature=player_embedded

namespace my {
	namespace drawing {
		extern void draw (void* drawData, void (*bufferSwapper) (void*), void* bufferSwapperClosure);
		extern void* setup (void);
		extern void cleanup (void*& drawData);
	} // namespace drawing
} // namespace my

namespace _ {
	struct DrawData {
	public:
		void*&		Get (void) { return drawData; }
		void		Set (void* const d) { drawData = d; }
					DrawData (void): drawData(NULL) {}
	private:
		void*		drawData;
	};
	struct MainLoopCallbackClosure {
		my::OpenGL* opengl;
		DrawData*	drawData;
	};

	namespace Wrappers {
		void bufferSwapper (void* data) {
			my::OpenGL& opengl(*static_cast<my::OpenGL*>(data));
			opengl.SwapBuffers();
		}
	}

	namespace Callbacks {

		static void MainLoop (void* _closure) {
			MainLoopCallbackClosure const& closure(*static_cast<MainLoopCallbackClosure const* const>(_closure));
			my::drawing::draw(closure.drawData->Get(), &Wrappers::bufferSwapper, closure.opengl);
		}
	}

#define WITH_CONSOLE
#ifdef WITH_CONSOLE
	typedef my::Console	ConsoleBase;
#else
	typedef struct {
		bool IsValid (void) const { return true; }
		template <typename T> void operator << (T const&) { }
	}	ConsoleBase;
#endif
	struct Console: public ConsoleBase {};

	Console* info_console(NULL);


	// support for util and ddebug
	namespace ddebug {
		static void onError (char const* str) {
			info_console->WriteToOutputStreamA(str);
		}
	}


	static void buffahSwappah (void* const clos) {
		DASSERT(clos == NULL);
		glutSwapBuffers();
	}

	static void* drawData(NULL);
	static void drawinit (void) {
		drawData = my::drawing::setup();
	}
	static void draw (void) {
		my::drawing::draw(drawData, &buffahSwappah, NULL);
	}
	static void drawcleanup (void) {
		my::drawing::cleanup(drawData);
	}

	struct {
		operator bool (void) const {
			std::ifstream ifs("../shaders/vertex2.txt");
			std::string line0;
			getline(ifs, line0);
			return line0 == std::string("// yes");
		}
	} BE_A_SHADER_COMPILER;

}


// implement global instances
namespace my { namespace global {
	namespace log {
		void info (LPCTSTR msg) {
			_::info_console->WriteToOutputStream(_T("[INFO]: "));
			_::info_console->WriteToOutputStream(msg);
		}

		void infoA (char const* const msg) {
			_::info_console->WriteToOutputStreamA("[INFO]: ");
			_::info_console->WriteToOutputStreamA(msg);
		}
	} // namespace log
}} // namespace my::global

namespace my {
	int APIENTRY WinMain0 (
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPTSTR    lpCmdLine,
			int       nCmdShow) {

		{
			char progname[] = "satubublis";
			int argc(1);
			char* argv[] = { &progname[0] };
			glutInit(&argc, &argv[0]);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
			glutInitWindowSize(800, 600);
			glutCreateWindow("Universe control");
			glutDisplayFunc(&_::draw);
		}

		_::Console		console;
		PASSERT(console.IsValid())

		_::info_console = &console;

		dinit(&_::ddebug::onError);
		d3dtost::Initialise();

		{
			my::OpenGL opengl;

			if (opengl.Initialise() && opengl.IsValid()) {
				console << _T("Hello comrades\n");// << std::endl;

				_::drawinit();


				///////////////
				glutMainLoop();
				///////////////


				_::drawcleanup();
			}
		}

		d3dtost::CleanUp();
		dclose();
		system("pause");
		_::info_console = NULL;

		return 0;
	}

	int APIENTRY WinMain (
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPTSTR    lpCmdLine,
			int       nCmdShow) {

		_::Console		console;
		PASSERT(console.IsValid())

		_::info_console = &console;

		dinit(&_::ddebug::onError);
		d3dtost::Initialise();

		{

			my::Window		window(hInstance);
			my::Direct3D9	direct3d;
			my::OpenGL		opengl(window);


			if (true
					&& window.IsValid()
					&& direct3d.IsValid()
					&& opengl.Initialise()
					&& opengl.IsValid()
				) {
				console << _T("Hello comrades\n");// << std::endl;

				_::DrawData drawData;
				drawData.Set(my::drawing::setup());

				_::MainLoopCallbackClosure mainLoopClosure;
				mainLoopClosure.opengl = &opengl;
				mainLoopClosure.drawData = &drawData;

				window.SetMainLoopCallback(&_::Callbacks::MainLoop, &mainLoopClosure);

				extern void TestCompileShaders (void);

				if (_::BE_A_SHADER_COMPILER)
					TestCompileShaders();
				else
					window.MainLoop(lpCmdLine, nCmdShow);

				my::drawing::cleanup(drawData.Get());
			}
		}

		d3dtost::CleanUp();
		dclose();
		system("pause");
		_::info_console = NULL;

		return 0;
	}


}

int APIENTRY _tWinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR    lpCmdLine,
		int       nCmdShow) {
	// run main twice, so as to ensure clean-up works correctly
	return
	//	my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) == 0 &&
	//	my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) == 0 &&
		my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
