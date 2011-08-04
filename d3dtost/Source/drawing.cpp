#include "stdafx.h"


#define DONT	if (false)
#define DO		if (true)

#define TEXTURE_TYPE_2D		0x00
#define TEXTURE_TYPE_3D		0x01
#define TEXTURE_TYPE		TEXTURE_TYPE_3D

using namespace ::gl::ext;

namespace _ {
	static const float WW(2000.f);

	static const size_t VAOs(4);
	static const size_t VBOs(6);
	static const size_t VTOs(1);
	static const size_t TEXTURES_NUM(2);
	static const size_t IMAGES_NUM(2);

	static const GLuint COLOUR_WITH_COLOUR(0);
	static const GLuint COLOUR_WITH_TEXTURE(1);

	static const GLuint TexturesUnits[TEXTURES_NUM] =
		{ 12, 23 };

	typedef ankh::images::Image*		ImagesArray[IMAGES_NUM];
	typedef ankh::textures::Texture*	TexturesArray[TEXTURES_NUM];

	struct DrawData {
		GLuint				vertexArrayIds[VAOs];
		GLuint				bufferIds[VBOs];
		GLuint				texturesIds[TEXTURES_NUM];
		GLuint				numberOfPoints;
		GLuint				numberOfWorldCubeLineSegments;
		unsigned long int	startingTime;
		unsigned long int	prevtime;
		GLuint				sampler_location;
		glt::TGADecoder*	targa;
		TexturesArray		textures;
		ImagesArray			images;
		GLuint				numberOfTexturedSegments;
		size_t				previousTextureIndex;
	};


	template <typename T, const unsigned int N>
	struct Arrayfier {
		typedef T (*arr)[N];
	};

	P_INLINE
	static void errorHandler (LPCTSTR const message) {
		PASSERT(false)
	}

	P_INLINE
	static float GetRotationAngle (unsigned long int const dt_milli) {
		// times in milliseconds
		// ----
		// we want one round per second => w = rad/sec = 2pi/sec => ang = w*sec = 2pi*sec = 360*sec (%360)
		// 360 * dt * 1e-3  = 0.360 * dt
		float const result(.045f * dt_milli);
		return result - floorf(result/360.f)*360.f;
	}

	P_INLINE
	static GLuint GetTextureZ (unsigned long int const dt_milli) {
		// we want a change every second/8, total changes = 32
		// 0~1, 1~2, 2~3, ...

#if TEXTURE_TYPE == TEXTURE_TYPE_3D
		// return (dt_milli / 125) % 16;		// 16 changes over 8 seconds
		// return (dt_milli / 500) % 3;			// 3 changes over 1.5 seconds
		return (dt_milli / 250) % 11;			// 10 changes over 2.5 seconds
#elif TEXTURE_TYPE == TEXTURE_TYPE_2D
		return 0;
#else
#	error ""
#endif
	}

	P_INLINE
	static size_t GetTextureIndex (unsigned long int const dt_milli) {
		// there are TEXTURES_NUM textures and we want to keep each one for 3 seconds
		UCOMPILECHECK(_::TEXTURES_NUM == sizeof(_::TexturesUnits)/sizeof(_::TexturesUnits[0]))
		return _::TexturesUnits[(dt_milli / ((3 * 1000) / _::TEXTURES_NUM )) % _::TEXTURES_NUM];
	}

	static void* __last_static_buffer_allocation(NULL);
	static size_t __last_static_buffer_allocation_size(0);
	P_INLINE
	static void* AllocateSingleAllocationBufferMemory (size_t const size) {

		void* const result(codeshare::utilities::GlobalSingleAllocationBuffer::Get().Allocate(size));

		P_DEBUG_STMT(__last_static_buffer_allocation = result;)
		P_DEBUG_STMT(__last_static_buffer_allocation_size = size;)

		return result;
	}

	P_INLINE
	static void* ReallocateSingleAllocationBufferMemory (size_t const size) {
		P_DEBUG_STMT(__last_static_buffer_allocation_size = size;)

		void* const result(codeshare::utilities::GlobalSingleAllocationBuffer::Get().Reallocate(size));

		P_DEBUG_STMT(__last_static_buffer_allocation = result;)

		return result;
	}

