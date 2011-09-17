#include <stdafx.h>

//////////////////////////////////////////////////////////
namespace	{
namespace _ {
//////////////////////////////////////////////////////////

typedef std::list<my::gl::shapes::Shape*> ShapesList;
struct InstanceData {
	ShapesList		shapes;
};
#define SHAPES			(DPTR(reinterpret_cast<_::InstanceData* const>(data))->shapes)
#define SHAPES_CONST	(DPTR(reinterpret_cast<_::InstanceData const* const>(data))->shapes)

//////////////////////////////////////////////////////////
}
}
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

namespace my { namespace gl { namespace shapes {

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

P_INLINE
void ShapeCompositionFactory::Add (Shape const& shape) {
	SHAPES.push_back(ShapeUtilities::CloneShape(shape));
}

//////////////////////////////////////////////////////////

P_INLINE
void ShapeCompositionFactory::Reset (void) {
	std::for_each(SHAPES.begin(), SHAPES.end(), uptr_fun(&ShapeUtilities::DisposeClonedShape));
 	SHAPES.clear();
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
	data(DNEW(_::InstanceData))
	{ }

//////////////////////////////////////////////////////////

P_INLINE
ShapeCompositionFactory::~ShapeCompositionFactory (void) {
	Reset();
	DDELETE(reinterpret_cast<_::InstanceData* const>(data));
}

//////////////////////////////////////////////////////////

}}} // namespace my::gl::shapes

//////////////////////////////////////////////////////////