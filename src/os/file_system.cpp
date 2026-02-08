#include "rud/env.hpp"

#ifdef OS_LINUX
#include "impl/linux/linux_file_system.cpp"
#else
#error "file_system not implemented for this os"
#endif