	P_INLINE
	static void DeallocateSingleAllocationBufferMemory (void* ptr) {
		PASSERT(ptr == __last_static_buffer_allocation)
		codeshare::utilities::GlobalSingleAllocationBuffer::Get().ReleaseArrayOf<char>(__last_static_buffer_allocation_size);
	}

	static const float FLOAT_EQUALITY_MARGIN(1e-6f);
	P_INLINE
	static bool eq (float const f1, float const f2) {
#ifdef P_DEBUG
		bool const f2_gt_f1(f2 > f1);
		float const diff(f2_gt_f1? f2-f1 : f1-f2);
		bool const diff_too_small(diff <= FLOAT_EQUALITY_MARGIN);

		return diff_too_small;
#else
		return	f2 > f1?
					f2-f1 <= FLOAT_EQUALITY_MARGIN
				:
					f1-f2 <= FLOAT_EQUALITY_MARGIN
				;
#endif
	}

	static void SetAttribute (
		GLuint const					vao,
		GLuint const					vbo,
		my::gl::shapes::Shape const&	shape,
		GLboolean const					normalised,
		bool const						textured,
		GLuint&							numberOfPoints)
	{
		using namespace	my::gl::shapes;
		using namespace	my::gl::math;
		using			my::OpenGL;

		glBindVertexArray(vao);
		PASSERT(glIsVertexArray(vao) == GL_TRUE)

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		PASSERT(glIsBuffer(vbo) == GL_TRUE)

		size_t const	count		(shape.GetNumberOfVertices());
		size_t const	bytesize	(std::max(	textured?
													count * sizeof(TexturedVertexData) :
													count * sizeof(VertexData),
												1u));
		void* const		_data		(_::AllocateSingleAllocationBufferMemory(bytesize));

		typedef void*	voidp;
		void* const		data		(textured? voidp(shape.GetTexturedVertexData(_data, bytesize)) : voidp(shape.GetVertexData(_data, bytesize)));
		PASSERT(data != NULL)
		size_t const	stride		(textured? TexturedVertexData::Stride() : VertexData::Stride());
		voidp const		attr1off	(textured? TexturedVertexData::PositionOffsetPointer() : VertexData::PositionOffsetPointer());
		voidp const		attr2off	(textured? TexturedVertexData::TextureCoordinatesOffsetPointer() : VertexData::ColourOffsetPointer());
		GLuint const	attr2index	(textured? GLuint(OpenGL::VAI_TEXCOORD) : GLuint(OpenGL::VAI_COLOUR));

		glBufferData(GL_ARRAY_BUFFER, bytesize, data, GL_STATIC_DRAW);

		_::DeallocateSingleAllocationBufferMemory(_data);

		glVertexAttribPointer(OpenGL::VAI_POSITION,	4,	GL_FLOAT,	normalised,	stride,	attr1off);
		glVertexAttribPointer(attr2index,			4,	GL_FLOAT,	normalised,	stride,	attr2off);

		glEnableVertexAttribArray(OpenGL::VAI_POSITION);
		glEnableVertexAttribArray(attr2index);

		numberOfPoints = count;
	}


	P_INLINE
	static void ApplyCamera (my::gl::shapes::Shape& shape) {
		using namespace my::gl::shapes;
		using namespace my::gl::math;

	//	shape.TranslateZ( _::WW * 0.125f * 2);
	//	shape.RotateY(M_PI/6.f);
	//	shape.RotateX(-M_PI/4.f);
	//	shape.TranslateY(_::WW * 0.125f * 1);
	}



