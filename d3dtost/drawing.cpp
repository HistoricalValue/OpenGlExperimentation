#include "stdafx.h"


#define __NE()  PASSERT(!my::openglutil::GlErrorsHandled(&_::errorHandler))


namespace _ {
	struct DrawData {
		GLuint				vertexArrayIds[2];
		GLuint				bufferIds[5];
		GLuint				textureIds[1];
		GLuint				numberOfPoints;
		unsigned long int	startingTime;
		unsigned long int	prevtime;
	};

	typedef float	vec4[4];

	struct VertexData {
		vec4		position;
		vec4		colour;
	};

	static void errorHandler (LPCTSTR const message) {
		PASSERT(false)
	}

	static void const* ptroff (void const* base, void const* off) {
		PASSERT(base <= off)
		return reinterpret_cast<void const* const>(
				reinterpret_cast<unsigned int>(off) -
				reinterpret_cast<unsigned int>(base)
			);
	}

	static float GetRotationAngle (unsigned long int const startingTime, unsigned long int const currtime) {
		// times in milliseconds
		// ----
		// we want one round per second => w = rad/sec = 2pi/sec => ang = w*sec = 2pi*sec = 360*sec (%360)
		// 360 * dt * 1e-3  = 0.360 * dt
		return .045f * (currtime - startingTime);
	}

	static void* __last_static_buffer_allocation(NULL);
	static size_t __last_static_buffer_allocation_size(0);
	static void* AllocateStaticBufferMemory (size_t const size) {
		P_STATIC_ASSERT(sizeof(char) == 1)

		void* const result(codeshare::StaticBuffer::AsArrayOf<char>(size));

		P_DEBUG_STMT(__last_static_buffer_allocation = result;)
		P_DEBUG_STMT(__last_static_buffer_allocation_size = size;)

		return result;
	}

	static void DeallocateStaticBufferMemory (void* ptr) {
		PASSERT(ptr == __last_static_buffer_allocation)
	}

	static const float FLOAT_EQUALITY_MARGIN(1e-6f);
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
}

namespace my {

	namespace drawing {
		// ----------------------------
		// Proper declarations
		extern void draw (void* drawData, void (*bufferSwapper) (void*), void* bufferSwapperClosure);

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
//		using my::gl::extensions::ExtensionManager::glDepthRangef;


		// ----------------------------
		void draw (void* const drawData, void (*const bufferSwapper) (void*), void* const bufferSwapperClosure) {
			_::DrawData& dd(*static_cast<_::DrawData* const>(drawData));

//			unsigned long int const	currtime(codeshare::GetATimestamp());
//			PASSERT(currtime > dd.prevtime)
//			unsigned long int dt(currtime - dd.prevtime);
//			if (dt > 600 || true) {
//				dd.prevtime = currtime;

				//glDisable(GL_DEPTH_TEST); __NE()
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


				unsigned long int const currtime(codeshare::GetATimestamp());
				float const angle(_::GetRotationAngle(dd.startingTime, currtime));
				float const cam(-30.0f);

//#define VARIATIONS
#ifdef VARIATIONS


				glEnable(GL_DEPTH_TEST); __NE()
				glEnable(GL_CULL_FACE); __NE()

				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.6f,
							0.0f,
							cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.6f,
							-0.6f,
							cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.6f,
							0.6f,
							cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()
//				glDisable(GL_DEPTH_TEST); __NE()
//				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()
//				glEnable(GL_DEPTH_TEST); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.0f,
							0.0f,
							-cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.0f,
							-0.6f,
							-cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.0f,
							0.6f,
							-cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()




				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.6f,
							0.0f,
							-0.f); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.6f,
							-0.6f,
							-0.f); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.6f,
							0.6f,
							-0.f); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()


				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////


				glDisable(GL_DEPTH_TEST); __NE()

				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.8f,
							0.0f,
							cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.8f,
							-0.6f,
							cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.8f,
							0.6f,
							cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()
