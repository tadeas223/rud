#ifndef RUD_TEST_HELPERS_HPP
#define RUD_TEST_HELPERS_HPP

#include <cstdio>
#include <sys/wait.h>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

inline bool code_aborts(void* ctx, void (*func)(void* ctx)) {
    pid_t pid = fork();
    if (pid < 0) {
        return false;
    } else if (pid == 0) {
        FILE* devnull = fopen("/dev/null", "w");
        if (devnull) {
            dup2(fileno(devnull), STDERR_FILENO);
            fclose(devnull);
        }

        func(ctx);      // Run the code that might abort
        _exit(0);    // Exit normally if no abort occurred
    } else {
        // Parent process
        int status = 0;
        waitpid(pid, &status, 0);

        // True if child terminated via SIGABRT
        return WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT;
    }
}

#endif
