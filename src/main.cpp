#include "rud/ds/array.hpp"
#include "rud/ds/c_darray.hpp"
#include "rud/ds/c_list.hpp"
#include <cstdio>
#include <vector>

using namespace rud;
using namespace rud::ds;

int main (int argc, char *argv[]) {
    C_DArray darray = C_DArray<u32>::make();
    
    constexpr u32 len = 1000;
    for(u32 i = 0; i < len; i++) {
        darray.push(i);
    }

    struct _Ctx {
        u32 some_value = 10000;
    } darray_ctx;

    darray.destroy(Destroyer<u32>::make(&darray_ctx, [](void* ctx, u32 value) {
        _Ctx* context = reinterpret_cast<_Ctx*>(ctx);
        printf("%d\n", value + context->some_value);
    }));
    return 0;
}


// int main (int argc, char *argv[]) {
//     std::vector<u32> vector;
//     
//     constexpr u32 len = 100000;
//     for(u32 i = 0; i < len; i++) {
//         vector.push_back(i);
//     }
// 
//     for(u32 i = 0; i < len; i++) {
//         printf("%d\n", vector[i]);
//     }
// 
//     return 0;
// }
