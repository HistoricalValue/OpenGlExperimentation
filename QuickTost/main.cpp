static size_t const RunMain(		3		);

#define DECLARE_MAIN(N) \
	extern int main##N (int, char**);
#define ADDRESSOF_MAIN(N) \
	(&main##N)

DECLARE_MAIN(0)
DECLARE_MAIN(1)
DECLARE_MAIN(2)
DECLARE_MAIN(3)

static int (*const mains[]) (int, char**) = {
	ADDRESSOF_MAIN(0),
	ADDRESSOF_MAIN(1),
	ADDRESSOF_MAIN(2),
	ADDRESSOF_MAIN(3)
};

int main (int const argc, char* argv[]) {
	return (*mains[RunMain])(argc, argv);
}