//				glDisable(GL_DEPTH_TEST); __NE()
//				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()
//				glEnable(GL_DEPTH_TEST); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.2f,
							0.0f,
							-cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.2f,
							-0.6f,
							-cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.2f,
							0.6f,
							-cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()




				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.4f,
							0.0f,
							-0.f); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.4f,
							-0.6f,
							-0.f); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.4f,
							0.6f,
							-0.f); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////


				glDisable(GL_CULL_FACE); __NE()


				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.4f,
							0.0f,
							cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.4f,
							-0.6f,
							cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							-0.4f,
							0.6f,
							cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()
//				glDisable(GL_DEPTH_TEST); __NE()
//				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()
//				glEnable(GL_DEPTH_TEST); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.2f,
							0.0f,
							-cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.2f,
							-0.6f,
							-cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.2f,
							0.6f,
							-cam); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()




				//////////////////////////////////
				//////////////////////////////////
				//////////////////////////////////



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.8f,
							0.0f,
							-0.f); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[1]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.8f,
							-0.6f,
							-0.f); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()



				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
							_::GetRotationAngle(dd.startingTime, currtime),
							0.8f,
							0.6f,
							-0.f); __NE()
				glDrawArrays(GL_LINE_LOOP, 0, dd.numberOfPoints); __NE()

#else

				glBindVertexArray(dd.vertexArrayIds[0]); __NE()
				glVertexAttrib4f(2,
						_::GetRotationAngle(dd.startingTime, currtime),
						-0.0f,
						0.0f,
						cam); __NE()
				glDrawArrays(GL_TRIANGLES, 0, dd.numberOfPoints); __NE()

#endif

				(*bufferSwapper)(bufferSwapperClosure);
//			}
		}


//#define SILLY
#define MODEL_CUBE
#ifdef SILLY
#	define GROUP1
#	define GROUP2
#elif defined(MODEL_CUBE)
#	define GROUP4
#else
#	define GROUP3
#endif

#define NORMALISING_FACTOR 100.f

