#include "stdafx.h"

template <typename C, typename F>
static void foreach (C const& c, F const& f)
	{ std::for_each(c.begin(), c.end(), f); }

#ifdef _DEBUG
#	define NOTEND(I,END) _NOTEND(I,END)
#else
#	define NOTNED(I,END)
#endif

template <typename I>
static inline I& _NOTEND (I& i, I const& end) {
	DASSERT(i != end);
	return i;
}

namespace {
namespace floats {
static const float precision(1e-06f);

static inline bool iszero	(float const f)						{ return -precision < f && f < precision; }
static inline bool equals	(float const f1, float const f2)	{ return iszero(f1-f2); }
static inline bool less		(float const f1, float const f2)	{ return f1 -f2 < precision; }

} // floats

template <typename T>
struct elcollector {
	std::list<T> els;
	elcollector& operator , (T const& el) { els.push_back(el); return *this; }
}; // elcollector<T>

} //


//////////////////////////////////////////////////////////////////////////////////////
//    LE SHIT !!!   //
namespace nurbs {
//////////////////////////////////////////////////////////////////////////////////////

class spline {
public:
	typedef unsigned char		uchar;
	typedef std::vector<float>	knots_t;
	typedef std::vector<my::gl::math::Vector4>	cpoints_t;

	uchar							order (void) const
										{ DASSERT(addsup()); return order_; }
	uchar							degree (void) const
										{ DASSERT(addsup()); return order_ - 1; }
	size_t							numknots (void) const
										{ DASSERT(addsup()); return knots.size(); }
	size_t							numcpoints (void) const
										{ DASSERT(addsup()); return cpoints.size(); }

	float const						getknot (size_t const i) const {
											DASSERT(addsup());
											DASSERT(i < numknots());
											return knots.at(i);
										}
	my::gl::math::Vector4 const&	getcpoint (size_t const i) const {
											DASSERT(addsup());
											DASSERT(i < numcpoints());
											return cpoints.at(i);
										}

	cpoints_t const&				getcpoints (void) const
										{ DASSERT(addsup()); return cpoints; }
	knots_t const&					getknots (void) const
										{ DASSERT(addsup()); return knots; }

	size_t							l (void) const
										{ DASSERT(addsup()); return numknots() - 1; }
	uchar							n (void) const
										{ DASSERT(addsup()); return numcpoints() - 1; }
	uchar							m (void) const
										{ DASSERT(addsup()); return order(); }
	uchar							k (void) const
										{ DASSERT(addsup()); return m() - 1; }

	bool							u_in_definition_domain (float const u) const
										{ DASSERT(addsup()); return knots.at(k()) <= u && u <= knots.at(n()+1); }


	template <typename KnotIter, typename CPointIter>
	spline (KnotIter const& knot_begin, KnotIter const& knot_end, CPointIter const& cpoint_being, CPointIter const& cpoint_end):
		knots(knot_begin, knot_end),
		cpoints(cpoint_being, cpoint_end),
		order_(std::distance(knot_begin, knot_end) - std::distance(cpoint_being, cpoint_end))
		{ DASSERT(addsup()); }

private:
	uchar								order_;		// m, curve degree k = m-1
	knots_t								knots;
	cpoints_t							cpoints;

