#ifndef __MY_UTIL__MY__GL__MATH__VECTOR_4__H__
#define __MY_UTIL__MY__GL__MATH__VECTOR_4__H__

#include <MyUtil.h>

namespace my { namespace gl { namespace math {

	class MYUTIL_API Vector4 {
	public:
		static Vector4									New (float x = 0.f, float y = 0.f, float z = 0.f, float w = 1.f);
		static Vector4									New (float const v[4]);

														Vector4 (Vector4 const&);
														~Vector4 (void);

		void											operator = (Vector4 const& other);

		Vector4											operator + (Vector4 const& other) const;

		Vector4 const									normalised (void) const;			// magnitude = 1
		float											magnitude (void) const;				// NOTICE: normalised by "w"
		float											operator * (Vector4 const&) const;	// inner/dot product (.)				a . b = ||a|| ||b|| cos(a^b) , a.b < ||a|| ||b||
		Vector4											operator * (float) const;			// scalar multiplication
		Vector4											mul (Vector4 const&) const;			// element-by-element multiplication
		float											angle (Vector4 const&) const;		// angle this^other in [-pi, pi]
		Vector4											operator - (Vector4 const&) const;	// vector subtraction (element-by-element)
		Vector4											operator / (Vector4 const&) const;	// element-by-element division
		template <typename T>
		Vector4											operator / (T) const;				// division by scalar
	/// TODO implement (figure out)
	//	Vector4											rhcross (Vector4 const&) const;		// right-hand-system cross-product (x)	||a|| ||b|| sin(a^b) *directionVector
	//	Vector4											lhcross (Vector4 const&) const;		// left-hand-system cross-product (x)

		//
		struct Setter {
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

	typedef Vector4 vec4;
	inline vec4 const O			(void)	{ return vec4(); }
	inline vec4 const Axis_X	(void)	{ return vec4(1, 0, 0, 1); }
	inline vec4 const Axis_Y	(void)	{ return vec4(0, 1, 0, 1); }
	inline vec4 const Axis_Z	(void)	{ return vec4(0, 0, 1, 1); }

}}} // namespace my::gl::math


#endif // __MY_UTIL__MY__GL__MATH__VECTOR_4__H__
