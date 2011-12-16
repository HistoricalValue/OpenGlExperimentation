#include "stdafx.h"
#include <drawing_setup_opengl_adapters.h>

///////////////////////////////////////////////////////////

namespace	_ {

///////////////////////////////////////////////////////////

void SetupOpenGlAdapters (void) {
	gl::adapt::VertexArrayManager::SingletonCreate();
	gl::adapt::VertexbufferManager::SingletonCreate();
	gl::adapt::FramebufferManager::SingletonCreate();
	gl::adapt::RenderbufferManager::SingletonCreate();
}

///////////////////////////////////////////////////////////

void CleanUpOpenGlAdapters (void) {
	gl::adapt::RenderbufferManager::SingletonDestroy();
	gl::adapt::FramebufferManager::SingletonDestroy();
	gl::adapt::VertexbufferManager::SingletonDestroy();
	gl::adapt::VertexArrayManager::SingletonDestroy();
}

///////////////////////////////////////////////////////////

}	// _

///////////////////////////////////////////////////////////