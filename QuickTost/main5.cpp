#include "../../ankh/Images/_trash/upig.h"
#include <ustrings.h>
#include <ufunctors.h>
#include <Mesh.h>

template <typename MeshElementsType>
static inline void lol (MeshElementsType& els) {
    using namespace ankh::shapes;
    typedef uspecific_mem_fun1<void, MeshElementsType, MeshElement const&, &MeshElementsType::push_back> Operation;

//    ubind1st(uspecific_mem_fun1<void, MeshElementsType, shapes::MeshElement const&, &MeshElementsType::push_back>(), *meshElements);
    puts(ucstringarg(upig(ubind1st(Operation(), &els))));

    puts(ucstringarg(pig<ubind_traits<Operation>::binder1st_type::ValRef>::t2s()));
}

int main5 (int, char**) {
    ankh::shapes::Mesh::Elements els;
    lol(els);

    puts("hit entar");
    getchar();
    return 0;
}
