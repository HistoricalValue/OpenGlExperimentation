#if 1
#include <nmutil/Functional.h>

using namespace nmutil;

struct A{};
struct B{};
struct C{};

static inline bool fA (A&) { return true; }
static inline bool fB (B const&) { return true; }
static inline bool fAB (A&, B const&) { return true; }
static inline bool fABC (A&, B const&, C) { return true; }

bool f (A&, const B&, C) {
	return true;
}

int main (int, char**) {
	A a;

	uptr_fun(&fA)(a);
	uptr_fun(&fB)(B());

	uptr_fun(&fAB)(a, B());

	uptr_fun(&fABC)(a, B(), C());

	ubind2nd(uptr_fun(&f), (B()))(a, (C()));
	ubind1st(uptr_fun(&f), a)(B(), C());
}
#else
#include <ufunctors.h>

struct A{};
struct B{};
struct C{};
typedef utriple<A,B,C> XXX;
static inline bool fA (A&) { return true; }
static inline bool fB (B const&) { return true; }
static inline bool fC0 (C&) { return true; }
static inline bool fC1 (C const&) { return true; }
typedef std::pointer_to_unary_function<A&, bool> FA;
typedef std::pointer_to_unary_function<B const&, bool> FB;
typedef ufunctor_first<XXX, FA> apply_FA;
typedef ufunctor_second<XXX, FB> apply_FB;

//#define TMPXXX() XXX(A(),B(),C())
static inline XXX TMPXXX (void) { return XXX(A(),B(),C()); }

int main (int, char**) {

	XXX	xxx((A()),(B()),(C()));
	apply_FA(std::ptr_fun(&fA))(xxx);

	apply_FB(std::ptr_fun(&fB))(XXX(A(),B(),C()));

	//

	utuple_firstfunctoriser<XXX>(std::ptr_fun(&fA))(xxx);
	utuple_secondfunctoriser<XXX>(std::ptr_fun(&fB))(TMPXXX());
	utuple_thirdfunctoriser<XXX>(std::ptr_fun(&fC1))(TMPXXX());
	utuple_thirdfunctoriser<XXX>(std::ptr_fun(&fC0))(static_cast<XXX&>(TMPXXX()));
}

#endif