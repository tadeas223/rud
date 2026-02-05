#ifndef LINEAR_VIEW_HPP
#define LINEAR_VIEW_HPP

#include "rud/macros.hpp"
#include "rud/types.hpp"
namespace rud::ds {
    template<typename T>
    struct LinearView {
        const T* (*get_func)(void* ctx, u32 index);
        T* (*set_func)(void* ctx, u32 index);
        u32 (*len_func)(void* ctx);

        void* ctx;

        const T* get(u32 index) const {
            return get_func(ctx, index);
        }
        
        T* set(u32 index) {
            return get_func(ctx, index);
        }
        
        u32 len() const {
            return len_func(ctx);
        }

        const T* operator[](u32 index) const {
            return get_func(ctx, index);
        }
        
        T* operator[](u32 index) {
            return set_func(ctx, index);
        }
        
        TempFuncDecl(LinearView)
    };
}

#endif
