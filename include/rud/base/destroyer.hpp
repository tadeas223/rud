#ifndef RUD_BASE_DESTROYER_HPP
#define RUD_BASE_DESTROYER_HPP

namespace rud {
    template<typename T>
    struct Destroyer {
        void* p_ctx;
        void (*destroy_func)(void*, T);
    
        inline static Destroyer make(void* ctx, void (*destroy_func)(void*, T)) {
            return {ctx, destroy_func};
        }

        void invoke(T value) {
            destroy_func(p_ctx, value);
        }
    };
}

#endif
