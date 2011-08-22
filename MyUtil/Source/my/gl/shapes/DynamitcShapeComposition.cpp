#include <stdafx.h>

//////////////////////////////////////////////////////////

namespace my { namespace gl { namespace shapes {

//////////////////////////////////////////////////////////

bool DynamicShapeComposition::IsFull (void) const {
	return SpecificNumberOfShapesComposition::IsFull();
}

//////////////////////////////////////////////////////////

void DynamicShapeComposition::Add (Shape const& shape) {
	PASSERT(!IsFull())
	SpecificNumberOfShapesComposition::Add(ShapeUtilities::CloneShape(shape));
}

//////////////////////////////////////////////////////////

DynamicShapeComposition::DynamicShapeComposition (size_t const numberOfShapes):
	SpecificNumberOfShapesComposition(numberOfShapes)
	{ }

//////////////////////////////////////////////////////////

DynamicShapeComposition::DynamicShapeComposition (DynamicShapeComposition const& other):
	SpecificNumberOfShapesComposition(other.GetCapacity()) {
	PASSERT(other.IsFull())

	Shape const* const* const shapes= (other.GetShapesArray());						// stupid microsoft -- cannot ()-init
	Shape const* const* const end	= (shapes + other.GetNumberOfAddedShapes());	// stupid microsoft -- cannot ()-init
	for (Shape const* const* i = shapes; i != end; ++i)
		Add(**i);

	PASSERT(IsFull())
}

//////////////////////////////////////////////////////////

DynamicShapeComposition::~DynamicShapeComposition (void) {
	PASSERT(IsFull())
	Shape* const* const shapes = (GetShapesArray()); // stupid microsoft -- cannot ()-init
	std::for_each(shapes, shapes + GetNumberOfAddedShapes(), uptr_fun(ShapeUtilities::DisposeClonedShape));
}

//////////////////////////////////////////////////////////

DynamicShapeComposition* DynamicShapeComposition::Clone (void* const mem, size_t const bytesize) const {
	size_t const requiredSize(GetSizeOf());
	PASSERT(bytesize >= requiredSize);

	DynamicShapeComposition* result(NULL);
	if (bytesize >= requiredSize)
		result = new(mem) DynamicShapeComposition(*this);

	return result;
}

//////////////////////////////////////////////////////////

size_t DynamicShapeComposition::GetSizeOf (void) const {
	return sizeof(DynamicShapeComposition);
}

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

}}} // namespace my::gl::shapes

//////////////////////////////////////////////////////////