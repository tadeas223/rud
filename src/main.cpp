#include "rud/os/file.hpp"

using namespace rud::os;
using namespace rud;
using namespace rud::os_low;

int main (int argc, char *argv[]) {
    File file = File::make(Lit("test.txt"),
        FileAccessMode::Write, FileCreateMode::Create).or_panic();

    file.write_str(Lit("hello")).or_panic();

    file.destroy();
    return 0;
}
