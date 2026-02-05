#ifndef RUD_DS_ARRAY_HPP
#define RUD_DS_ARRAY_HPP

#include "rud/compile_settings.hpp"
#include "rud/types.hpp"
namespace rud::ds {
    template<typename T, u32 N>
    struct Array {
        T data[N];

        static Array<T, N> make() {
            return {};
        }

        constexpr u32 len() {
            return N;
        }

        const T* get(u32 index) {
            Assert(index < N, Lit("index is outside of an array").temp());

            return &data[index];
        }

        T* set(u32 index) {
            Assert(index < N, Lit("index is outside of an array").temp());

            return &data[index];
        }
        
        void set(u32 index, T value) {
            Assert(index < N, Lit("index is outside of an array").temp());

            data[index] = value;
        }

        T* operator[](u32 index) {
            return set(index);
        }
        
        const T* operator[](u32 index) const {
            return get(index);
        }
    };
}

#endif
