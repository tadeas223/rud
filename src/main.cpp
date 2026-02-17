#include <iostream>
#include <stdio.h>
#include <chrono>
#include "rud/base/string_functions.hpp"
#include "rud/ds/c_darray.hpp"
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
    C_Directory current = C_Directory::get_current_directory().or_panic();

    C_File file = C_File::make(
            &current, 
            Lit("test.txt"), 
            FileAccessMode::Write, 
            FileCreateMode::Create
    ).or_panic();
    
    file.write_str(Lit("hello\n")).or_panic();

    current.destroy();
    file.destroy();
    return 0;
}
