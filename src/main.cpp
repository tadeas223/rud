#include "rud/base/string.hpp"
#include "rud/ds/vector.hpp"
#include "rud/os/std_in.hpp"
#include "rud/os/std_out.hpp"
#include "rud/os_low/system.hpp"

using namespace rud::ds;
using namespace rud;
using namespace rud::os;
using namespace rud::os_low;

int main (int argc, char *argv[]) {
    StdIn in = StdIn::make();
    StdOut out = StdOut::make();

    out.write_str(Lit("what is your name?\n")).or_panic();

    AllocString name = in.read_line().or_panic();

    AllocString format = AllocString::make();
    format.push_copy(Lit("hello, "));
    format.push_copy(name);

    out.write_str(format).or_panic();

    format.destroy();
    name.destroy();
    
    Vector<String> args = Vector<String>::make();
    run_process(Lit("ls"), args.to_linear_view());
    args.destroy();

    return 0;
}
