// MyUtil.cpp : Defines the exported functions for the DLL application.
//

#include <stdafx.h>
#include <MyUtil.h>
#include <my/gl/shapes/ShapeUtilities.h>

///////////////////////////////////////////////////

namespace my {

    void Initialise (void) {
        gl::shapes::ShapeUtilities::Initialise();
    }

    void CleanUp (void) {
        gl::shapes::ShapeUtilities::CleanUp();
    }

} // namespace my