	static GLboolean const POINTS_NORMALISED(GL_TRUE);
	///////////////////////////////////////////////////////
	// Object setups
	///////////////////////////////////////////////////////
	static
	void SetUpLineShapes (
			GLuint const	vertexArrayId,
			GLuint const	buffer0Id,
			GLuint const	buffer1Id,
			GLuint&			numberOfPoints)
	{
		using namespace my::gl::shapes;
		using namespace my::gl::math;

		PASSERT(Line::GetLineNumberOfVertices() == 2u)


		Line x(Vertex(Vector4::New(-1.f, 0.f, 0.f, 1.f)), Vertex(Vector4::New(1.f, 0.f, 0.f, 1.f)));
		Line y(Vertex(Vector4::New( 0.f,-1.f, 0.f, 1.f)), Vertex(Vector4::New(0.f, 1.f, 0.f, 1.f)));
		Line z(Vertex(Vector4::New( 0.f, 0.f,-1.f, 1.f)), Vertex(Vector4::New(0.f, 0.f, 1.f, 1.f)));

		x.SetColour(ColourFactory::LightBlue());
		y.SetColour(ColourFactory::LightRed());
		z.SetColour(ColourFactory::LightGreen());

		Shape* axesArray[3];
		ShapeComposition axes(&axesArray[0], sizeof(axesArray));
		axes.Add(&x); PASSERT(!axes.IsFull())
		axes.Add(&y); PASSERT(!axes.IsFull())
		axes.Add(&z); PASSERT(axes.IsFull())

		Nothing nothing;
		Axes axs;
		{
			Shape& shape(
					axes
				//	axs
				//	nothing
				);

			shape.Scale(500.f);
			ApplyCamera(shape);
			SetAttribute(vertexArrayId, buffer0Id, shape, POINTS_NORMALISED, false, numberOfPoints);
		}
	}

	static
	void SetUpTriangleObjects (
			GLuint const	vertexArrayId,
			GLuint const	buffer0Id,
			GLuint const	buffer1Id,
			GLuint&			numberOfWorldCubeLineSegments)
	{
		using namespace my::gl::shapes;
		using namespace my::gl::math;

		Nothing								nothing;
		{
			Shape& shape(
				nothing
			//	companions
			//	companion0
			//	plane
				);

		//	shape.Scale(_::WW / 10.f);
			_::ApplyCamera(shape);
			_::SetAttribute(vertexArrayId, buffer0Id, shape, POINTS_NORMALISED, false, numberOfWorldCubeLineSegments);
		}
	}

	static
	void SetUpTexturedTriangleObjects (
			GLuint const	vertexArrayId,
			GLuint const	buffer0Id,
			GLuint const	buffer1Id,
			GLuint&			numberOfTexturedSegments)
	{
		using namespace my::gl::shapes;
		using namespace my::gl::math;

		PASSERT(SolidCube::GetSolidCubeNumberOfVertices() == 36u)

		Shape*								shapesArray[7];

		SolidCube							companion0;
		SolidCube							companion1;
		SolidCube							companion2;
		SolidCube							companion3;
		SolidCube							companion4;

		ShapeComposition					companions(&shapesArray[0], sizeof(shapesArray));
		companions.Add(&companion0);
		companions.Add(&companion1);
		companions.Add(&companion2);
		companions.Add(&companion3);
		companions.Add(&companion4);
		PASSERT(shapesArray[0] == &companion0)
		PASSERT(shapesArray[1] == &companion1)
		PASSERT(shapesArray[2] == &companion2)
		PASSERT(shapesArray[3] == &companion3)
		PASSERT(shapesArray[4] == &companion4)

		companions.Scale( 125.f);
		companion0.Adjust(Vector4::New(-3.f  * 250.f, 0.f, 0.f, 0.f));
		companion1.Adjust(Vector4::New(-1.5f * 250.f, 0.f, 0.f, 0.f));
		companion2.Adjust(Vector4::New(-0.f  * 250.f, 0.f, 0.f, 0.f));
		companion3.Adjust(Vector4::New( 1.5f * 250.f, 0.f, 0.f, 0.f));
		companion4.Adjust(Vector4::New( 3.f  * 250.f, 0.f, 0.f, 0.f));
		companions.Adjust(Vector4::New(0.f, 125.f, 0.f, 0.f));

		companion0.SetColour(ColourFactory::LightRed());
		companion1.SetColour(ColourFactory::LightGreen());
		companion2.SetColour(ColourFactory::LightBlue());
		companion3.SetColour(ColourFactory::LightYellow());
		companion4.SetColour(ColourFactory::LightPurple());

	/////////////

		Plane plane(ColourFactory::LightYellow());
	//	plane.RotateX((3.f * M_PI) / 2.f);
	//	plane.TranslateY(50.0f);
		plane.Scale(250.f);


		SolidCube compos;
		compos.Scale(250.f);
	/////////////

		Shape* sceneryShapesArray[2];
		ShapeComposition scenery(&sceneryShapesArray[0], sizeof(sceneryShapesArray));
		scenery.Add(&companions);
		scenery.Add(&plane);

		// Upload shape as textured, buffer 2
		{
			Shape& shape(
				scenery
			//	compos
				);
			_::ApplyCamera(shape);
			_::SetAttribute(vertexArrayId, buffer0Id, shape, POINTS_NORMALISED, true, numberOfTexturedSegments);
		}
	}

