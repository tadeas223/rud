#include "rud/env.hpp"

#ifdef OS_LINUX
#include "impl/linux/linux_system.cpp"
#else
#error "system not implemented for this os"
#endif
