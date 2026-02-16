#include "rud/os/directory.hpp"
#include "rud/os/file.hpp"
#include "rud/os/std_out.hpp"

using namespace rud::os;
using namespace rud;
using namespace rud::os_low;
using namespace rud::ds;

int main (int argc, char *argv[]) {
    C_Directory dir = C_Directory::make(Lit("doc")).or_panic();
    
    C_File file = C_File::make(&dir, Lit("test.txt"), FileAccessMode::Write, FileCreateMode::Create).or_panic();

    file.destroy();
    
    dir.destroy();
    return 0;
}
