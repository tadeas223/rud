#ifndef RUD_DS_LINEAR_VIEW_HPP
#define RUD_DS_LINEAR_VIEW_HPP

#include "rud/base/types.hpp"
namespace rud::ds {
    template<typename T>
    struct LinearView {
        const T* (*get_func)(void* ctx, u32 index);
        void (*set_func)(void* ctx, u32 index, T value);
        u32 (*len_func)(void* ctx);

        void* ctx;

        const T* get(u32 index) const {
            return get_func(ctx, index);
        }
        
        void set(u32 index, T value) {
            return get_func(ctx, index, value);
        }
        
        u32 len() const {
            return len_func(ctx);
        }

        const T* operator[](u32 index) const {
            return get_func(ctx, index);
        }
    };
}

#endif
