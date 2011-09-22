#define WITH_MS_BUG_001 0

// MS BUG HAHAHAHAHAHAHHAHAHAHHAHHAHAHAHA
#if WITH_MS_BUG_001
template <typename T>
class C { };


template <typename T>
template <>
struct C<T>::LALOS<true> {};
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
//	catch (A a) {}

	std::cout << pig<std::iterator_traits<std::list<int>::iterator>::iterator_category>::t2s() << std::endl;


	return 0;
}

#endif