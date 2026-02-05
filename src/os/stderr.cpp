#include "rud/env.hpp"

#ifdef OS_LINUX
#include "impl/linux/linux_stderr.cpp"
#else
#error "stderr not implemented for this os"
#endif
