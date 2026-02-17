#include <iostream>
#include <stdio.h>
#include <chrono>
#include "rud/base/string_functions.hpp"
#include "rud/ds/c_dlist.hpp"
#include "rud/base/memory.hpp"
#include "rud/os/std_in.hpp"
#include "rud/os/directory.hpp"
#include "rud/os/file.hpp"
#include "rud/os/std_out.hpp"
#include "rud/os_low/io.hpp"
#include "rud/os_low/system.hpp"

using namespace rud::ds;
using namespace rud;
using namespace rud::os;
using namespace rud::os_low;

int main (int argc, char *argv[]) {
    C_DList<u32> list = C_DList<u32>::make();

    list.push_front(1);

    list.destroy();
    return 0;
}
