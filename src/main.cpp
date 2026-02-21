#include "rud/base/memory.hpp"
#include "rud/ds/c_dlist.hpp"
#include <limits>

using namespace rud::ds;
using namespace rud;

int main (int argc, char *argv[]) {
        void* a = allocate_size(std::numeric_limits<u64>::max());

        deallocate(a);


    return 0;
}
