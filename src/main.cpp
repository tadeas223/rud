#include <stdio.h>
#include "rud/ds/c_darray.hpp"
#include "rud/base/memory.hpp"

using namespace rud::ds;
using namespace rud;
int main (int argc, char *argv[]) {
    const char* s1 = "hello";
    char s2[6];
    
    mem_copy(s2, s1, 6);

    printf("%s\n", s2);
    return 0;
}
