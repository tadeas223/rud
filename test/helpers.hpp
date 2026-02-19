#ifndef RUD_TEST_HELPERS_HPP
#define RUD_TEST_HELPERS_HPP

#include <cstdlib>
#include <sys/wait.h>

inline bool code_aborts(void (*func)()) {
    pid_t pid = fork();
    if(pid == 0) {
        // Child process
        func();        // call the code that should abort
        exit(0);       // if it didn't abort
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT;
    }
}

#endif