	static inline
	void SetUpShapes (
			GLuint const	line_vertexArrayId,
			GLuint const	line_buffer0Id,
			GLuint const	line_buffer1Id,
			GLuint&			numberOfPoints,
			//
			GLuint const	tria_vertexArrayId,
			GLuint const	tria_buffer0Id,
			GLuint const	tria_buffer1Id,
			GLuint&			numberOfWorldCubeLineSegments,
			//
			GLuint const	text_vertexArrayId,
			GLuint const	text_buffer0Id,
			GLuint const	text_buffer1Id,
			GLuint&			numberOfTexturedSegment)
	{
		SetUpLineShapes(line_vertexArrayId, line_buffer0Id, line_buffer1Id, numberOfPoints);
		SetUpTriangleObjects(tria_vertexArrayId, tria_buffer0Id, tria_buffer1Id, numberOfWorldCubeLineSegments);
		SetUpTexturedTriangleObjects(text_vertexArrayId, text_buffer0Id, text_buffer1Id, numberOfTexturedSegment);
	}

	static
	void InitialiseAnkh (void) {
	// initialise Images and Textures lib
		{	bool const success(ankh::images::Initialise());
			PASSERT(success) }
		{	bool const success(ankh::textures::Initialise());
			PASSERT(success) }

		ankh::images::InstallDefaultImageDecoders();
	}

	static
	void PlayWithTextureUnitsForTesting (void)
	{
		using namespace ankh;
		textures::TextureUnitManager& tum(textures::TextureUnitManager::GetSingleton());

		textures::TextureUnit& tu00(tum.Get(textures::TextureUnitIds::TEXTURE0 ));
		textures::TextureUnit& tu17(tum.Get(textures::TextureUnitIds::TEXTURE17));
		textures::TextureUnit& tu23(tum.Get(textures::TextureUnitIds::TEXTURE23));
		textures::TextureUnit& tu30(tum.Get(textures::TextureUnitIds::TEXTURE30));
		// Error
	//	textures::TextureUnit& tu37(tum.Get(textures::TextureUnitId(37)));

		tu17.Activate();
		DASSERT(!tu23.IsActive());

		tu30.Activate();
		DASSERT(!tu17.IsActive());

		tu23.Activate();
		DASSERT(!tu17.IsActive());
		DASSERT(!tu30.IsActive());

		tu23.Deactivate();
		DASSERT(!tu23.IsActive());

		tu17.Activate();
		DASSERT(!tu23.IsActive());

		tu30.Activate();
		DASSERT(!tu17.IsActive());

		tu23.Activate();
		DASSERT(!tu17.IsActive());
		DASSERT(!tu30.IsActive());
		// Error
	//	tu17.Deactivate();

		tu23.Deactivate();
		DASSERT(tu00.IsActive());
		// active-by-default texture ID can be deactivated as many times
		// as one wants -- it will still be active.
		tu00.Deactivate();
		tu00.Deactivate();
		tu00.Deactivate();
		DASSERT(tu00.IsActive());
	//	tu00.Activate();
		tu17.Activate();
		tu00.Activate();
		tu00.Deactivate();
	}

	static
	void InstallImageDecoders (glt::TGADecoder*& targa) {
		ankh::images::ImageLoader& il(ankh::images::ImageLoader::GetSingleton());

		il.InstallDecoder(targa = DNEW(glt::TGADecoder));
	}

	static
	void LoadTehStonets(ImagesArray& images)
	{
		using namespace ankh::images;

		ImageLoader&		il	(ImageLoader		::GetSingleton());

	//	images[0] = il.LoadFromPath("../textures/paccy.png");

		images[0] = il.LoadFromPaths("../textures/digitframes", 11, "png");	// gets loaded with Devil
	//	images[1] = il.LoadFromPath("../textures/taliatela.jpg");
	//	{
	//		FILE* const fp(ubinaryfileopen("../textures/CoolTexture.tga", "r"));
	//		PortableBinFileReader reader(fp);
	//		images[2] = il.LoadFromData("CoolTexture", "tga", reader);	// gets loaded with Targa
	//		fclose(fp);
	//	}
		size_t bufbytesize;
		void* const buf(uloadbinaryfile("../textures/paccy.png", &bufbytesize));
		images[1] = il.Load3DFromData(32, "pacco", "png", buf, bufbytesize);
	}