#define COORDS(XYZ) 0.##XYZ##f, 0.##XYZ##f, 0.##XYZ##f
#define POINT(X,Y)	(X##.f/100.f), (Y##.f/100.f), 0.0f, 1.0f
#define P3(X,Y,Z)	(X##.f/NORMALISING_FACTOR), (Y##.f/NORMALISING_FACTOR), (Z##.f/NORMALISING_FACTOR), 1.0f
#undef RGB
#define RGB(R,G,B)	0.##R##f, 0.##G##f, 0.##B##f, 1.0f
#define COLOR(R, G, B) R, G, B, 1.0f, R, G, B, 1.0f, R, G, B, 1.0f, R, G, B, 1.0f, R, G, B, 1.0f
#define POSCOL(X,Y,Z,COL) { { P3(X,Y,Z) } , { COL } }

		// ----------------------------
		void* setup (void) {
			_::DrawData&		drawData					(*new _::DrawData);
			GLuint				(&vertexArrayIds)[2]		(drawData.vertexArrayIds);
			GLuint				(&bufferIds)[5]				(drawData.bufferIds);
			GLuint				(&textureIds)[1]			(drawData.textureIds);
			GLuint&				numberOfPoints				(drawData.numberOfPoints);
			unsigned long int&	startingTime				(drawData.startingTime);
			unsigned long int&	prevtime					(drawData.prevtime);

			startingTime									= codeshare::GetATimestamp();
			prevtime										= startingTime;

			_::VertexData	data							[] =	{
#ifdef GROUP1
															{ { POINT(-10,  00) }, RGB(8, 0, 0) },
															{ { POINT( 00,  10) }, RGB(0, 8, 0) },
															{ { POINT( 10,  00) }, RGB(0, 0, 8) },
															{ { POINT( 05, -10) }, RGB(0, 8, 8) },
															{ { POINT(-05, -10) }, RGB(8, 8, 0) },
															{ { POINT(-10,  00) }, RGB(8, 0, 8) },
															//
															{ { P3(-10,  00, 02) }, RGB(8, 0, 0) },
															{ { P3( 00,  10, 02) }, RGB(0, 8, 0) },
															{ { P3( 10,  00, 02) }, RGB(0, 0, 8) },
															{ { P3( 05, -10, 02) }, RGB(0, 8, 8) },
															{ { P3(-05, -10, 02) }, RGB(8, 8, 0) },
															{ { P3(-10,  00, 02) }, RGB(8, 0, 8) },
															//
															{ { P3(-10,  00, 04) }, RGB(8, 0, 0) },
															{ { P3( 00,  10, 04) }, RGB(0, 8, 0) },
															{ { P3( 10,  00, 04) }, RGB(0, 0, 8) },
															{ { P3( 05, -10, 04) }, RGB(0, 8, 8) },
															{ { P3(-05, -10, 04) }, RGB(8, 8, 0) },
															{ { P3(-10,  00, 04) }, RGB(8, 0, 8) },
															//
															{ { P3(-10,  00, 06) }, RGB(8, 0, 0) },
															{ { P3( 00,  10, 06) }, RGB(0, 8, 0) },
															{ { P3( 10,  00, 06) }, RGB(0, 0, 8) },
															{ { P3( 05, -10, 06) }, RGB(0, 8, 8) },
															{ { P3(-05, -10, 06) }, RGB(8, 8, 0) },
															{ { P3(-10,  00, 06) }, RGB(8, 0, 8) },
															//
															{ { P3(-10,  00, 08) }, RGB(8, 0, 0) },
															{ { P3( 00,  10, 08) }, RGB(0, 8, 0) },
															{ { P3( 10,  00, 08) }, RGB(0, 0, 8) },
															{ { P3( 05, -10, 08) }, RGB(0, 8, 8) },
															{ { P3(-05, -10, 08) }, RGB(8, 8, 0) },
															{ { P3(-10,  00, 08) }, RGB(8, 0, 8) },
															//
															{ { P3(-10,  00, 10) }, RGB(8, 0, 0) },
															{ { P3( 00,  10, 10) }, RGB(0, 8, 0) },
															{ { P3( 10,  00, 10) }, RGB(0, 0, 8) },
															{ { P3( 05, -10, 10) }, RGB(0, 8, 8) },
															{ { P3(-05, -10, 10) }, RGB(8, 8, 0) },
															{ { P3(-10,  00, 10) }, RGB(8, 0, 8) },
#endif
															//
#ifdef GROUP2
															{ { P3(00,00,  00) }, { RGB(8, 8, 8) } },
															{ { P3(10,00,  00) }, { RGB(8, 8, 8) } },
															{ { P3(10,00,  10) }, { RGB(8, 8, 8) } },
															{ { P3(20,00,  10) }, { RGB(8, 8, 8) } },
															{ { P3(20,00,  20) }, { RGB(8, 8, 8) } },
															{ { P3(10,00,  20) }, { RGB(8, 8, 8) } },
															{ { P3(10,00,  10) }, { RGB(8, 8, 8) } },
															{ { P3(00,00,  10) }, { RGB(8, 8, 8) } },
															{ { P3(00,00,  00) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00, 00) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00,-10) }, { RGB(8, 8, 8) } },
															{ { P3(-20,00,-10) }, { RGB(8, 8, 8) } },
															{ { P3(-20,00,-20) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00,-20) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00,-10) }, { RGB(8, 8, 8) } },
															{ { P3( 00,00,-10) }, { RGB(8, 8, 8) } },
															{ { P3( 00,00, 00) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00, 00) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00, 10) }, { RGB(8, 8, 8) } },
															{ { P3(-20,00, 10) }, { RGB(8, 8, 8) } },
															{ { P3(-20,00, 20) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00, 20) }, { RGB(8, 8, 8) } },
															{ { P3(-10,00, 10) }, { RGB(8, 8, 8) } },
															{ { P3( 00,00, 10) }, { RGB(8, 8, 8) } },
															{ { P3( 00,00, 00) }, { RGB(8, 8, 8) } },
#endif
															//
#ifdef GROUP3
				{ { P3(-80, 40,  0) }, { RGB(8,6,2) } },
				{ { P3(-80,-80,  0) }, { RGB(8,6,0) } },
				{ { P3( 20,-80,  0) }, { RGB(8,4,0) } },

				{ { P3( 20,-70,  0) }, { RGB(2,6,8) } },
				{ { P3( 20, 50,  0) }, { RGB(0,6,8) } },
				{ { P3(-80, 50,  0) }, { RGB(0,4,8) } },

				{ { P3(-70,-70, 80) }, { RGB(6,8,2) } },
				{ { P3( 15,-70, 80) }, { RGB(0,8,4) } },
				{ { P3(-70, 45, 80) }, { RGB(0,8,6) } },
