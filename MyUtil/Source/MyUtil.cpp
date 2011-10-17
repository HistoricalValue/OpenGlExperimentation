// MyUtil.cpp : Defines the exported functions for the DLL application.
//

#include <stdafx.h>
#include <MyUtil.h>
#include <my/gl/shapes/ShapeUtilities.h>
#include <my/gl/adapters/BufferManager.h>

///////////////////////////////////////////////////

namespace my {

	void Initialise (void) {
		gl::shapes::ShapeUtilities::Initialise();
		gl::adapters::BufferManager::SingletonCreate();

	}

	void CleanUp (void) {
		gl::adapters::BufferManager::SingletonDestroy();
		gl::shapes::ShapeUtilities::CleanUp();
	}

} // namespace my
