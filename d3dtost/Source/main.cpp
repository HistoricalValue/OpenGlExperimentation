#include "stdafx.h"

#if defined(INTERNAL_H)
#	error early shit
#endif

#if defined(_WINDOWS_)
#	error shit
#endif

#include "TheCursed.h"

#define WITH_TRIPLE_CLEANUP 0

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
	struct Console: public ConsoleBase {
		Console (void): ConsoleBase() {}
	private:
		Console (Console const&);
		void operator = (Console const&);
	};

	Console* info_console(NULL);


	// support for util and ddebug
	namespace ddebug {
		static void onError (char const* str) {
			info_console->WriteToOutputStreamA(str);
		}

		static void asserter (bool const val, const char* const expr, uvoidvoid_f const vvf) {
			if (!val) {
				(*vvf)();

				(void) expr;
				int x = 6;
				int y = x;
				(void) y;

				_assert(!"Ze FAILR");
			}
		}
	}

	struct {
		operator bool (void) const {
			std::ifstream ifs("../shaders/vertex2.txt");
			std::string line0;
			getline(ifs, line0);
			return line0 == std::string("// yes");
		}
	} BE_A_SHADER_COMPILER;

	struct _BE_A_MESH_TOOL {
		operator bool (void) const {
			if (yes)
				return true;

			std::ifstream ifs("../shaders/vertex.c");
			std::string line0;
			getline(ifs, line0);
			return line0 == std::string("// mesh tool");
		}
		bool yes;

		_BE_A_MESH_TOOL (void): yes(false) {}
	} BE_A_MESH_TOOL;
}


// implement global instances
namespace my { namespace global {
	namespace log {
		void info (TCHAR const* msg) {
			_::info_console->WriteToOutputStream(_T("[INFO]: "));
			_::info_console->WriteToOutputStream(msg);
		}

		void infoA (char const* const msg) {
			_::info_console->WriteToOutputStreamA("[INFO]: ");
			_::info_console->WriteToOutputStreamA(msg);
		}
	} // namespace log

	Console&	GetConsole (void)
					{ return *DNULLCHECK(_::info_console); }

}} // namespace my::global

namespace my {

	int APIENTRY WinMain (
			HINSTANCE hInstance,
			HINSTANCE P_UNUSED_PARAMETER(hPrevInstance),
			TCHAR*    lpCmdLine,
			int       nCmdShow) {

		_::Console		console;
		PASSERT(console.IsValid())

		_::info_console = &console;

		dinit(&_::ddebug::onError);
		dsetassertfunc(&_::ddebug::asserter);
		UtilPackage::Initialise();
		d3dtost::Initialise();

		if (_::BE_A_MESH_TOOL) {
			extern void MeshProcess (void);
			console << "Being a MESH TOOL\n";
			MeshProcess();
		}
		else
		{

			my::Window		window(hInstance);
			my::Direct3D9	direct3d;
			my::OpenGL		opengl(window);


			if (	   window.IsValid()
					&& direct3d.IsValid()
					&& opengl.Initialise()
					&& opengl.IsValid()
				) {
				console << "Hello comrades\n";// << std::endl;

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
		UtilPackage::CleanUp();
		dclose();
		_::info_console = NULL;

		return 0;
	}


}

int APIENTRY _tWinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		TCHAR*    lpCmdLine,
		int       nCmdShow) {
	_::BE_A_MESH_TOOL.yes = true;
	// run main twice, so as to ensure clean-up works correctly
	return
		my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) == 0	&&
		!(_::BE_A_MESH_TOOL.yes = false)								&&
		(_::BE_A_MESH_TOOL ||
#if WITH_TRIPLE_CLEANUP
		my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) == 0	&&
		my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) == 0	&&
#endif
		my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
		);
}
