#include "rud/memory.hpp"

#include "rud/ds/linear_view.hpp"
#include "rud/os/io.hpp"
#include "rud/os/system.hpp"
#include <cstdio>

#include "rud/ds/list.hpp"
#include "rud/ds/array.hpp"

using namespace rud;
using namespace rud::ds;
using namespace rud::os;

int main (int argc, char *argv[]) {
    Array<String, 1> array = Array<String, 1>::make();
    array.set(0, Lit("test.txt"));

    run_process(Lit("cat"), array.to_linear_view());
    return 0;
}
