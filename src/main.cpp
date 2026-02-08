#include "rud/ds/array.hpp"
#include "rud/os/file_system.hpp"
#include "rud/os/standard_streams.hpp"

using namespace rud;
using namespace rud::ds;
using namespace rud::os;

int main (int argc, char *argv[]) {
    File file = File::make(Lit("test.txt"),
        FileAccessMode::Write)
    .or_panic();

    file.write(Lit("test")).or_panic();

    file.destroy();
    return 0;
}