#endif


#undef WHITE
#define WHITE		RGB(9,9,9)
#define COL1_WHITE
#ifdef COL1_WHITE
#	define	COL1	WHITE
#	define	COL2	WHITE
#	define	COL3	WHITE
#else
#	define COL1	RGB(4,8,2)
#	define COL2	RGB(7,8,0)
#	define COL3	RGB(6,8,0)
#endif

#define COL4	RGB(4,2,8)
#define COL5	RGB(7,0,8)
#define COL6	RGB(6,0,8)

#define COL7	RGB(8,6,4)
#define COL8	RGB(8,2,0)
#define COL9	RGB(8,4,2)

#define COL10	RGB(0,2,8)
#define COL11	RGB(0,0,8)
#define COL12	RGB(6,6,8)

#define COL13	RGB(6,8,8)
#define COL14	RGB(0,8,8)
#define COL15	RGB(4,8,8)

#ifdef GROUP45
				POSCOL(-10, 0, -80, COL1),
				POSCOL(0, 0, 80, COL5),
				POSCOL(10, 0, -80, COL3),

				POSCOL(-20, -20, 0, COL7),
				POSCOL(0, 20, 0, COL8),
				POSCOL(20, -20, 0,  COL9)

#elif defined(GROUP4)
				// Floor
				POSCOL(	-80,	 0,	 60,		COL1),
				POSCOL(	-80,	 0,	-60,		COL2),
				POSCOL(	 80,	 0,	-60,		COL3),
				//
				POSCOL(	 80,	 0,	-59,		COL1),
				POSCOL(	 80,	 0,	 60,		COL2),
				POSCOL(	-79,	 0,  60,		COL3),

				// Up
				POSCOL(	-40,	80,	 40,		COL4),
				POSCOL(	-40,	80,	-40,		COL5),
				POSCOL(	 40,	80,	-40,		COL6),
				//
				POSCOL(	 40,	80,	-39,		COL4),
				POSCOL(	 40,	80,	 40,		COL5),
				POSCOL(	-39,	80,	 40,		COL6),

				// Right
				POSCOL(	 40,	 0,	-40,		COL8),
				POSCOL(	 40,	 0,	 40,		COL7),
				POSCOL(	 40,	80,	-40,		COL9),
				//
				POSCOL(	 40,	80,	-39,		COL7),
				POSCOL(	 40,	 0,	 40,		COL9),
				POSCOL(	 40,	80,	 40,		COL8),

				// Left
				POSCOL(	-40,	 0,	-40,		COL11),
				POSCOL(	-40,	80,	-40,		COL12),
				POSCOL(	-40,	 0,	 40,		COL10),
				//
				POSCOL(	-40,	80,	-39,		COL11),
				POSCOL(	-40,	80,	 40,		COL12),
				POSCOL(	-40,	 0,	 40,		COL10),

				// Back
				POSCOL(	 40,	 0,	 40,		COL13),
				POSCOL(	-40,	 0,	 40,		COL14),
				POSCOL(	-40,	80,	 40,		COL15),

				POSCOL(	-39,	80,	 40,		COL13),
				POSCOL(	 40,	80,	 40,		COL14),
				POSCOL(	 40,	 0,	 40,		COL15)

