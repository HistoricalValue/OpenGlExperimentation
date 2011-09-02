#include <stdio.h>

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

	return 0;
}
