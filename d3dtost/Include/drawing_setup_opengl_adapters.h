#ifndef __DRAWING__SETUP_OPENGL_ADAPTERS__H__
#define __DRAWING__SETUP_OPENGL_ADAPTERS__H__

#include <drawing.h>

///////////////////////////////////////////////////////////

namespace	_ {

///////////////////////////////////////////////////////////

static
void SetupOpenGlAdapters (void) {
	gl::adapt::VertexArrayManager::SingletonCreate();
	
	gl::adapt::FramebufferManager::SingletonCreate();
	gl::adapt::RenderbufferManager::SingletonCreate();
}

///////////////////////////////////////////////////////////

static
void CleanUpOpenGlAdapters (void) {
	gl::adapt::RenderbufferManager::SingletonDestroy();
	gl::adapt::FramebufferManager::SingletonDestroy();

	gl::adapt::VertexArrayManager::SingletonDestroy();
}

///////////////////////////////////////////////////////////

}	// _

///////////////////////////////////////////////////////////

#endif // __DRAWING__SETUP_OPENGL_ADAPTERS__H__
