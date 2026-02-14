#include "rud/base/string.hpp"
#include "rud/os/std_in.hpp"
#include "rud/os/std_out.hpp"

using namespace rud::ds;
using namespace rud;
using namespace rud::os;
using namespace rud::os_low;

int main (int argc, char *argv[]) {
    StdIn in = StdIn::make();

    debug_print(Lit("your name: "));

    AllocString name = in.read_line().or_panic();

    AllocString str = AllocString::make();
    str.push_copy(Lit("hello, "))
        ->push_copy(name);
    
    debug_print(str);

    name.destroy();
    str.destroy();

    return 0;
}
