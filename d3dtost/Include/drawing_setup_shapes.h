#ifndef __DRAWING__SETUP_SHAPES___H__
#define __DRAWING__SETUP_SHAPES___H__

#include <drawing.h>
#include <drawing_nurbs.h>

///////////////////////////////////////////////////////

namespace	_	{

///////////////////////////////////////////////////////

template <
		typename VertexDataType,
		VertexDataType*	(my::gl::shapes::Shape::* const GetVertexDataMemfunc) (void* memory, size_t bytesize) const,
		size_t (* const StrideFunc) (void),
		void* (* const PositionOffsetPointerFunc) (void),
		void* (* const ColouringDataOffsetFunc) (void),
		void* (* const NormalOffsetPointerFunc) (void),
		void* (* const AOOffsetPointerFunc) (void),
		typename ColouringDataIndexProviderType>
struct SetAttributeSettings {
	typedef VertexDataType		VertexData;

	static voidp	GetVertexData (my::gl::shapes::Shape const& shape, void* const memory, size_t const bytesize)
						{ return DNULLCHECK((shape.*GetVertexDataMemfunc)(memory, bytesize)); }

	static size_t	Stride (void)
						{ return (*StrideFunc)(); }

	static voidp	PositionOffset (void)
						{ return (*PositionOffsetPointerFunc)(); }

	static voidp	ColouringDataOffset (void)
						{ return (*ColouringDataOffsetFunc)(); }

	static voidp	NormalsOffset (void)
						{ return (*NormalOffsetPointerFunc)(); }

	static voidp	AOOffset (void)
						{ return (*AOOffsetPointerFunc)(); }

