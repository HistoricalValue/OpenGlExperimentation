#include "stdafx.h"

#include "TheCursed.h"

#include <drawing.h>
#include <drawing_nurbs.h>
#include <drawing_utils.h>
#include <drawing_globalstate.h>
#include <drawing_buffers.h>
#include <drawing_setup_shapes.h>
#include <drawing_setup_geometrical.h>
#include <drawing_setup_ankh.h>
#include <drawing_setup_images.h>
#include <drawing_setup_textures.h>
#include <drawing_setup_opengl_adapters.h>
#include <drawing_setup_framebuffers.h>
#include <NurbsFacade.h>
#include <MyUtils.h>

using namespace gl::ext;
using namespace gl::adapt;
using my::gl::adapters::Buffer;
using my::gl::adapters::BufferManager;

namespace _ {

	using namespace my::drawing;

	///////////////////////////////////////////////////////

	static
	void ConfigureOpenGl (void) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	//	glEnable(GL_TEXTURE_3D);

		glDisable(GL_PROGRAM_POINT_SIZE);
		glPointSize(5.f);
	}

	template <typename SizeType> static
	void DrawGrid (SizeType const& numberOfLineSegments) {
		PASSERT(WITH_GRID == 1)

		glUniform1ui(my::OpenGL::VUL_COLSELTR, Options::TriangleShapeGridColouringMethod());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glLineWidth(0.2f);
		glPolygonOffset(-1, -1);
		glDrawArrays(GL_TRIANGLES, 0, psafecast<GLsizei>(numberOfLineSegments));
		glDisable(GL_POLYGON_OFFSET_LINE);
	//	glDisable(GL_LINE_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

namespace my {

	namespace drawing {


		// ----------------------------
		void draw (void* drawData, void (*bufferSwapper) (void*), void* bufferSwapperClosure); // stupid microsoft : proper declaration -- needed for linking with main
		void draw (void* const drawData, void (*const bufferSwapper) (void*), void* const bufferSwapperClosure) {
			_::DrawData& dd(*static_cast<_::DrawData* const>(drawData));

		//	dd.framebuffer->BindForDrawing();

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
				msg[sizeof(msg)/sizeof(msg[0]) - 3] = psafecast<TCHAR>(_T('0') + texz);
				my::global::log::info(&msg[0]);
			}

			// Set directional light direction
			glUniform4f(OpenGL::VUL_POOPLIGHT, _::POOP_LIGHT[0], _::POOP_LIGHT[1], _::POOP_LIGHT[2], _::POOP_LIGHT[3]);

			// Draw points
			if (_::WITH_DRAW_POINTS) {
				PASSERT(::gl::prim::VertexArray::Is(dd.vertexArrayIds[0]->GetId()))
				dd.vertexArrayIds[0]->Bind();
				glVertexAttrib4f(psafecast<GLuint>(OpenGL::VAI_AXYC.operator GLint()),
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, Options::PointShapeColouringMethod());
				glDrawArrays(GL_POINTS, 0, psafecast<GLsizei>(dd.numberOfPointPoints));
			}

			// Draw lines
			if (_::WITH_DRAW_LINES) {
				PASSERT(::gl::prim::VertexArray::Is(dd.vertexArrayIds[1]->GetId()))
				dd.vertexArrayIds[1]->Bind();
				glVertexAttrib4f(psafecast<GLuint>(OpenGL::VAI_AXYC.operator GLint()),
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, Options::LineShapeColouringMethod());
				glDrawArrays(GL_LINES, 0, psafecast<GLsizei>(dd.numberOfPoints));
			}

			// Draw Triangles
		//	DONT
			if (_::WITH_DRAW_TRIANGLES) {
				PASSERT(::gl::prim::VertexArray::Is(dd.vertexArrayIds[2]->GetId()))
				dd.vertexArrayIds[2]->Bind();
				glVertexAttrib4f(psafecast<GLuint>(OpenGL::VAI_AXYC.operator GLint()),
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, Options::TriangleShapeColouringMethod());
				glDrawArrays(GL_TRIANGLES, 0, psafecast<GLsizei>(dd.numberOfWorldCubeLineSegments));
				#if WITH_GRID == 1
				_::DrawGrid(dd.numberOfWorldCubeLineSegments);
				#endif
			}

			// and textured triangles too
			if (_::WITH_DRAW_TEXTURED) {
				PASSERT(::gl::prim::VertexArray::Is(dd.vertexArrayIds[3]->GetId()))
				dd.vertexArrayIds[3]->Bind();
				glVertexAttrib4f(psafecast<GLuint>(OpenGL::VAI_AXYC.operator GLint()),
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, Options::TexturedTriangleShapeColouringMethod());
				glUniform1i(OpenGL::VUL_SAMPLER0, psafecast<GLint>(_::GetTextureIndex(dt)));

				glDrawArrays(GL_TRIANGLES, 0, psafecast<GLsizei>(dd.numberOfTexturedSegments));
				#if WITH_GRID == 1
				_::DrawGrid(dd.numberOfTexturedSegments);
				#endif
			}

			(*bufferSwapper)(bufferSwapperClosure);
		}

		// ----------------------------
		void* setup (void) {
			_::DrawData& dd(*DNEW(_::DrawData));
			uzeromemory(&dd);

			dd.prevtime = dd.startingTime = codeshare::utilities::GetATimestamp();

			_::SetupOpenGlAdapters();

			// Gen VAOs
			P_STATIC_ASSERT(sizeof(dd.vertexArrayIds)/sizeof(dd.vertexArrayIds[0]) == _::NUMBER_OF_VAOs)
			VertexArrayManager::GetSingleton().Create(dd.vertexArrayIds);

			// Gen VBOs
			P_STATIC_ASSERT(sizeof(dd.buffers)/sizeof(dd.buffers[0]) == _::NUMBER_OF_VBOs)
			_::GenBuffers(dd.buffers);

			if (_::WITH_DRAW_TRIANGLES || _::WITH_DRAW_TEXTURED) {
				nurbs::Initialise();
				_::setmesh(dd, nurbs::load(ucstringarg(_::NURBS_LOAD_FROM)));
			}

			///////////////////////////
			// VAO#0: Points
			// (buffers #1)
			_::SetupPointShapes(dd, dd.vertexArrayIds[0], dd.buffers[1], NULL, dd.numberOfPointPoints);
			///////////////////////////
			// VAO#1: Line objects
			// (buffers #3 #4)
			_::SetUpLineShapes(dd, dd.vertexArrayIds[1], dd.buffers[3], dd.buffers[4], dd.numberOfPoints);
			///////////////////////////
			// VAO#2: Triangle objects
			// (buffer #5)
			_::SetUpTriangleObjects(dd, dd.vertexArrayIds[2], dd.buffers[5], NULL, dd.numberOfWorldCubeLineSegments);
			///////////////////////////
			// VAO#3: Textured triangle objects
			// (buffer #2 )
			_::SetUpTexturedTriangleObjects(dd, dd.vertexArrayIds[3], dd.buffers[2], NULL, dd.numberOfTexturedSegments);


			_::InitialiseAnkh();

			psafecast(dd.sampler_location, OpenGL::VUL_SAMPLER0.operator GLint());

			_::PlayWithTextureUnitsForTesting();
			_::LoadTehStonets(dd.images);
			_::CreateTextures(dd.images, dd.textures, dd.previousTextureIndex);
			_::SetupCamera();
			{
				float const wh = 0.80f;
				_::SetupFrustrum(1.00f, 5.00f, -wh, wh, -wh, wh);
			}
			_::ConfigureOpenGl();

			_::SetupFramebuffers(dd);

			return &dd;
		}

		void cleanup (void*& _drawData) {
			{
				_::DrawData& dd(*static_cast<_::DrawData*>(_drawData));

				_::CleanUpFrameBuffers(dd);

				_::DestroyTextures(dd.textures);
				_::UnloadTehStonets(dd.images);

				_::CleanUpAnkh();

				if (_::WITH_DRAW_TRIANGLES || _::WITH_DRAW_TEXTURED) {
					nurbs::unload(&_::getmesh(dd));
					nurbs::CleanUp();
				}

				_::DeleteBuffers(dd.buffers);
				VertexArrayManager::GetSingleton().Release(dd.vertexArrayIds);

				_::CleanUpOpenGlAdapters();

				DDELETE(&dd);
			}

			unullify(_drawData);
		}

	} // namespace drawing

} // namespace my

