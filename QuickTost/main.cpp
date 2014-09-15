static size_t const RunMain(        6        );

#define DECLARE_MAIN(N) \
    extern int main##N (int, char**);
#define ADDRESSOF_MAIN(N) \
    (&main##N)

DECLARE_MAIN(0)
DECLARE_MAIN(1)
DECLARE_MAIN(2)
DECLARE_MAIN(3)
DECLARE_MAIN(4)
DECLARE_MAIN(5)
DECLARE_MAIN(6)

static int (*const mains[]) (int, char**) = {
    ADDRESSOF_MAIN(0),
    ADDRESSOF_MAIN(1),
    ADDRESSOF_MAIN(2),
    ADDRESSOF_MAIN(3),
    ADDRESSOF_MAIN(4),
    ADDRESSOF_MAIN(5),
    ADDRESSOF_MAIN(6)
};

int main (int const argc, char* argv[]) {
    return (*mains[RunMain])(argc, argv);
}
