#include "rud/ds/vector.hpp"
#include "rud/os/file.hpp"
#include "rud/os/read_stream.hpp"
#include "rud/os/std_in.hpp"
#include "rud/os/std_out.hpp"
#include "rud/os_low/io.hpp"
#include "rud/os_low/io_error.hpp"
#include <cstdio>
using namespace rud::ds;
using namespace rud;
using namespace rud::os;
using namespace rud::os_low;

void print(String str) {
    for(u32 i = 0; i < str.len; i++) {
        printf("%c", str.chars[i]);
    }
}

int main (int argc, char *argv[]) {
    StdIn in = StdIn::make();
    
    debug_print(Lit("your name: "));
    AllocString str = in.read_line().or_panic();
    debug_print(Lit("hello, "));
    debug_print(str);

    str.destroy();

    return 0;
}
