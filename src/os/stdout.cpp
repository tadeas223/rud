#include "rud/env.hpp"

#ifdef OS_LINUX
#include "impl/linux/linux_stdout.cpp"
#else
#error "stdout not implemented for this os"
#endif
