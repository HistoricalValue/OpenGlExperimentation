#include <ufunctors.h>
#include <stdio.h>


struct A{
    int a;
    A (void): a(12) {}
    A (A const& o): a(o.a + 1) {}
    ~A(void) {
        char a[] = "AAAAAAAAAAAA";
    }
};
struct B{
    int b;
    B (void): b(25) {}
    B (B const& o): b(o.b + 1) {}
    ~B (void) {
        char a[] =" AAAAAAAAAAA" ;
    }
};
struct C{};

struct XXX {
    bool f0 (void) { printf("x=%d\n", x); return true; }
    bool f0c (void) const { printf("x=%d\n", x); return true; }

    bool f1  (A const & a) { printf("a=%d x=%d\n", a.a, x); return true; }
    bool f1c (A& a) const { printf("a=%d x=%d\n", a.a, x); return true; }

    bool f2  (A& a, B const& b) { printf("a=%d, b=%d, x=%d\n", a.a, b.b, x); return true; }
    bool f2c (A& a, B const& b) const { printf("a=%d, b=%d, x=%d\n", a.a, b.b, x); return true; }

    int x;
    XXX (void): x(23) {}
};

static inline bool fA (A&) { return true; }
static inline bool fB (B const&) { return true; }
static inline bool fAB (A&, B const&) { return true; }
static inline bool fABC (A&, B const&, C) { return true; }

inline bool f0        (void)                                { puts("f0"); return true; }
inline bool f1        (A& a)                                { printf("f1: a=%d\n", a.a); return true; }
inline bool f2        (A& a, B const& b)                    { printf("f2: a=%d b=%d\n", a.a, b.b); return true; }
inline bool f1ref3    (A&,        B const &,    C        )    { puts("f1ref [3]"); return true; }
inline bool f1cref3    (A const&,    B const &,    C        )    { puts("f1cref[3]"); return true; }
inline bool f1scal3    (A const,    B const &,    C        )    { puts("f1scal[3]"); return true; }
inline bool f2ref3    (A&,        B&,            C        )    { puts("f2ref [3]"); return true; }
inline bool f2cref3    (A const&,    B const &,    C        )    { puts("f2cref[3]"); return true; }
inline bool f2scal3    (A const,    B const,    C        )    { puts("f2scal[3]"); return true; }
inline bool f3ref3    (A&,        B const &,    C&        )    { puts("f3ref [3]"); return true; }
inline bool f3cref3    (A const&,    B const &,    C const&)    { puts("f3cref[3]"); return true; }
inline bool f3scal3    (A const,    B const &,    C const    )    { puts("f3scal[3]"); return true; }
inline bool f1ref2    (A&,        B const &            )    { puts("f1ref [2]"); return true; }
inline bool f1cref2    (A const&,    B const &            )    { puts("f1cref[2]"); return true; }
inline bool f1scal2    (A const,    B const &            )    { puts("f1scal[2]"); return true; }
inline bool f2ref2    (A&,        B&                    )    { puts("f2ref [2]"); return true; }
inline bool f2cref2    (A const&,    B const &            )    { puts("f2cref[2]"); return true; }
inline bool f2scal2    (A const,    B const                )    { puts("f2scal[2]"); return true; }
inline bool f3ref2    (A&,        B const &            )    { puts("f3ref [2]"); return true; }
inline bool f3cref2    (A const&,    B const &            )    { puts("f3cref[2]"); return true; }
inline bool f3scal2    (A const,    B const &            )    { puts("f3scal[2]"); return true; }

bool f (A&, const B&, C) {
    return true;
}

static void paramain (void);
static void paraparamain (void);
int main0 (int, char**) {
    paramain();
    return 0;
}

void paramain (void) {
    return paraparamain();
}