#endif

														};
			const size_t _numberOfPoints				(sizeof(data)/sizeof(data[0]));
			numberOfPoints								= _numberOfPoints;

			_::VertexData	__dummy;
			GLvoid const*	positionOffset		(_::ptroff(&__dummy, &__dummy.position));
			GLvoid const*	colorOffset			(_::ptroff(&__dummy, &__dummy.colour));
			GLsizei const	stride				(sizeof(_::VertexData));

			// Gen VAOs
			glGenVertexArrays(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]), &vertexArrayIds[0]); __NE()

			// Gen VBOs
			glGenBuffers(sizeof(bufferIds)/sizeof(bufferIds[0]), &bufferIds[0]); __NE()

			// create position-only and color-only data
			_::VertexData	vertexPositions[_numberOfPoints];
			{ for (size_t i(0); i < _numberOfPoints; ++i)
				memcpy_s(&vertexPositions[i].position[0], sizeof(vertexPositions[i].position), &data[i].position[0], sizeof(data[i].position)); }
			_::VertexData	vertexColors[_numberOfPoints];
			{ for (size_t i(0); i < _numberOfPoints; ++i)
				memcpy_s(&vertexColors[i].colour[0], sizeof(vertexColors[i].colour), &data[i].colour[0], sizeof(data[i].colour)); }

			// upload data
			// buffer#0: positions for shape#0
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[0]); __NE()
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions[0], GL_STATIC_DRAW); __NE()
			// buffer#1: colors for shape#0
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]); __NE()
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), &vertexColors[0], GL_STATIC_DRAW); __NE()
			// buffer#2: positions for shape#1
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[2]); __NE()
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions[0], GL_STATIC_DRAW); __NE()
			// buffer#3: colors for shape#1
			// change colours for second color buffer
			_::VertexData	vertexColorsGrey[_numberOfPoints];
			_::vec4 grey = { 0.6f, 0.6f, 0.6f, 1.0f };
			P_STATIC_ASSERT(sizeof(grey)/sizeof(grey[0]) == 4)
			for (size_t i(0); i < _numberOfPoints; ++i) {
				PASSERT( (&(vertexColorsGrey[i].colour[0])) == (&vertexColorsGrey[i].colour[0]) )
				PASSERT( sizeof(vertexColorsGrey[i].colour) == 4 * sizeof(float) )
				P_STATIC_ASSERT(sizeof(grey)/sizeof(grey[0]) == 4)
				P_STATIC_ASSERT(sizeof(grey) == sizeof(float) * 4)
				memcpy_s(&(vertexColorsGrey[i].colour[0]), sizeof(vertexColorsGrey[i].colour), &grey[0], sizeof(grey));
				PASSERT( vertexColorsGrey[i].colour[0] == grey[0] )
				PASSERT( vertexColorsGrey[i].colour[1] == grey[1] )
				PASSERT( vertexColorsGrey[i].colour[2] == grey[2] )
				PASSERT( vertexColorsGrey[i].colour[3] == grey[3] )
			}
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[3]); __NE()
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColorsGrey), &vertexColorsGrey[0], GL_STATIC_DRAW); __NE()


			//////////////////////////
			// VAO#0: shape/colors #0
			glBindVertexArray(vertexArrayIds[0]); __NE()
			// position data from buffer#0
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[0]); __NE()
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, stride, positionOffset); __NE()
			// colour data from buffer#1
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]); __NE()
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, stride, colorOffset); __NE()


			glEnableVertexAttribArray(0); __NE()
			glEnableVertexAttribArray(1); __NE()


			/////////////////////////
			// VAO#1: shape/colors #1
			glBindVertexArray(vertexArrayIds[1]); __NE()
			// position data from buffer#2
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[2]); __NE()
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, stride, positionOffset); __NE()
			// colour data from buffer#3
			glBindBuffer(GL_ARRAY_BUFFER, bufferIds[3]); __NE()
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, stride, colorOffset); __NE()


			glEnableVertexAttribArray(0); __NE()
			glEnableVertexAttribArray(1); __NE()



			glDepthFunc(
//				GL_NEVER
				GL_LESS
//				GL_EQUAL
//				GL_LEQUAL
//				GL_GREATER
///				GL_NOTEQUAL
//				GL_GEQUAL
//				GL_ALWAYS
				);
			glEnable(GL_DEPTH_TEST); __NE()
