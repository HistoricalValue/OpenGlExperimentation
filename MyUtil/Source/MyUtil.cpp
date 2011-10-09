// MyUtil.cpp : Defines the exported functions for the DLL application.
//

#include <stdafx.h>
#include <MyUtil.h>
#include <my/gl/shapes/ShapeUtilities.h>

// This is an example of an exported variable
MYUTIL_API int nMyUtil=0;

// This is an example of an exported function.
MYUTIL_API int fnMyUtil(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see MyUtil.h for the class definition
CMyUtil::CMyUtil()
{
	return;
}

///////////////////////////////////////////////////

namespace my {

	void Initialise (void) {
		gl::shapes::ShapeUtilities::Initialise();
	}

	void CleanUp (void) {
		gl::shapes::ShapeUtilities::CleanUp();
	}

} // namespace my
