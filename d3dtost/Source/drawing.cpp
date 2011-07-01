#include "stdafx.h"


#define DONT	if (false)
#define DO		if (true)

#define __NE()  PASSERT(!my::openglutil::GlErrorsHandled(&_::errorHandler))

// #define __WITH_GL_IS_TEXTURE

#ifdef __WITH_GL_IS_TEXTURE
#	define __ASSERT_GL_IS_TEXTURE(TEX_ID)	PASSERT(glIsTexture((TEX_ID)) == GL_TRUE)
#else
#	define __ASSERT_GL_IS_TEXTURE(TEX_ID)
#endif

namespace _ {
	const static float WW(2000.f);

	const static size_t VAOs(3);
	const static size_t VBOs(6);
	const static size_t VTOs(1);

	struct DrawData {
		GLuint				vertexArrayIds[VAOs];
		GLuint				bufferIds[VBOs];
		GLuint				textureIds[VTOs];
		GLuint				numberOfPoints;
		GLuint				numberOfWorldCubeLineSegments;
		unsigned long int	startingTime;
		unsigned long int	prevtime;
		GLuint				sampler_location;
		ankh::images::ImageDecoder*		tgadecoder;
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
		GLboolean const					normalised)
	{
		using namespace	my::gl::shapes;
		using namespace	my::gl::math;
		using namespace	my::gl::extensions::ExtensionManager;
		using			my::OpenGL;

		glBindVertexArray(vao); __NE()
		PASSERT(glIsVertexArray(vao) == GL_TRUE)

		glBindBuffer(GL_ARRAY_BUFFER, vbo); __NE()
		PASSERT(glIsBuffer(vbo) == GL_TRUE)

		size_t const count		(shape.GetNumberOfVertices());
		size_t const bytesize	(std::max(count * sizeof(VertexData), 1u));
		void* const _data		(_::AllocateSingleAllocationBufferMemory(bytesize));

		VertexData* const data	(shape.GetVertexData(_data, bytesize));
		PASSERT(data != NULL)

		glBufferData(GL_ARRAY_BUFFER, bytesize, data, GL_STATIC_DRAW); __NE()

		_::DeallocateSingleAllocationBufferMemory(_data);

		glVertexAttribPointer(OpenGL::VAI_POSITION, 4, GL_FLOAT, normalised, VertexData::Stride(), VertexData::PositionOffsetPointer()); __NE()
		glVertexAttribPointer(OpenGL::VAI_COLOUR, 4, GL_FLOAT, normalised, VertexData::Stride(), VertexData::ColourOffsetPointer()); __NE()

		glEnableVertexAttribArray(OpenGL::VAI_POSITION); __NE()
		glEnableVertexAttribArray(OpenGL::VAI_COLOUR); __NE()
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
}

namespace my {

	namespace drawing {
		// ----------------------------
		using my::gl::extensions::ExtensionManager::glGenVertexArrays;
		using my::gl::extensions::ExtensionManager::glBindVertexArray;
		using my::gl::extensions::ExtensionManager::glDeleteVertexArrays;
		using my::gl::extensions::ExtensionManager::glGenBuffers;
		using my::gl::extensions::ExtensionManager::glDeleteBuffers;
		using my::gl::extensions::ExtensionManager::glBindBuffer;
		using my::gl::extensions::ExtensionManager::glBufferData;
		using my::gl::extensions::ExtensionManager::glVertexAttribPointer;
		using my::gl::extensions::ExtensionManager::glEnableVertexAttribArray;
		using my::gl::extensions::ExtensionManager::glDisableVertexAttribArray;
		using my::gl::extensions::ExtensionManager::glVertexAttrib1f;
		using my::gl::extensions::ExtensionManager::glVertexAttrib4f;
		using my::gl::extensions::ExtensionManager::glDepthRangef;
		using my::gl::extensions::ExtensionManager::glIsBuffer;
		using my::gl::extensions::ExtensionManager::glIsVertexArray;
		using my::gl::extensions::ExtensionManager::glIsTexture;
		using ::gl::ExtensionsManager::glUniform1i;