//			glEnable(GL_CULL_FACE); __NE()
			glDisable(GL_CULL_FACE); __NE()



			///////////////////////////
			// Textures
			//void glGenTextures(	GLsizei  	n, GLuint *  	textures);
			//void glBindTexture(	GLenum  	target, GLuint  	texture)
			//void glDeleteTextures(	GLsizei  	n, const GLuint *  	textures)
			//
			//glPixelStorei(GL_PACK_ALIGNMENT, 1);
			//glPixelStorei(GL_PACK_ROW_LENGTH, 0);
			//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
			//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

			// Load teh stonet
			GLbyte* textureData(NULL); {

				P_STATIC_ASSERT(sizeof(textureIds) == sizeof(textureIds[0]))
				glGenTextures(sizeof(textureIds)/sizeof(textureIds[0]), &textureIds[0]); __NE()
				glBindTexture(GL_TEXTURE_2D, textureIds[0]); __NE()

				GLint width(0), height(0), components(0);
				GLenum format(0);
				GLshort depth(0);
				GLuint imageSize(0);

				textureData = (glt::ReadTGABits(
						"../textures/stone.tga",
						&_::AllocateStaticBufferMemory, &_::DeallocateStaticBufferMemory,
						&format, &width, &height,
						&depth, &imageSize,
						&components
						));

				openglutil::GlConst _format(static_cast<openglutil::GlConst>(format));
				openglutil::GlConst _components(static_cast<openglutil::GlConst>(components));


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

					glTexImage2D(
							GL_TEXTURE_2D,
							0,
							components,
							width,
							height,
							0,
							format,
							GL_UNSIGNED_BYTE,
							textureData); __NE()



					// Create a buffer with texture coordinates
					float	textureCoordinates[4 * _numberOfPoints];
					memset(&textureCoordinates[0], 0x00, sizeof(textureCoordinates)/sizeof(textureCoordinates[0]));
					{	float*			tc		(&textureCoordinates[0]);
						_::VertexData*	vdp		(&vertexPositions[0]);
						for (size_t i(0); i < _numberOfPoints; ++i) {
							PASSERT(tc <= &textureCoordinates[sizeof(textureCoordinates)/sizeof(textureCoordinates[0]) - 1])
							PASSERT(vdp <= &vertexPositions[sizeof(vertexPositions)/sizeof(vertexPositions[0]) - 1])

							float const vx(vdp->position[0]);
							float const vz(vdp->position[2]);
							++vdp;

							P_STATIC_ASSERT(sizeof(textureCoordinates) == sizeof(textureCoordinates)/sizeof(textureCoordinates[0])*4)
							*tc++ =	static_cast<float>(
									_::eq(vx, -80.f/NORMALISING_FACTOR)?
										0.f
									:_::eq(vx, 80.f/NORMALISING_FACTOR)?
										1.f//width
									:
										0.f
									);
							*tc++ = static_cast<float>(
									_::eq(vz, -60.f/NORMALISING_FACTOR)?
										0.f
									:_::eq(vz, 60.f/NORMALISING_FACTOR)?
										1.f//height
									:
										0.f
									);
							tc += 2;
						}
					}

					// Back to VAO#0
					glBindVertexArray(vertexArrayIds[0]); __NE()
					// Buffer#4: texture coordinates
					glBindBuffer(GL_ARRAY_BUFFER, bufferIds[4]); __NE()
					glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), &textureCoordinates[0], GL_STATIC_DRAW); __NE()
					glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, 0, 0); __NE()

					glEnableVertexAttribArray(3); __NE()
				}
				else
					global::logger::Get().Warning(_T("Could not load texture Stone"));

			}


			return &drawData;
		}

		void cleanup (void*& _drawData) {
			{
				_::DrawData&	drawData				(*static_cast<_::DrawData*>(_drawData));
				GLuint			(&vertexArrayIds)[2]	(drawData.vertexArrayIds);
				GLuint			(&bufferIds)[5]			(drawData.bufferIds);
				GLuint			(&textureIds)[1]		(drawData.textureIds);

				P_STATIC_ASSERT(sizeof(vertexArrayIds)/sizeof(vertexArrayIds[0]) == 2)
				P_STATIC_ASSERT(sizeof(bufferIds)/sizeof(bufferIds[0]) == 5)

				glDeleteBuffers(sizeof(bufferIds)/sizeof(bufferIds[0]), &bufferIds[0]); __NE()
				glDeleteVertexArrays(sizeof(bufferIds)/sizeof(bufferIds[0]), &vertexArrayIds[0]); __NE()

				glDeleteTextures(sizeof(textureIds)/sizeof(textureIds[0]), &textureIds[0]); __NE()
			}

			delete _drawData;
			_drawData = NULL;
		}

	} // namespace drawing

} // namespace my

