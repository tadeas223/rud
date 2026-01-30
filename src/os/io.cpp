#include "rud/env.hpp"

#ifdef OS_LINUX
#include "impl/linux/linux_io.cpp"
#else
#error "io not implemented for this os"
#endif
