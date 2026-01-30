#include "rud/memory.hpp"
#include "rud/os/io.hpp"

using namespace rud;
using namespace rud::os;

template <typename Writeable>
Result<void, IOError> write_hello(Writer<Writeable>& writer) {
    Result<u64, IOError> r_write = writer.write(Lit("hello\n"));
    if(!r_write.ok) {
        return Result<void, IOError>::make_error(r_write.error);
    }
    return Result<void, IOError>::make_ok();
}

int main (int argc, char *argv[]) {
    File file = File::make(Lit("test.txt"),
        FileAccessMode::Read|FileAccessMode::Write,
        FileCreateMode::Create
    ).except(Lit("failed to open or create a file"));
    
    StdOut out = StdOut::make();

    write_hello(out).except(Lit("failed to write to stdout"));
    write_hello(file).except(Lit("failed to write to a file"));

    file.destroy();
    return 0;
}
