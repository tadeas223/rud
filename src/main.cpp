#include "rud/ds/vector.hpp"
#include <cstdio>
using namespace rud::ds;
using namespace rud;
int main (int argc, char *argv[]) {
    Vector<u32> vec = Vector<u32>::make();

    vec.push(1);
    vec.push(2);
    vec.push(3);
    vec.push(4);
    vec.push(5);

    vec.set(3, 19);

    for(u32 i = 0; i < vec.len(); i++) {
        printf("%d\n", *vec[i]); 
    }

    vec.destroy();
    return 0;
}
