#ifndef __MY_UTIL__MY__GL__MATH__VECTOR_4__H__
#define __MY_UTIL__MY__GL__MATH__VECTOR_4__H__

#include <MyUtil.h>

#define M_E			static_cast<float>(2.71828182845904523536f )
#define M_LOG2E		static_cast<float>(1.44269504088896340736f )
#define M_LOG10E	static_cast<float>(0.434294481903251827651f)
#define M_LN2		static_cast<float>(0.693147180559945309417f)
#define M_LN10		static_cast<float>(2.30258509299404568402f )
#define M_PI		static_cast<float>(3.14159265358979323846f )
#define M_PI_2		static_cast<float>(1.57079632679489661923f )
#define M_PI_3		static_cast<float>(1.047197551196597746154f)
#define M_PI_4		static_cast<float>(0.785398163397448309616f)
#define M_PI_6		static_cast<float>(0.523598775598298873077f)
#define M_PI_8		static_cast<float>(0.392699081698724154807f)
#define M_1_PI		static_cast<float>(0.318309886183790671538f)
#define M_2_PI		static_cast<float>(0.636619772367581343076f)
#define M_2_SQRTPI	static_cast<float>(1.12837916709551257390f )
#define M_SQRT2		static_cast<float>(1.41421356237309504880f )
#define M_SQRT1_2	static_cast<float>(0.707106781186547524401f)

namespace my { namespace gl { namespace math {

	class MYUTIL_API Vector4 {
	public:
		static Vector4									New (float x = 0.f, float y = 0.f, float z = 0.f, float w = 1.f);
		static Vector4									New (float const v[4]);
		static Vector4									New (Vector4 const&);

														Vector4 (Vector4 const&);
														~Vector4 (void);

		bool											operator == (Vector4 const& other) const;

		void											operator = (Vector4 const& other);

		Vector4											operator + (Vector4 const& other) const;
		void											operator += (Vector4 const& other);
		void											addtothis_asvec3 (Vector4 const& other);

		Vector4 const									normalised (void) const;			// magnitude = 1
		float											magnitude (void) const;				// NOTICE: normalised by "w"
		float											operator * (Vector4 const&) const;	// inner/dot product (.)				a . b = ||a|| ||b|| cos(a^b) , a.b < ||a|| ||b||
		Vector4											operator * (float) const;			// scalar multiplication
		Vector4											mul (Vector4 const&) const;			// element-by-element multiplication
		Vector4											mul (float) const;					// scalar multiplication without touching w
		float											angle (Vector4 const&) const;		// angle this^other in [-pi, pi]
		Vector4											operator - (Vector4 const&) const;	// vector subtraction (element-by-element)
		Vector4											operator / (Vector4 const&) const;	// element-by-element division
		template <typename T>
		Vector4											operator / (T) const;				// division by scalar
		template <typename T>
		Vector4											operator + (T) const;				// adding a scalar
		template <typename T>
		Vector4											add (T) const;						// adding a scalar without touching element 4 (w)
		template <typename T>
		void											addtothis (T);						// adding a scalar to this vector without touching element 4 (w)
	/// TODO implement (figure out)
	//	Vector4											rhcross (Vector4 const&) const;		// right-hand-system cross-product (x)	||a|| ||b|| sin(a^b) *directionVector
	//	Vector4											lhcross (Vector4 const&) const;		// left-hand-system cross-product (x)

		//
		struct MYUTIL_API Setter {
			Setter (float&, bool null = false);
			float operator = (float) const;
		private:
			float&	e;
			bool	null;
		};

		Setter											x (void);
		Setter											y (void);
		Setter											z (void);
		Setter											w (void);

		Setter											s (void);
		Setter											t (void);
		Setter											p (void);
		Setter											q (void);

		Setter											r (void);
		Setter											g (void);
		Setter											b (void);
		Setter											a (void);

		Setter											operator [] (size_t);

		//

		float											x (void) const;
		float											y (void) const;
		float											z (void) const;
		float											w (void) const;

		float											s (void) const;
		float											t (void) const;
		float											p (void) const;
		float											q (void) const;

		float											r (void) const;
		float											g (void) const;
		float											b (void) const;
		float											a (void) const;

		float											operator [] (size_t) const;

	private:
		float											v[4];
														Vector4 (float x, float y, float z, float w);
	}; // class Vector4

	template <typename T>
	inline Vector4 const operator * (T const& m, Vector4 const& v)
		{ return v * m; }

	typedef Vector4 vec4;
	inline vec4 const O			(void)	{ return vec4::New(); }
	inline vec4 const Axis_X	(void)	{ return vec4::New(1, 0, 0, 1); }
	inline vec4 const Axis_Y	(void)	{ return vec4::New(0, 1, 0, 1); }
	inline vec4 const Axis_Z	(void)	{ return vec4::New(0, 0, 1, 1); }

}}} // namespace my::gl::math


#endif // __MY_UTIL__MY__GL__MATH__VECTOR_4__H__