	static
	void CreateTextures(
			ImagesArray const&	images,
			TexturesArray&		textures,
			size_t&				previousTextureIndex)
	{
		using namespace ankh::textures;

		TextureUnitManager&	tum	(TextureUnitManager	::GetSingleton());
		TextureManager&		tm	(TextureManager		::GetSingleton());

		textures[0] = tm.New("Numberwng", images[0]);
		textures[1] = tm.New("Pacco", images[1]);

		textures[0]->BindTo(tum.Get(_::TexturesUnits[0]));
		textures[1]->BindTo(tum.Get(_::TexturesUnits[1]));
	//
	//
	//	textures[0] = (tm.New("../textures/stone.tga"));
	//	textures[1] = (tm.New("Taliatela", images[1]));
	//	textures[2] = (tm.New("Cool", images[2]));
	//
	//	TextureUnit& tu16(tum.Get(TextureUnitIds::TEXTURE16));
	//	TextureUnit& tu17(tum.Get(TextureUnitIds::TEXTURE17));
	//	TextureUnit& tu18(tum.Get(TextureUnitIds::TEXTURE18));
	//
	//	glUniform1i(my::OpenGL::VUL_SAMPLER1, tu16.GetIndex());
	//	glUniform1i(my::OpenGL::VUL_SAMPLER2, tu17.GetIndex());
	//	glUniform1i(my::OpenGL::VUL_SAMPLER3, tu18.GetIndex());

		previousTextureIndex = 0;
	}

	static
	void ConfigureOpenGl (void) {
		glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_CULL_FACE);
	//	glEnable(GL_TEXTURE_3D);
	}
}

namespace my {

	namespace drawing {


		// ----------------------------
		void draw (void* drawData, void (*bufferSwapper) (void*), void* bufferSwapperClosure); // stupid microsoft : proper declaration -- needed for linking with main
		void draw (void* const drawData, void (*const bufferSwapper) (void*), void* const bufferSwapperClosure) {
			_::DrawData& dd(*static_cast<_::DrawData* const>(drawData));

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			unsigned long int const _currtime(codeshare::utilities::GetATimestamp());
			PASSERT(_currtime >= dd.prevtime);
			unsigned long int const dt(_currtime - dd.prevtime);
			float const angle(_::GetRotationAngle(dt));
			float const cam(30.f);
			GLuint const texz(_::GetTextureZ(dt));

			if (texz != dd.previousTextureIndex) {
				glUniform1ui(OpenGL::VUL_TEXTUREZ, texz);
				dd.previousTextureIndex = texz;
				TCHAR msg[] = _T("Current time ist: 0\n");
				msg[sizeof(msg)/sizeof(msg[0]) - 3] = _T('0') + texz;
				my::global::log::info(&msg[0]);
			}

			// Draw lines
			{
				PASSERT(glIsVertexArray(dd.vertexArrayIds[1]) == GL_TRUE)
				glBindVertexArray(dd.vertexArrayIds[1]);
				glVertexAttrib4f(OpenGL::VAI_AXYC,
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, _::COLOUR_WITH_COLOUR);
				glDrawArrays(GL_LINES, 0, dd.numberOfPoints);
			}

			// Draw Triangles
		//	DONT
			{
				PASSERT(glIsVertexArray(dd.vertexArrayIds[2]) == GL_TRUE)
				glBindVertexArray(dd.vertexArrayIds[2]);
				glVertexAttrib4f(OpenGL::VAI_AXYC,
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, _::COLOUR_WITH_COLOUR);
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfWorldCubeLineSegments);
			}

			// and textured triangles too
			{
				PASSERT(glIsVertexArray(dd.vertexArrayIds[3]) == GL_TRUE)
				glBindVertexArray(dd.vertexArrayIds[3]);
				glVertexAttrib4f(OpenGL::VAI_AXYC,
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, _::COLOUR_WITH_TEXTURE);
				glUniform1i(OpenGL::VUL_SAMPLER0, _::GetTextureIndex(dt));
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfTexturedSegments);
			}