	bool								addsup (void) const {
											//	k = m-1		=> OK by definition,
											//	n = l-m		=> numcpoints = numknots - order
											return order_ > 0 && numcpoints() == numknots() - order();
										}
};

//////////////////////////////////////////////////////////////////////////////////////
namespace _ {
//////////////////////////////////////////////////////////////////////////////////////

static inline float N1 (spline const& spl, size_t const i ,float const u) {
	bool const gt(u >= spl.getknot(i));
	bool const lt(i+1 == spl.l()? u <= spl.getknot(i+1) : u < spl.getknot(i+1));

	return (gt && lt)? 1.0f : 0.0f;
}

static inline float N (spline const& spl, size_t const i, size_t const m, float const u) {
	float result;

	size_t const k(m-1);

	if (m == 1)
		result = N1(spl, i, u);
	else {
		DASSERT(k >= 1);

		float const	nom1	= u - spl.getknot(i);
		float const	denom1	= spl.getknot(i+k) - spl.getknot(i);
		float const	nom2	= spl.getknot(i+k+1) - u;
		float const	denom2	= spl.getknot(i+k+1) - spl.getknot(i+1);

		bool const	both1zero	= floats::iszero(denom1) && floats::iszero(nom1);
		bool const	both2zero	= floats::iszero(denom2) && floats::iszero(nom2);

		DASSERT(both1zero || !floats::iszero(denom1));
		DASSERT(both2zero || !floats::iszero(denom2));

		float const	N1	= both1zero? 0.0f : (nom1/denom1) * N(spl,i,k,u);
		float const N2	= both2zero? 0.0f : (nom2/denom2) * N(spl,i+1,k,u);

		result = N1 + N2;

		DASSERT(result >= 0.0f);
		DASSERT(result <= 1.0f);
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////

static bool VerifyBaseFunctions (size_t const m, std::vector<float> const& knots) {
	using my::gl::math::vec4;
	DASSERT(m >= 1);

	size_t const l(knots.size() - 1);
	size_t const n(l - m);

	std::list<vec4> empty;
	spline spl(knots.begin(), knots.end(), empty.begin(), empty.end());

	// Verify that N_i,m has support in [u_i, u_i+m] for m>=1 and is also >=0 for all u in domain of definition
	for (size_t i(0); i <= n; ++i) { // foreach control point
		for (float u(spl.getknot(m-1)); u <= spl.getknot(n + 1); u += 0.01f) { // for all u in definition range
			float const	Nval			(N(spl,i,m,u));
			float const	u_i				(spl.getknot(i));
			float const	u_i_m			(spl.getknot(i+m));
			bool const	u_lt_u_i		(u < u_i);
			bool const	u_gt_u_i_m		(u > u_i_m);
			bool const	Nval_lt_zero	(Nval < 0.0f);
			bool const	Nval_is_zero	(floats::iszero(Nval));

			if (u_lt_u_i) {
				bool const isok(Nval_is_zero);
				DASSERT(isok);
				if (!isok)
					return false;
			}
			else
			if (u_gt_u_i_m) {
				bool const isok(Nval_is_zero);
				DASSERT(isok);
				if (!isok)
					return false;
			}

			if (Nval_lt_zero) {
				DASSERT(false);
				return false;
			}
		}
	}

	// Verify that SUM N_i,m(u) for i in [0,n] for all u in [u_m-1, u_n+1]
	for (float u(spl.getknot(m-1)); u <= spl.getknot(n+1); u += 0.01f) {
		float sum(0.0f);

		for (size_t i(0); i <= n; ++i) {
			sum += N(spl,i,m,u);
			DASSERT(floats::less(sum, 1.0000000f));
		}

		if (!floats::equals(sum, 1.0f)) {
			DASSERT(false);
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////

static inline my::gl::math::Vector4 p0 (spline const& spl, size_t const i, float const u) {
	return spl.getcpoint(i);
}

static inline my::gl::math::Vector4 p (size_t const j, size_t const i, float const u) {
	
}

//////////////////////////////////////////////////////////////////////////////////////

template <typename C>
static std::list<my::gl::shapes::Line> const linestrip (C const& points, my::gl::shapes::Colour const& col) {
	using my::gl::math::Vector4;
	using my::gl::shapes::Line;
	using my::gl::shapes::Vertex;
	using my::gl::shapes::Colour;
	using my::gl::shapes::ColourFactory;

	typename C::const_iterator points_end(points.end());
	typename C::const_iterator i(points.begin());
	DASSERT(i != points_end);
	typename C::const_iterator prev(i++);

	std::list<Line> result;
	for (; i != points_end; prev = i++)
		result.push_back(Line(Vertex(*prev), Vertex(*i), col, ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(ColourFactory::Brighter(col))))));

	return result;
}

template <typename C>
static std::list<my::gl::shapes::Point> const vertices (C const& points, my::gl::shapes::Colour const& col) {
	using my::gl::math::Vector4;
	using my::gl::shapes::Point;
	using my::gl::shapes::Vertex;
	using my::gl::shapes::Colour;

	typename C::const_iterator const points_end(points.end());
	typename C::const_iterator i(points.begin());

	std::list<Point> result;
	for (; i != points_end; i++)
		result.push_back(Point(Vertex(Vector4::New(*i)), col));

	return result;
}

template <typename C>
static inline void addshapesto (my::gl::shapes::ShapeCompositionFactory& f, C const& shapes) {
	using my::gl::shapes::ShapeCompositionFactory;
	using my::gl::shapes::Shape;
	foreach(shapes, ubind1st(uspecific_mem_fun1<void, ShapeCompositionFactory, Shape const&, &ShapeCompositionFactory::Add>(), &f));
}

#define LINE(BX,BY,BZ,EX,EY,EZ) (Line(Vertex(Vector4::New(BX,BY,BZ,1)), Vertex(Vector4::New(EX,EY,EZ,1))))

//////////////////////////////////////////////////////////////////////////////////////

static spline* _spl(NULL);
static float	minx(-0.8f);
static float	maxx( 0.8f);
static float	miny(-0.8f);
static float	maxy( 0.8f);

static void Initialise (void) {
	{
		DASSERT((minx < 0 && miny < 0 && maxx > 0 && maxy > 0));

		using my::gl::math::Vector4;
		using my::gl::math::vec4;

		size_t const width_units(16);
		size_t const height_units(16);
		float const width_unit( (maxx-minx) / width_units );
		float const height_unit( (maxy - miny) / height_units );

		elcollector<float> elcol;
		elcol , -4.f,1.f,1.5f,0.f,1.f,-4.f,-2.8f,-5.4f,6.2f,-6.4f,5.3f,0.f,0.8f,6.1f,-0.5f,3.4f,-7.4f,-0.6f,-6.2f,7.2f,6.1f,6.6f;
		DASSERT(elcol.els.size() % 2 == 0);

		std::vector<vec4> cpoints;
		cpoints.reserve(elcol.els.size() / 2);
		for (std::list<float>::const_iterator i(elcol.els.begin()); i != elcol.els.end(); ++i) {
			float const x(*i * width_unit);
			++i;
			DASSERT(i != elcol.els.end());
			float const y(*i * height_unit);

			cpoints.push_back(Vector4::New(x, y));
		}

		size_t const order(4);

		std::vector<float> knots;
		knots.reserve(cpoints.size() + order);
		for (size_t i(0); i < knots.capacity(); ++i)
			knots.push_back(float(i));

		_spl = DNEWCLASS(spline, (knots.begin(), knots.end(), cpoints.begin(), cpoints.end()));

		DASSERT(VerifyBaseFunctions(_spl->m(), knots));
	}

}

static void CleanUp (void) {
	udelete(_spl);
}

//////////////////////////////////////////////////////////////////////////////////////

static inline spline const& getspline (void)
	{ return *DPTR(_DNOTNULL(_spl)); }

static inline std::vector<my::gl::math::Vector4> const& cpoints (void)
	{ return getspline().getcpoints(); }

static inline std::vector<float> const& knots (void)
	{ return getspline().getknots(); }

//////////////////////////////////////////////////////////////////////////////////////

static my::gl::math::Vector4 nurbat (spline const& spl, float u) {
	using my::gl::math::vec4;

	DASSERT(spl.u_in_definition_domain(u));

	vec4 sum(vec4::New());

	typedef std::vector<vec4>		vec4s_t;
	typedef vec4s_t::const_iterator	ite_t;

	ite_t const	cpoints_begin	(spl.getcpoints().begin());
	ite_t const	cpoints_end		(spl.getcpoints().end());

	for (ite_t i(cpoints_begin); i != cpoints_end; ++i)
		sum.addtothis_asvec3(N(spl, std::distance(cpoints_begin, i), spl.m(), u) * *i);

	return sum;
}

//////////////////////////////////////////////////////////////////////////////////////

static std::list<my::gl::math::Vector4> const producenurbspoints (void) {
	using my::gl::math::Vector4;
	using my::gl::math::vec4;

	std::list<Vector4> points;
	spline const& spl(getspline());
	spline::knots_t const& knots(spl.getknots());

	{
		for (size_t j(spl.k()); j <= spl.n(); ++j) {
			for (float u(spl.getknot(j)); u <= spl.getknot(j + 1); u += 0.01f) {
				Vector4 sum(Vector4::New());

				std::list<Vector4> subparts;
				for (size_t i(j - (spl.k())); i <= j; ++i) {
					subparts.push_back(N(spl,i,spl.m(),u) * spl.getcpoint(i));
					sum.addtothis_asvec3(subparts.back());
				}

				DASSERT(floats::equals(static_cast<Vector4 const&>(sum).w(), 1.0f));
				DASSERT(static_cast<Vector4 const&>(sum).x() >= minx);
				DASSERT(static_cast<Vector4 const&>(sum).x() <= maxx);
				DASSERT(static_cast<Vector4 const&>(sum).y() >= miny);
				DASSERT(static_cast<Vector4 const&>(sum).y() <= maxy);

				points.push_back(sum);
			}
		}
	}

	return points;
}

//////////////////////////////////////////////////////////////////////////////////////
} // _
//////////////////////////////////////////////////////////////////////////////////////

void addaslinesto (my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::vec4;
	_::addshapesto(f, _::linestrip(_::producenurbspoints(), Colour(vec4::New(0.8f, 0.4f, 0.8f))));
}

void addaspointsto (my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::vec4;
	_::addshapesto(f, _::vertices(_::producenurbspoints(), Colour(vec4::New(0.5f, 0.2f, 0.2f))));
}

void addcontrolpointsto (my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::vec4;

	_::addshapesto(f, _::vertices(_::cpoints(), Colour(vec4::New(0.5f, 0.7f, 0.7f))));
}

void addknotpointsto (my::gl::shapes::ShapeCompositionFactory& f) {
	using my::gl::shapes::Colour;
	using my::gl::math::vec4;

	typedef std::vector<float>		knots_t;
	typedef knots_t::const_iterator	ite_t;

	spline const& spl(_::getspline());

	std::list<vec4> points;

	knots_t	const&	nots(_::knots());
	size_t			i	(spl.k());
	size_t const	end	(spl.n() + 2);
	for (; i != end; ++i)
		points.push_back(_::nurbat(spl, nots.at(i)));

	_::addshapesto(f, _::vertices(points, Colour(vec4::New(0.7f, 0.5f, 0.5f))));
}

//////////////////////////////////////////////////////////////////////////////////////
//   as dasd a s   //
}	// nurbs
//////////////////////////////////////////////////////////////////////////////////////






#define DONT	if (false)
#define DO		if (true)

#define TEXTURE_TYPE_2D		0x00
#define TEXTURE_TYPE_3D		0x01
#define TEXTURE_TYPE		TEXTURE_TYPE_3D

using namespace ::gl::ext;

namespace _ {
	static const bool	WITH_DRAW_POINTS	(true);
	static const bool	WITH_DRAW_LINES		(true);
	static const bool	WITH_DRAW_TRIANGLES	(false);
	static const bool	WITH_DRAW_TEXTURED	(false);
	//
	static const bool	WITH_CAMERA			(false);

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
		GLuint				numberOfPointPoints;
		unsigned long int	startingTime;
		unsigned long int	prevtime;
		GLuint				sampler_location;
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

		glVertexAttribPointer(OpenGL::VAI_POSITION,	4,	GL_FLOAT,	!normalised,	stride,	attr1off);
		glVertexAttribPointer(attr2index,			4,	GL_FLOAT,	!normalised,	stride,	attr2off);

		glEnableVertexAttribArray(OpenGL::VAI_POSITION);
		glEnableVertexAttribArray(attr2index);

		numberOfPoints = count;
	}



	static GLboolean const POINTS_NORMALISED(GL_TRUE);
	///////////////////////////////////////////////////////
	// Object setups
	///////////////////////////////////////////////////////
	static
	void SetupPointShapes (
			GLuint const	vertexArrayId,
			GLuint const	buffer0Id,
			GLuint const	buffer1Id,
			GLuint&			numberOfPointPoints)
	{
		// Save setup time
		if (_::WITH_DRAW_POINTS) {
			using namespace my::gl::shapes;
			ShapeCompositionFactory f;

			nurbs::addcontrolpointsto(f);
		//	nurbs::addaspointsto(f);
			nurbs::addknotpointsto(f);

			DynamicShapeComposition* const dcomp(f.Generate());

			SetAttribute(vertexArrayId, buffer0Id, *dcomp, POINTS_NORMALISED, false, numberOfPointPoints);

			f.Dispose(dcomp);
		}
	}

	static
	void SetUpLineShapes (
			GLuint const	vertexArrayId,
			GLuint const	buffer0Id,
			GLuint const	buffer1Id,
			GLuint&			numberOfPoints)
	{
		// Save setup time
		if (_::WITH_DRAW_LINES) {
			using namespace my::gl::shapes;
			using namespace my::gl::math;

			Nothing nothing;
			Axes axs;
			ShapeCompositionFactory f;

			f.Add(axs);
			nurbs::addaslinesto(f);

			DynamicShapeComposition* const dcomp(f.Generate());

			{
				Shape& shape(
					//	axs
					//	nothing
						*dcomp
					);

				SetAttribute(vertexArrayId, buffer0Id, shape, POINTS_NORMALISED, false, numberOfPoints);
			}

			f.Dispose(dcomp);
		}
	}

	static
	void SetUpTriangleObjects (
			GLuint const	vertexArrayId,
			GLuint const	buffer0Id,
			GLuint const	buffer1Id,
			GLuint&			numberOfWorldCubeLineSegments)
	{
		// Save setup time
		if (_::WITH_DRAW_TRIANGLES) {
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

				_::SetAttribute(vertexArrayId, buffer0Id, shape, POINTS_NORMALISED, false, numberOfWorldCubeLineSegments);
			}
		}
	}