		// ----------------------------
		void draw (void* drawData, void (*bufferSwapper) (void*), void* bufferSwapperClosure); // proper declaration -- needed for linking with main
		void draw (void* const drawData, void (*const bufferSwapper) (void*), void* const bufferSwapperClosure) {
			_::DrawData& dd(*static_cast<_::DrawData* const>(drawData));
		
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
			unsigned long int const _currtime(codeshare::utilities::GetATimestamp());
			PASSERT(_currtime >= dd.prevtime);
			unsigned long int const dt(_currtime - dd.prevtime);
			float const angle(_::GetRotationAngle(dt));
			float const cam(30.f);
		
		
			// Draw lines
			{
				PASSERT(glIsVertexArray(dd.vertexArrayIds[1]) == GL_TRUE)
				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(OpenGL::VAI_AXYC,
						angle,
						-0.0f,
						0.0f,
						cam); __NE()
				glDrawArrays(GL_LINES, 0, dd.numberOfPoints); __NE()
			}
		
			// Draw Triangles
		//	DONT
			{
				PASSERT(glIsVertexArray(dd.vertexArrayIds[2]) == GL_TRUE)
				glBindVertexArray(dd.vertexArrayIds[2]); __NE()
				glVertexAttrib4f(OpenGL::VAI_AXYC,
						angle,
						-0.0f,
						0.0f,
						cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfWorldCubeLineSegments); __NE()
			}
		

			(*bufferSwapper)(bufferSwapperClosure);
		}

