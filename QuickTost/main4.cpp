#if 0

#else
template <bool condition> struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true> {};
#define STATIC_ASSERT(COND) { typedef char __static_assertion_failure_pointless_type##__LINE__ [sizeof(STATIC_ASSERTION_FAILURE<COND>)]; }

template <typename T1, typename T2>
struct typeequality { static const bool result = false; };

template <typename T>
struct typeequality<T,T>{ static const bool result = true; };

template <typename Tmust, typename T>
T const& checkedreturn (T const& val) {
	STATIC_ASSERT((typeequality<Tmust, T>::result))
	return val;
}

template <bool> struct reterner;
template <> struct reterner<true> { static int f (void) { return 0; } };
template <> struct reterner<false>{ static unsigned f (void) { return 0; }};

int main4 (int, char**) {

	return checkedreturn<int>(reterner<typeequality<double,double>::result>::f());
}

#endif
