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
		void onError (char const* str) {
			info_console->WriteToOutputStreamA(str);
		}
	}

}


// implement global instances
namespace my { namespace global {
	namespace log {
		void info (LPCTSTR msg) {
			_::info_console->WriteToOutputStream(_T("[INFO]: "));
			_::info_console->WriteToOutputStream(msg);
		}
	} // namespace log
}} // namespace my::global

namespace my {
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
		my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) == 0 &&
	//	my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) == 0 &&
		my::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
