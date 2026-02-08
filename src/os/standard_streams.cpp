#include "rud/env.hpp"

#ifdef OS_LINUX
#include "impl/linux/linux_standard_streams.cpp"
#else
#error "standard_streams not implemented for this os"
#endif
