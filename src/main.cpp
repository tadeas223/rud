#include "rud/base/memory.hpp"
#include "rud/base/string.hpp"
#include "rud/base/string_functions.hpp"
#include "rud/ds/c_vector.hpp"
#include "rud/os/std_in.hpp"
#include "rud/os/std_out.hpp"
#include "rud/os_low/system.hpp"
#include <cstdlib>
#include <unistd.h>

using namespace rud::ds;
using namespace rud;
using namespace rud::os;
using namespace rud::os_low;

enum class BuitinState {
    Exit,
    Executed,
    NotBuiltin,
};

BuitinState handle_builtin(String cmd, const C_Vector<StringView> args) {
    if(cmd == Lit("exit")) {
        return BuitinState::Exit;
    } else if(cmd == Lit("cd")) {
        ascii* cstr;
        bool free_cstr = true;
        if(args.len == 0) {
            free_cstr = false;
            cstr = getenv("HOME");
        } else {
            cstr = args[0]->to_cstr();
            debug_print(*args[0]);
            debug_print(Lit("\n"));
        }

        int result = chdir(cstr);
        if(free_cstr) {
            deallocate(cstr);
        }
        
        if(result < 0) {
            debug_print(Lit("not a directory\n"));
        }

        return BuitinState::Executed;
    }

    return BuitinState::NotBuiltin;
}

int main (int argc, char *argv[]) {
    StdIn in = StdIn::make();
    StdOut out = StdOut::make();

    out.write_str(Lit("shell :)\n")).or_panic();
    
    bool loop = true;
    while(loop) {
        out.write_str(Lit("> ")).or_panic();
        
        C_StringAlloc cmd = in.read_line().or_panic();

        C_Vector<StringView> split = string_split(cmd.view(), ' ');
        StringView name = split.pop_front();
        
        BuitinState builtin_state = handle_builtin(name, split);
        if(builtin_state == BuitinState::NotBuiltin) {
            run_process(*split[0], split.to_linear_view());
        } else if(builtin_state == BuitinState::Exit) {
            loop = false;
        }

        split.destroy();
        cmd.destroy();
    }

    return 0;
}

