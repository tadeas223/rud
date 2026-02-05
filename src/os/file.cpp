#include "rud/env.hpp"

#ifdef OS_LINUX
#include "impl/linux/linux_file.cpp"
#else
#error "file not implemented for this os"
#endif
