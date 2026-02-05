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
    Array<String, 1> args = Array<String, 1>::make();
    args.set(0, Lit("5"));

    run_process(Lit("sleep"), args.to_linear_view());

    debug_print(Lit("done"));
    return 0;
}
