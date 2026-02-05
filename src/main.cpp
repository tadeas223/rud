#include "rud/ds/array.hpp"
#include "rud/string.hpp"
#include "rud/ds/list.hpp"
#include "rud/os/io.hpp"
#include <cstdio>

using namespace rud;
using namespace rud::os;
using namespace rud::ds;

int main (int argc, char *argv[]) {
    Array<u32, 5> arr = Array<u32, 5>::make();
    *arr[0] = 1;
    *arr[1] = 1;
    *arr[2] = 2;

    for(u32 i = 0; i < arr.len(); i++) {
        printf("%d\n", *arr[i]); 
    }

    return 0;
}
