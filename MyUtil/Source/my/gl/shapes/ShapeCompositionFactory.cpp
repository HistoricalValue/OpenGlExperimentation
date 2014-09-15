#include <stdafx.h>
#include <PQuickDispose.h>

#include <my/gl/shapes/ShapeCompositionFactory.h>
#include <my/gl/shapes/ShapeUtilities.h>
#include <MyGlobalInstances.h>

//////////////////////////////////////////////////////////
namespace    {
namespace _ {
//////////////////////////////////////////////////////////

typedef std::list<my::gl::shapes::Shape*> ShapesList;

#define SHAPES            shapes
#define SHAPES_CONST    static_cast<_::ShapesList const&>(shapes)

//////////////////////////////////////////////////////////
}
}
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

namespace my { namespace gl { namespace shapes {

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

P_INLINE
size_t ShapeCompositionFactory::GetNumberOfAddedShapes (void) const {
    return SHAPES_CONST.size();
}

//////////////////////////////////////////////////////////

P_INLINE
void ShapeCompositionFactory::Add (Shape const& shape) {
    SHAPES.push_back(ShapeUtilities::CloneShape(shape));
}

//////////////////////////////////////////////////////////

P_INLINE
void ShapeCompositionFactory::Reset (void) {
    {
        unsigned long int const t0 = ugettime();
        std::for_each(SHAPES.begin(), SHAPES.end(), uptr_fun(&ShapeUtilities::DisposeClonedShape));
        unsigned long int const t1 = ugettime();
        TCHAR buf[1024];
        _sntprintf_s(&buf[0], _countof(buf), _countof(buf), _T("it's all here: %ld\n"), t1-t0);
        my::global::logger::Get().Info(my::String(buf));
    }

    {
        unsigned long int const t0 = ugettime();
        SHAPES.clear();
        unsigned long int const t1 = ugettime();
        TCHAR buf[1024];
        _sntprintf_s(&buf[0], _countof(buf), _countof(buf), _T("or here: %ld\n"), t1-t0);
        my::global::logger::Get().Info(my::String(buf));
    }
}

//////////////////////////////////////////////////////////

P_INLINE
DynamicShapeComposition* ShapeCompositionFactory::Generate (void) const {
    DynamicShapeComposition* const result(DNEWCLASS(DynamicShapeComposition, (SHAPES_CONST.size())));

    {
        _::ShapesList::const_iterator const end(SHAPES_CONST.end());
        for (_::ShapesList::const_iterator i(SHAPES_CONST.begin()); i != end; ++i)
            DPTR(result)->Add(**i);
    }

    PASSERT(result->IsFull());

    return result;
}

//////////////////////////////////////////////////////////

P_INLINE
void ShapeCompositionFactory::Dispose (DynamicShapeComposition* const comp) const {
    DDELETE(comp);
}

//////////////////////////////////////////////////////////

P_INLINE
ShapeCompositionFactory::ShapeCompositionFactory (void):
    shapes()
    { }

//////////////////////////////////////////////////////////

P_INLINE
ShapeCompositionFactory::~ShapeCompositionFactory (void) {
    Reset();
}

//////////////////////////////////////////////////////////

}}} // namespace my::gl::shapes

//////////////////////////////////////////////////////////