		static GLboolean const POINTS_NORMALISED(GL_TRUE);
		// ----------------------------
		void* setup (void) {
			using				_::VAOs;
			using				_::VBOs;
			using				_::VTOs;

			_::DrawData&		drawData						(*DNEW(_::DrawData));
			GLuint				(&vertexArrayIds)[VAOs]			(drawData.vertexArrayIds);
			GLuint				(&bufferIds)[VBOs]				(drawData.bufferIds);
			GLuint				(&textureIds)[VTOs]				(drawData.textureIds);
			GLuint&				numberOfPoints					(drawData.numberOfPoints);
			GLuint&				numberOfWorldCubeLineSegments	(drawData.numberOfWorldCubeLineSegments);
			unsigned long int&	startingTime					(drawData.startingTime);
			unsigned long int&	prevtime						(drawData.prevtime);
			GLuint&				sampler_location				(drawData.sampler_location);
			ankh::images::ImageDecoder*&	tgadecoder			= (drawData.tgadecoder); // stupid microsoft
			

			startingTime									= codeshare::utilities::GetATimestamp();
			prevtime										= startingTime;

			// Gen VAOs
			P_STATIC_ASSERT(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]) == 3)
			glGenVertexArrays(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]), &vertexArrayIds[0]); __NE()
			
			// Gen VBOs
			P_STATIC_ASSERT(sizeof(bufferIds)/sizeof(bufferIds[0]) == 6)
			glGenBuffers(sizeof(bufferIds)/sizeof(bufferIds[0]), &bufferIds[0]); __NE()

			// Gen textures
			glGenTextures(sizeof(textureIds)/sizeof(textureIds[0]), &textureIds[0]); __NE()
			


			// Shapes
			{
				using namespace my::gl::shapes;
				using namespace my::gl::math;

				///////////////////////////
				// VAO#1: Line objects
				// (buffers #3 #4)
				{
					
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
						_::ApplyCamera(shape);
						_::SetAttribute(vertexArrayIds[1], bufferIds[3], shape, POINTS_NORMALISED);
						numberOfPoints = shape.GetNumberOfVertices();
					}
				}
				///////////////////////////
				// VAO#2: Triangle objects
				// (buffer #5)
				{
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

					Plane plane(ColourFactory::LightWhite());
					plane.RotateX((3.f * M_PI) / 2.f);
					plane.TranslateY(-50.0f);
					plane.ScaleZ(125.f * 2);
					plane.ScaleX(125.f * 5);
					companions.Add(&plane);

					Nothing								nothing;
					{
						Shape& shape(
						//	nothing
							companions
						//	companion0
						//	plane
							);

					//	shape.Scale(_::WW / 10.f);
						_::ApplyCamera(shape);
						_::SetAttribute(vertexArrayIds[2], bufferIds[5], shape, POINTS_NORMALISED);
						numberOfWorldCubeLineSegments = shape.GetNumberOfVertices();
					}
				}
			}


			// Textures
			DONT {
				using namespace gl::textures;

				TextureUnitManager tum;

				TextureUnit& tu(*tum.GetUnit(GL_TEXTURE5));
				tu.Activate();

				codeshare::utilities::Placeholder<TextureObject> toph;
				TextureObjectManager::Create(toph.GetInternal(), toph.SizeOfValue());
				TextureObject& to(*toph.GetInternal());


			}

			{ // initialise Images and Textures lib
				{	bool const success(ankh::textures::Initialise());
					PASSERT(success) }
				{	bool const success(ankh::images::Initialise());
					PASSERT(success) }
			}

			// get teh stonet sampler location
			sampler_location = OpenGL::VUL_SAMPLER4;
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


				glUniform1i(sampler_location, tum.GetActiveUnitIndex());
			}

			// Install image decoders
			{
				ankh::images::ImageLoader& il(ankh::images::ImageLoader::GetSingleton());

				il.InstallDecoder(tgadecoder = DNEW(glt::TGADecoder));
			}

			// Load teh stonet cooly
			{
				ankh::textures::TextureManager& tm(ankh::textures::TextureManager::GetSingleton());

				ankh::textures::Texture* const stone(tm.New("../textures/stone.tga"));
			}

			// Load teh stonet
			GLbyte* textureData(NULL); DONT {
				using namespace gl::textures;

				__ASSERT_GL_IS_TEXTURE(textureIds[0])
				glBindTexture(GL_TEXTURE_2D, textureIds[0]); __NE()

				GLint width(0), height(0), internalFormat(0);
				GLenum format(0);
				GLshort depth(0);
				GLuint imageSize(0);

				textureData = (glt::ReadTGABits(
						"../textures/stone.tga",
						&_::AllocateSingleAllocationBufferMemory, &_::DeallocateSingleAllocationBufferMemory,
						&format, &width, &height,
						&depth, &imageSize,
						&internalFormat
						));

				openglutil::GlConst _format(openglutil::ToGlConst(format));
				openglutil::GlConst _components(openglutil::ToGlConst(internalFormat));


				if (textureData) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
							GL_CLAMP_TO_EDGE
						//	GL_CLAMP_TO_BORDER
						//	GL_MIRRORED_REPEAT
						//	GL_REPEAT
							); __NE()
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
							GL_CLAMP_TO_EDGE
						//	GL_CLAMP_TO_BORDER
						//	GL_MIRRORED_REPEAT
						//	GL_REPEAT
							); __NE()
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						//	GL_LINEAR
							GL_NEAREST
							); __NE()
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
						//	GL_LINEAR
							GL_NEAREST
							); __NE()

					glPixelStorei(GL_UNPACK_ALIGNMENT, 1); __NE()
					glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); __NE()
					glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); __NE()
					glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0); __NE()

					// Upload texture to texture unit
					glTexImage2D(
							GL_TEXTURE_2D,
							0,					// mipmap level
							internalFormat,		// internal format
							width,
							height,
							0,					// border -- must be 0
							format,				// data format
							GL_UNSIGNED_BYTE,	// data type
							textureData); __NE()

					_::DeallocateSingleAllocationBufferMemory(textureData);
					{ void* n = _::AllocateSingleAllocationBufferMemory(12); _::DeallocateSingleAllocationBufferMemory(n); }
				}
				else
					global::logger::Get().Warning(_T("Could not load texture Stone"));
			}

			
			glEnable(GL_DEPTH_TEST); __NE()
			glEnable(GL_CULL_FACE); __NE()

			return &drawData;
		}

		void cleanup (void*& _drawData) {
			{
				_::DrawData&	drawData				(*static_cast<_::DrawData*>(_drawData));
				GLuint			(&vertexArrayIds)[3]	(drawData.vertexArrayIds);
				GLuint			(&bufferIds)[6]			(drawData.bufferIds);
				GLuint			(&textureIds)[1]		(drawData.textureIds);

				P_STATIC_ASSERT(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]) == 3)
				P_STATIC_ASSERT(sizeof(bufferIds)/sizeof(bufferIds[0]) == 6)

				glDeleteBuffers(sizeof(bufferIds)/sizeof(bufferIds[0]), &bufferIds[0]); __NE()
				glDeleteVertexArrays(sizeof(bufferIds)/sizeof(bufferIds[0]), &vertexArrayIds[0]); __NE()

				glDeleteTextures(sizeof(textureIds)/sizeof(textureIds[0]), &textureIds[0]); __NE()

				udelete(drawData.tgadecoder);

				DDELETE(&drawData);
			}
			_drawData = NULL;

			ankh::textures::CleanUp();
		}

	} // namespace drawing

} // namespace my

