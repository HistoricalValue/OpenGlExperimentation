#include <stdio.h>


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

int main2 (int, char**) {
	printf("%.21lf\n%.21lf\n%.21lf\n", 
			double(M_PI) / 3.0f,
			double(M_PI_6) / 0.5f,
			double((M_PI_8) * 8.0f) / 3.0f
		);

	return 0;
}
