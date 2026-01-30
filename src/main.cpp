#include "rud/compile_settings.hpp"
#include "rud/memory.hpp"
#include "rud/option.hpp"
#include "rud/os/io.hpp"
#include "rud/system.hpp"

using namespace rud;
using namespace rud::os;

int main (int argc, char *argv[]) {
    Result<File, IOError> r_file = File::make(Lit("test.txt"),
        FileAccessMode::Read);

    if(r_file.is_error()) {
        panic(Lit("failed to open file"));
    }
    File file = r_file.unwrap();
    
    AllocString string = file.read_to_string()
        .except(Lit("failed to read file"));

    debug_print(string);

    string.destroy();
    file.destroy();
    return 0;
}
