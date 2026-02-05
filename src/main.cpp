#include <cstdio>
#define COMPILE_MODE unsafe_mode

#include "rud/ds/list.hpp"
#include "rud/ds/array.hpp"

using namespace rud;
using namespace rud::ds;

void test(const LinearView<u32>* view) {
    for(u32 i = 0; i < view->len()+1; i++) {
        printf("%d\n", *view->get(i));
    }
}

int main (int argc, char *argv[]) {
    List<u32> list = List<u32>::make();

    list.push(1);
    list.push(2);
    list.push(3);
    list.push(4);
    
    Array<u32, 4> arr = Array<u32, 4>::make();
    *arr[0] = 1;
    *arr[1] = 2;
    *arr[2] = 3;
    *arr[3] = 4;

    test(list.to_linear_view().temp());
    test(arr.to_linear_view().temp());

    list.destroy();
    return 0;
}
