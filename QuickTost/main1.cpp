#include <my/gl/shapes/Axes.h>
#include <my/gl/shapes/ShapeCompositionFactory.h>
#include <stdio.h>
#include <DDebug.h>
#include <PProtectedClass_inl.h>
#include <PFunctors_inl.h>
#include <my/gl/shapes/ShapeUtilities.h>

static inline void waitre (void) {
#ifdef _DEBUG
    puts("Type \"return\" to continue.");
    _fgetchar();
#endif
}

static void NoShapeCompositionSelfValidation (my::gl::shapes::ShapeComposition*) {
}

static void DDebugShapeCompositionSelfValidation (my::gl::shapes::ShapeComposition* const self) {
    DASSERTPTR(self);
}



static void onerror ( char const* const msg ) {
    puts(msg);
}

int main1 (int, char**) {
    using namespace my::gl::shapes;
    using namespace my::gl::math;

    dinit(&onerror);
    codeshare::utilities::Initialise();
    my::Initialise();

    {
    //    Axes axes;
        const size_t szAxes(sizeof(Axes));
        const size_t szLines(sizeof(codeshare::utilities::Placeholder<Line>[300]));
        const size_t szOneLines(sizeof(codeshare::utilities::Placeholder<Line>));
        const size_t szLine(sizeof(Line));
        const size_t szVertex(sizeof(Vertex));
        const size_t szColour(sizeof(Colour));
        const size_t szNSHC(sizeof(NShapesComposition<300>));
        const size_t szSNSC(sizeof(SpecificNumberOfShapesComposition));
        const size_t szSC(sizeof(ShapeComposition));
        const size_t szShape(sizeof(Shape));

        Axes* const axes_p = DNEW(Axes);
        Axes& axes(*axes_p);

        Axes* const caxes_p = ShapeUtilities::CloneShape(axes);

        ShapeUtilities::DisposeClonedShape(caxes_p);
        DDELETE(axes_p);
    }


    codeshare::utilities::CleanUp();
    dclose();

    waitre();
    return 0;
}

