#include <stdio.h>
#include <list>
#include "uthreadlib.h"
#include "ufunctors.h"
#include "DDebug.h"
#include "Parallelisation.h"

///////////////////////////////////////////////////////////

struct vec3 {
	float x, y, z;

	vec3 const	operator * (vec3 const& v) const
					{ return vec3(x * v.x, y * v.y, z * v.z); }
	vec3 const	operator * (float const f) const
					{ return vec3(f * x, f * y, f * z); }

	vec3 const	mul_scalar (float const f) const
					{ return (*this) * f; }

	void		operator = (vec3 const& v)
					{ x = v.x; y = v.y; z = v.z; }

	vec3 (float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
	vec3 (vec3 const& o): x(o.x), y(o.y), z(o.z) {}
	~vec3 (void) {}
};

///////////////////////////////////////////////////////

static inline void onerror (char const* const msg)
	{ puts(msg); }

static inline void modvec (vec3& v)
	{ v = v * 15.0f; }

int main6 (int argc, char** argv) {
	dinit(&onerror);

	{
		typedef std::list<vec3> vec3s;

		vec3s* const	vsp(reinterpret_cast<vec3s* const>(operator new(sizeof(vec3s))));
		new(vsp) vec3s;
		vec3s&			vs(*DNULLCHECK(vsp));

		puts("generating...");
		for (float i = 0.01f; i < 1.0f; i += 0.0000001f)
			vs.push_back(vec3(1.0f, 0.1f, 0.01f));

		{
			ParallelisationManager m;

			puts("modifying...");
			parallel_foreach(m, vs, 4, &modvec);
		}

		puts("don");
//		for (vec3s::const_iterator i = vs.begin(); i != vs.end(); ++i)
//			printf("<%f,%f,%f>\n", i->x, i->y, i->z);


		getchar();
	}

	dclose();
	return 0;
}
