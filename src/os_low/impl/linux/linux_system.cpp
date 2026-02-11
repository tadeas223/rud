#include "rud/os_low/system.hpp"
#include "rud/base/memory.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace rud::os_low {
    [[noreturn]] void exit(u32 status_code) {
        _exit(status_code);
    }

    void run_process(const String msg, const ds::LinearView<String> args) {
        u32 len = args.len();

        ascii* cstr_msg = msg.to_cstr();
        
        // program args nullptr
        ascii** argv = static_cast<ascii**>(allocate_size(sizeof(ascii*) * (len+2)));
        
        argv[0] = cstr_msg;
        for(u32 i = 0; i < len; i++) {
            ascii* cstr = args[i]->to_cstr();
            argv[i+1] = cstr;
        }
        argv[len + 1] = nullptr;

        pid_t pid = fork();
        if(pid == 0) {
            execvp(cstr_msg, reinterpret_cast<char* const*>(argv));
            _exit(127);
        }

        waitpid(pid, nullptr, 0);
        
        u32 i = 0;
        while(argv[i] != nullptr) {
            deallocate(argv[i]);
            i++;
        }

        deallocate(argv);
    }
}
