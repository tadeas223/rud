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

    Vector<String> format = Vector<String>::make();
    format.push(Lit("hello, "));
    format.push(name);

    AllocString format_str = string_join(format.to_linear_view());
    debug_print(format_str);

    format_str.destroy();
    name.destroy();
    return 0;
}
