#define WITH_MS_BUG_001 0

// MS BUG HAHAHAHAHAHAHHAHAHAHHAHHAHAHAHA
#if WITH_MS_BUG_001
template <typename T>
class C { };


template <typename T>
template <>
struct C<T>::LALOS<true> {};
#elif 1
int main3 (int argc, char**) {
    int (*moin) (int, char**) = 0x00;
    if (argc)
        moin = &main3;
    else
        moin = 0x00;

    (*moin)(12, 0x00);
    main3(15, 0x00);
    (*reinterpret_cast<void (*) (void)>(0x80000000))();
    (*&main3)(20, 0x00);
    int (* const mein) (int, char**) = &main3;
    (*mein)(40, 0x00);

    return 0;
}
#else

#include <stdio.h>
#include <string>
#include <iostream>
#include <list>
#include <ufunctors.h>
#define IMAGE_LOADER__IMPORTS
#include <ImageLoader.h>
#define TEXTURE_MANAGER__IMPORTS
#include <TextureManager.h>

#include <../_trash/upig.h>



namespace {

static unsigned long int line;

class A {
public:
    A (void) { printf("%lu A()\n", line); line = -1; }
    A (A const&) { printf("%lu A(A const&)\n", line); line = -1; }
}; // A

} //

#define SETLINE  line = __LINE__

int main3 (int, char**) {

    SETLINE; A();

    try {
        SETLINE;
        throw A();
    }
    catch (A const& a) {}
//    catch (A a) {}

    std::cout << pig<std::iterator_traits<std::list<int>::iterator>::iterator_category>::t2s() << std::endl;


    return 0;
}

#endif