			(*bufferSwapper)(bufferSwapperClosure);
		}

		// ----------------------------
		void* setup (void) {
			using				_::VAOs;
			using				_::VBOs;
			using				_::VTOs;
			using				_::TEXTURES_NUM;
			using				_::IMAGES_NUM;

			_::DrawData* const	dd								(DNEW(_::DrawData));
			_::DrawData&		drawData						(*dd);
			GLuint				(&vertexArrayIds)[VAOs]			(drawData.vertexArrayIds);
			GLuint				(&bufferIds)[VBOs]				(drawData.bufferIds);
			GLuint&				numberOfPoints					(drawData.numberOfPoints);
			GLuint&				numberOfWorldCubeLineSegments	(drawData.numberOfWorldCubeLineSegments);
			unsigned long int&	startingTime					(drawData.startingTime);
			unsigned long int&	prevtime						(drawData.prevtime);
			GLuint&				sampler_location				(drawData.sampler_location);
			glt::TGADecoder*&	targa							= (drawData.targa); // stupid microsoft (and their inability to initialise references to pointers)
			_::TexturesArray&	textures						(drawData.textures);
			_::ImagesArray&		images							(drawData.images);
			GLuint&				numberOfTexturedSegments		(drawData.numberOfTexturedSegments);


			startingTime									= codeshare::utilities::GetATimestamp();
			prevtime										= startingTime;

			// Gen VAOs
			P_STATIC_ASSERT(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]) == 4)
			glGenVertexArrays(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]), &vertexArrayIds[0]);

			// Gen VBOs
			P_STATIC_ASSERT(sizeof(bufferIds)/sizeof(bufferIds[0]) == 6)
			glGenBuffers(sizeof(bufferIds)/sizeof(bufferIds[0]), &bufferIds[0]);


			///////////////////////////
			// VAO#1: Line objects
			// (buffers #3 #4)
			_::SetUpLineShapes(vertexArrayIds[1], bufferIds[3], bufferIds[4], numberOfPoints);
			///////////////////////////
			// VAO#2: Triangle objects
			// (buffer #5)
			_::SetUpTriangleObjects(vertexArrayIds[2], bufferIds[5], -1, numberOfWorldCubeLineSegments);
			///////////////////////////
			// VAO#3: Textured triangle objects
			// (buffer #2 )
			_::SetUpTexturedTriangleObjects(vertexArrayIds[3], bufferIds[2], -1, numberOfTexturedSegments);


			_::InitialiseAnkh();

			sampler_location = OpenGL::VUL_SAMPLER0;

			_::PlayWithTextureUnitsForTesting();
			_::InstallImageDecoders(targa);
			_::LoadTehStonets(images);
			_::CreateTextures(images, textures, drawData.previousTextureIndex);
			_::ConfigureOpenGl();

			return &drawData;
		}

		void cleanup (void*& _drawData) {
			{
				_::DrawData&	drawData				(*static_cast<_::DrawData*>(_drawData));
				GLuint			(&vertexArrayIds)[4]	(drawData.vertexArrayIds);
				GLuint			(&bufferIds)[6]			(drawData.bufferIds);

				P_STATIC_ASSERT(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]) == 4)
				P_STATIC_ASSERT(sizeof(bufferIds)/sizeof(bufferIds[0]) == 6)

				glDeleteBuffers(sizeof(bufferIds)/sizeof(bufferIds[0]), &bufferIds[0]);
				glDeleteVertexArrays(sizeof(bufferIds)/sizeof(bufferIds[0]), &vertexArrayIds[0]);

				udelete(drawData.targa);

				for (ankh::images::Image* const* i = &drawData.images[0]; i < &drawData.images[sizeof(drawData.images)/sizeof(drawData.images[0])]; ++i)
					ankh::images::ImageLoader::GetSingleton().Unload(*i);
				for (ankh::textures::Texture* const* i = &drawData.textures[0]; i < &drawData.textures[sizeof(drawData.textures)/sizeof(drawData.textures[0])]; ++i)
					ankh::textures::TextureManager::GetSingleton().Delete(*i);

				DDELETE(&drawData);
			}
			_drawData = NULL;

			ankh::textures::CleanUp();
			ankh::images::CleanUp();
		}

	} // namespace drawing

} // namespace my

