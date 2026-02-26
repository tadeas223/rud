#ifndef RUD_DS_LINEAR_VIEW_HPP
#define RUD_DS_LINEAR_VIEW_HPP

#include "rud/base/types.hpp"
namespace rud::ds {
    template<typename T>
    struct LinearView {
        T* (*p_get_func)(void* ctx, u32 index);
        u32 (*p_len_func)(void* ctx);

        void* p_ctx;

        inline static LinearView<T> make(void* ctx, u32 (*len_func)(void* ctx), T* (*get_func)(void*, u32)) {
            return {get_func, len_func, ctx};
        }

        inline T* get(u32 index) const {
            return p_get_func(p_ctx, index);
        }
        
        inline u32 len() const {
            return p_len_func(p_ctx);
        }

        inline T* operator[](u32 index) const {
            return p_get_func(p_ctx, index);
        }
    };
}

#endif