	static
	void SetUpTexturedTriangleObjects (
			GLuint const	vertexArrayId,
			GLuint const	buffer0Id,
			GLuint const	buffer1Id,
			GLuint&			numberOfTexturedSegments)
	{
		// Save setup time
		if (_::WITH_DRAW_TEXTURED) {
			using namespace my::gl::shapes;
			using namespace my::gl::math;

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
		//	plane.RotateX((3.f * M_PI) / 2.f);
		//	plane.TranslateY(50.0f);
			plane.Scale(0.250f);


			SolidCube compos;
			compos.Scale(0.250f);
		/////////////

			NShapesComposition<2> scenery;
			scenery.Add(&companions);
			scenery.Add(&plane);

			Nothing nothing;
			// Upload shape as textured, buffer 2
			{
				Shape& shape(
					scenery
				//	compos
				//	nothing
					);
				_::SetAttribute(vertexArrayId, buffer0Id, shape, POINTS_NORMALISED, true, numberOfTexturedSegments);
			}
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

	static inline
	void SetupCamera (void) {
		using namespace my::gl::math::Transformations;
		using namespace my::gl::math;

		mat4 m(1);
		m *= Translate(0, 0, 1);
		if (_::WITH_CAMERA) {
			m *= Rotate(Axis_Y(), M_PI_4 + M_PI_8);
		//	m *= ScaleX(0.5f);
		}

		glUniformMatrix4fv(::my::OpenGL::VUL_CAMERA, 1, GL_TRUE, m.as_float_array_16());
	}

	static inline
	void SetupFrustrum (float const n, float const f, float const l, float const r, float const b, float const t) {
		PASSERT(n > 0)
		PASSERT(f > 0)
		PASSERT(f > n)
		PASSERT(r > l)
		PASSERT(t > b)

		float const r_l		= r - l;
		float const t_b		= t - b;
		float const f_n		= f - n;
		float const _2n		= 2.0f * n;
		float const Cwn		= _2n / r_l;
		float const Dw		= (-r -l) / r_l;
		float const Chn		= _2n / t_b;
		float const Dh		= (-t -b) / t_b;
		float const A		= (f + n) / f_n;
		float const B		= (_2n * -f) / f_n;

		using my::gl::math::mat4;
		mat4 m(
				Cwn	,	0	,	Dw	,	0,
				0	,	Chn	,	Dh	,	0,
				0	,	0	,	A	,	B,
				0	,	0	,	1	,	0);
		glUniformMatrix4fv(::my::OpenGL::VUL_PROJECTION, 1, GL_TRUE, m.as_float_array_16());
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
	void LoadTehStonets(ImagesArray& images)
	{
		// Save LOTS of setup time
		if (_::WITH_DRAW_TEXTURED) {
			using namespace ankh::images;

			ImageLoader& il(ImageLoader::GetSingleton());

			images[0] = il.LoadFromPaths("../textures/digitframes", 11, "png");	// gets loaded with Devil

			size_t bufbytesize;
			void* const buf(uloadbinaryfile("../textures/paccy.png", &bufbytesize));
			images[1] = il.Load3DFromData(32, "pacco", "png", buf, bufbytesize);
			delete[] buf;
		}
	}

	static
	void CreateTextures(
			ImagesArray const&	images,
			TexturesArray&		textures,
			size_t&				previousTextureIndex)
	{
		// Save setup time
		if (_::WITH_DRAW_TEXTURED) {
			using namespace ankh::textures;

			TextureUnitManager&	tum	(TextureUnitManager	::GetSingleton());
			TextureManager&		tm	(TextureManager		::GetSingleton());

			textures[0] = tm.New("Numberwng", images[0]);
			textures[1] = tm.New("Pacco", images[1]);

			textures[0]->BindTo(tum.Get(_::TexturesUnits[0]));
			textures[1]->BindTo(tum.Get(_::TexturesUnits[1]));

			previousTextureIndex = 0;
		}
	}

	static
	void ConfigureOpenGl (void) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	//	glEnable(GL_TEXTURE_3D);

		glDisable(GL_PROGRAM_POINT_SIZE); DASSERT(glGetError() == GL_NO_ERROR);
		glPointSize(5.f);
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

			// Draw points
			if (_::WITH_DRAW_POINTS) {
				PASSERT(glIsVertexArray(dd.vertexArrayIds[0] == GL_TRUE))
				glBindVertexArray(dd.vertexArrayIds[0]);
				glVertexAttrib4f(OpenGL::VAI_AXYC,
						angle,
						-0.0f,
						0.0f,
						cam);
				glUniform1ui(OpenGL::VUL_COLSELTR, _::COLOUR_WITH_COLOUR);
				glDrawArrays(GL_POINTS, 0, dd.numberOfPointPoints);
			}

			// Draw lines
			if (_::WITH_DRAW_LINES) {
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
			if (_::WITH_DRAW_TRIANGLES) {
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
			if (_::WITH_DRAW_TEXTURED) {
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
			GLuint&				numberOfPointPoints				(drawData.numberOfPointPoints);
			GLuint&				numberOfWorldCubeLineSegments	(drawData.numberOfWorldCubeLineSegments);
			unsigned long int&	startingTime					(drawData.startingTime);
			unsigned long int&	prevtime						(drawData.prevtime);
			GLuint&				sampler_location				(drawData.sampler_location);
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

			nurbs::_::Initialise();

			///////////////////////////
			// VAO#0: Points
			// (buffers #1)
			_::SetupPointShapes(vertexArrayIds[0], bufferIds[1], -1, numberOfPointPoints);
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
			_::LoadTehStonets(images);
			_::CreateTextures(images, textures, drawData.previousTextureIndex);
			_::ConfigureOpenGl();
			_::SetupCamera();
			_::SetupFrustrum(0.50f, 1.90f, -0.90f, 0.90f, -0.90f, 0.90f);

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

				if (_::WITH_DRAW_TEXTURED) {
					for (ankh::images::Image* const* i = &drawData.images[0]; i < &drawData.images[sizeof(drawData.images)/sizeof(drawData.images[0])]; ++i)
						ankh::images::ImageLoader::GetSingleton().Unload(*i);
					for (ankh::textures::Texture* const* i = &drawData.textures[0]; i < &drawData.textures[sizeof(drawData.textures)/sizeof(drawData.textures[0])]; ++i)
						ankh::textures::TextureManager::GetSingleton().Delete(*i);
				}

				DDELETE(&drawData);
			}
			_drawData = NULL;

			ankh::textures::CleanUp();
			ankh::images::CleanUp();

			nurbs::_::CleanUp();
		}

	} // namespace drawing

} // namespace my