void paraparamain (void) {
    A a;
    B b;
    C c;
    XXX x;
    XXX const xx;

    if (false) {
        uptr_fun(&fA)(a);
        uptr_fun(&fB)(B());

        uptr_fun(&fAB)(a, B());

        uptr_fun(&fABC)(a, B(), C());

        ubind1st(uptr_fun(&f1ref3), a)(B(), C());
        ubind1st(uptr_fun(&f1cref3), A())(B(), C());
        ubind1st(uptr_fun(&f1scal3), A())(B(), C());
        ubind2nd(uptr_fun(&f2ref3), b)(A(), C());
        ubind2nd(uptr_fun(&f2cref3), B())(A(), C());
        ubind2nd(uptr_fun(&f2scal3), B())(A(), C());
        ubind3rd(uptr_fun(&f3ref3), c)(A(), B());
        ubind3rd(uptr_fun(&f3cref3), C())(A(), B());
        ubind3rd(uptr_fun(&f3scal3), C())(A(), B());

        ubind1st(uptr_fun(&f1ref2), a)(B());
        ubind1st(uptr_fun(&f1cref2), A())(B());
        ubind1st(uptr_fun(&f1scal2), A())(B());
        ubind2nd(uptr_fun(&f2ref2), b)(A());
        ubind2nd(uptr_fun(&f2cref2), B())(A());
        ubind2nd(uptr_fun(&f2scal2), B())(A());

        ubind1st(ubind3rd(uptr_fun(&f1ref3), C()), A())(B());
        ubind1st(ubind2nd(uptr_fun(&f1ref3), B()), A())(C());
        ubind1st(ubind1st(uptr_fun(&f1ref3), A()), B())(C());
        ubind2nd(ubind3rd(uptr_fun(&f1ref3), C()), B())(A());
        ubind2nd(ubind2nd(uptr_fun(&f1ref3), B()), C())(A());
        ubind2nd(ubind1st(uptr_fun(&f1ref3), A()), C())(B());

        ubind3rd(uptr_fun(&f), C())(a, B());
        ubind2nd(uptr_fun(&f), (B()))(a, (C()));
        ubind1st(uptr_fun(&f), a)(B(), C());

        ubind1st(uspecific_pointer_to_binary_function<bool, A&, B const&, &f1ref2>(), a)(B());
        ubind2nd(uspecific_pointer_to_binary_function<bool, A&, B const&, &f1ref2>(), B())(a);

        ubind1st(uspecific_pointer_to_ternary_function<bool, A&, B const&, C, &f1ref3>(), A())(B(),C());
        ubind2nd(uspecific_pointer_to_ternary_function<bool, A&, B const&, C, &f1ref3>(), B())(A(),C());
        ubind3rd(uspecific_pointer_to_ternary_function<bool, A&, B const&, C, &f1ref3>(), C())(A(),B());
    }

    if (false) {
        ubind1st(umemberfunctionpointer(&XXX::f1), &x)(A());
        ubind1st(umemberfunctionpointer(&XXX::f1c), &x)(a);
        ubind2nd(umemberfunctionpointer(&XXX::f1), A())(&x);
        ubind2nd(umemberfunctionpointer(&XXX::f1c), A())(&x);

        ubind1st(umemberfunctionpointer(&XXX::f2), &x)(a, B());
        ubind1st(umemberfunctionpointer(&XXX::f2c), &x)(a, B());
        ubind2nd(umemberfunctionpointer(&XXX::f2), A())(&x, B());
        ubind2nd(umemberfunctionpointer(&XXX::f2c), A())(&x, B());
        ubind3rd(umemberfunctionpointer(&XXX::f2), B())(&x, a);
        ubind3rd(umemberfunctionpointer(&XXX::f2c), B())(&x, a);

        ubind2nd(ubind1st(umemberfunctionpointer(&XXX::f2), &x), B())(A());

        XXX const xx;
        ubind1st(uspecific_const_mem_fun2<bool,XXX,A&,B const&,&XXX::f2c>(), &xx)(A(),B());
        ubind3rd(uspecific_mem_fun2<bool,XXX,A&,B const&,&XXX::f2>(), B())(&x, A());
        ubind2nd(uspecific_const_mem_fun1<bool,XXX,A&,&XXX::f1c>(), A())(&x);
        ubind1st(uspecific_mem_fun1<bool,XXX,A const&,&XXX::f1>(), &x)(A());
    }

    if (true) {
        uptr_fun(&f0)();
        uptr_fun(&f1)(a);
        ubind1st(uptr_fun(&f1), A(a))();

        ubind1st(ubind2nd(ubind3rd(uptr_fun(&f1ref3), C()), B()), a)();

        ubind1st(umemberfunctionpointer(&XXX::f0), &x)();
        ubind1st(umemberfunctionpointer(&XXX::f0c), &xx)();
        umemberfunctionpointer(&XXX::f2c)(&XXX(xx), A(a), B(B()));
        ubind3rd(umemberfunctionpointer(&XXX::f2c), B(B()))(&XXX(xx), A(a));
        ubind2nd(ubind3rd(umemberfunctionpointer(&XXX::f2c), B(B())), A(a))(&XXX(xx));
        ubind1st(ubind2nd(ubind3rd(umemberfunctionpointer(&XXX::f2c), B(B())), A(a)), &XXX(xx))();
    }

}
