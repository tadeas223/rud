#include "rud/ds/c_dlist.hpp"

using namespace rud::ds;
using namespace rud;

int main (int argc, char *argv[]) {
    C_DList<u32> list = C_DList<u32>::make();

    list.push_front(1);

    list.destroy();
    return 0;
}