	static GLuint	ColouringDataIndex (void)
						{ return GLuint(ColouringDataIndexProviderType()); }
};

template <const bool textured> struct SetAttributeSettingsCreator;
template <> struct SetAttributeSettingsCreator<false> {
	typedef	SetAttributeSettings<
					my::gl::shapes::VertexData,
					&my::gl::shapes::Shape::GetVertexData,
					&my::gl::shapes::VertexData::Stride,
					&my::gl::shapes::VertexData::PositionOffsetPointer,
					&my::gl::shapes::VertexData::ColourOffsetPointer,
					&my::gl::shapes::VertexData::NormalOffsetPointer,
					&my::gl::shapes::VertexData::AOOffsetPointer,
					my::OpenGL::_VAI_COLOUR
				> Settings;
};
template <> struct SetAttributeSettingsCreator<true> {
	typedef	SetAttributeSettings<
					my::gl::shapes::TexturedVertexData,
					&my::gl::shapes::Shape::GetTexturedVertexData,
					&my::gl::shapes::TexturedVertexData::Stride,
					&my::gl::shapes::TexturedVertexData::PositionOffsetPointer,
					&my::gl::shapes::TexturedVertexData::TextureCoordinatesOffsetPointer,
					&my::gl::shapes::TexturedVertexData::NormalOffsetPointer,
					&my::gl::shapes::TexturedVertexData::AOOffsetPointer,
					my::OpenGL::_VAI_TEXCOORD
				> Settings;
};

template <const bool textured>
static void SetAttribute (
	GLuint const					vao,
	my::gl::adapters::Buffer* const	vbo,
	my::gl::shapes::Shape const&	shape,
	GLboolean const					normalised,
	GLuint&							numberOfPoints)
{
	using namespace	my::gl::shapes;
	using namespace	my::gl::math;
	using			my::OpenGL;

	typedef typename SetAttributeSettingsCreator<textured>::Settings	Settings;

	prim::VertexArray::Bind(vao);
	PASSERT(prim::VertexArray::Is(vao))

	size_t const	count		(shape.GetNumberOfVertices());
	size_t const	bytesize	(std::max(count * sizeof(typename Settings::VertexData), 1u));
	void* const		_data		(AllocateSingleAllocationBufferMemory(bytesize));
	void* const		data		(Settings::GetVertexData(shape, _data, bytesize));
	GLsizei const	stride		(psafecast<GLsizei>(Settings::Stride()));
	voidp const		attr1off	(Settings::PositionOffset());
	voidp const		attr2off	(Settings::ColouringDataOffset());
	voidp const		attr3off	(Settings::NormalsOffset());
	voidp const		attr4off	(Settings::AOOffset());
	GLuint const	attr2index	(Settings::ColouringDataIndex());

	{
		IneffectiveBufferEntryDeleter deleter;
		size_t const bufferEntryOffset = vbo->Add(data, 1, bytesize, &deleter);
		PASSERT(bufferEntryOffset == 0)
		vbo->Commit();
		vbo->Bind();
	}

	DeallocateSingleAllocationBufferMemory(_data);

	glVertexAttribPointer(psafecast<GLuint>(OpenGL::VAI_POSITION.operator GLint()),	4,	GL_FLOAT,	psafecast<GLboolean>(!normalised),	stride,	attr1off);
	glVertexAttribPointer(attr2index,												4,	GL_FLOAT,	psafecast<GLboolean>(!normalised),	stride,	attr2off);
	glVertexAttribPointer(psafecast<GLuint>(OpenGL::VAI_NORMAL.operator GLint()),	4,	GL_FLOAT,	psafecast<GLboolean>(!normalised),	stride,	attr3off);
	glVertexAttribPointer(psafecast<GLuint>(OpenGL::VAI_AOFACTOR.operator GLint()),	1,	GL_FLOAT,	psafecast<GLboolean>(!normalised),	stride, attr4off);

	glEnableVertexAttribArray(psafecast<GLuint>(OpenGL::VAI_POSITION.operator GLint())		);
	glEnableVertexAttribArray(attr2index													);
	glEnableVertexAttribArray(psafecast<GLuint>(OpenGL::VAI_NORMAL.operator GLint())		);
	glEnableVertexAttribArray(psafecast<GLuint>(OpenGL::VAI_AOFACTOR.operator GLint())		);

	numberOfPoints = count;

	vbo->Unbind();
}



static const float POOP_LIGHT[4] = { 6.0f, 2.0f, 0.0f, 1.0f };
static GLboolean const POINTS_NORMALISED(GL_TRUE);
///////////////////////////////////////////////////////
// Object setups
///////////////////////////////////////////////////////
static
void SetupPointShapes (
		DrawData&						,
		GLuint const					vertexArrayId,
		my::gl::adapters::Buffer* const	buffer0,
		my::gl::adapters::Buffer* const	,
		GLuint&							numberOfPointPoints)
{
	// Save setup time
	if (_::WITH_DRAW_POINTS) {
		using namespace my::gl::shapes;
		ShapeCompositionFactory f;

		f.Add(Point(Vertex(my::gl::math::Vector4::New(_::POOP_LIGHT)), ColourFactory::White()));

		DynamicShapeComposition* const dcomp(f.Generate());

		SetAttribute<false>(vertexArrayId, buffer0, *dcomp, POINTS_NORMALISED, numberOfPointPoints);

		f.Dispose(dcomp);
	}
}

static
void SetUpLineShapes (
		DrawData&						dd,
		GLuint const					vertexArrayId,
		my::gl::adapters::Buffer* const	buffer0,
		my::gl::adapters::Buffer* const	,
		GLuint&							numberOfPoints)
{
	// Save setup time
	if (_::WITH_DRAW_LINES) {
		using namespace my::gl::shapes;
		using namespace my::gl::math;

		Nothing nothing;
		{
			timer t00("axes cleanup");
			Axes axs;
			{
				timer t02("factory cleanup");
				ShapeCompositionFactory f;

				f.Add(axs);
#if WITH_NORMALS == 1
				my::drawing::nurbs::addnormalsto(_::getmeshelements(dd), f);
#endif

				DynamicShapeComposition* dcomp(NULL);
				{
					timer t01("line shapes generation");
					dcomp = (f.Generate());
				}

				{
					Shape& shape(
						//	axs
						//	nothing
							*dcomp
						);

					SetAttribute<false>(vertexArrayId, buffer0, shape, POINTS_NORMALISED, numberOfPoints);
				}

				{
					timer t01("disposing of a dynamic shape composition");
					f.Dispose(dcomp);
				}

				t02.restart();
			}

			t00.restart();
		}
	}
}

static
void SetUpTriangleObjects (
		DrawData&						dd,
		GLuint const					vertexArrayId,
		my::gl::adapters::Buffer* const	buffer0,
		my::gl::adapters::Buffer* const	,
		GLuint&							numberOfWorldCubeLineSegments)
{
	// Save setup time
	if (_::WITH_DRAW_TRIANGLES) {
		using namespace my::gl::shapes;
		using namespace my::gl::math;

		timer t02("cleaning up factory (surf triangles)");
		ShapeCompositionFactory	f;
		{
			timer t03("adding surface triangles to factory");
			my::drawing::nurbs::addastrianglesto(_::getmeshelements(dd), f);
		}

		DynamicShapeComposition* dcomp(NULL);
		{
			timer t03("producing surface triangles from factory into dynamic composition");
			dcomp = f.Generate();
			my::global::log::info(format(_T("(surface is %ld (%ld) triangles)\n"), f.GetNumberOfAddedShapes(), dcomp->GetNumberOfAddedShapes()));
		}

		Nothing								nothing;
		{
			Shape& shape(
			//	nothing
				*dcomp
				);

			SetAttribute<false>(vertexArrayId, buffer0, shape, POINTS_NORMALISED, numberOfWorldCubeLineSegments);
		}

		{
			timer t03("destroying dynamic composition (surf triangl)");
			f.Dispose(dcomp);
		}

		t02.restart();
	}
}

static
void SetUpTexturedTriangleObjects (
		DrawData&						dd,
		GLuint const					vertexArrayId,
		my::gl::adapters::Buffer* const	buffer0,
		my::gl::adapters::Buffer* const	,
		GLuint&							numberOfTexturedSegments)
{
	// Save setup time
	if (_::WITH_DRAW_TEXTURED) {
		using namespace my::gl::shapes;
		using namespace my::gl::math;

		/////////////

		ShapeCompositionFactory f;
		my::drawing::nurbs::addastrianglesto(_::getmeshelements(dd), f);
		DynamicShapeComposition* const dcomp(f.Generate());

		/////////////

		PASSERT(SolidCube::GetSolidCubeNumberOfVertices() == 36u)

		SolidCube							companion0;
		SolidCube							companion1;
		SolidCube							companion2;
		SolidCube							companion3;
		SolidCube							companion4;

		NShapesComposition<5>				companions;
		companions.Add(&companion0);
		companions.Add(&companion1);
		companions.Add(&companion2);
		companions.Add(&companion3);
		companions.Add(&companion4);

		companions.Scale( 0.125f);
		companion0.Adjust(Vector4::New(-3.f  * 0.250f, 0.f, 0.f, 0.f));
		companion1.Adjust(Vector4::New(-1.5f * 0.250f, 0.f, 0.f, 0.f));
		companion2.Adjust(Vector4::New(-0.f  * 0.250f, 0.f, 0.f, 0.f));
		companion3.Adjust(Vector4::New( 1.5f * 0.250f, 0.f, 0.f, 0.f));
		companion4.Adjust(Vector4::New( 3.f  * 0.250f, 0.f, 0.f, 0.f));
		companions.Adjust(Vector4::New(0.f, 0.125f, 0.f, 0.f));

		companion0.SetColour(ColourFactory::LightRed());
		companion1.SetColour(ColourFactory::LightGreen());
		companion2.SetColour(ColourFactory::LightBlue());
		companion3.SetColour(ColourFactory::LightYellow());
		companion4.SetColour(ColourFactory::LightPurple());

	/////////////

		Plane plane(ColourFactory::LightYellow());

		SolidCube compos;
		compos.Scale(0.250f);
	/////////////

		NShapesComposition<3> scenery;
		scenery.Add(&companions);
		scenery.Add(&plane);
		scenery.Add(dcomp);

		Nothing nothing;
		// Upload shape as textured, buffer 2
		{
			Shape& shape(
				scenery
			//	compos
			//	nothing
			//	*dcomp
				);
			SetAttribute<true>(vertexArrayId, buffer0, shape, POINTS_NORMALISED, numberOfTexturedSegments);
		}

		f.Dispose(dcomp);
	}
}

static inline
void SetUpShapes (
		DrawData&						dd,
		//
		GLuint const					line_vertexArrayId,
		my::gl::adapters::Buffer* const	line_buffer0,
		my::gl::adapters::Buffer* const	line_buffer1,
		GLuint&							numberOfPoints,
		//
		GLuint const					tria_vertexArrayId,
		my::gl::adapters::Buffer* const	tria_buffer0,
		my::gl::adapters::Buffer* const	tria_buffer1,
		GLuint&							numberOfWorldCubeLineSegments,
		//
		GLuint const					text_vertexArrayId,
		my::gl::adapters::Buffer* const	text_buffer0,
		my::gl::adapters::Buffer* const	text_buffer1,
		GLuint&							numberOfTexturedSegment)
{
	SetUpLineShapes(dd, line_vertexArrayId, line_buffer0, line_buffer1, numberOfPoints);
	SetUpTriangleObjects(dd, tria_vertexArrayId, tria_buffer0, tria_buffer1, numberOfWorldCubeLineSegments);
	SetUpTexturedTriangleObjects(dd, text_vertexArrayId, text_buffer0, text_buffer1, numberOfTexturedSegment);
}

///////////////////////////////////////////////////////

}	//	_

///////////////////////////////////////////////////////

#endif	// __DRAWING__SETUP_SHAPES___H__
