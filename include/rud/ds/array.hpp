#ifndef RUD_DS_ARRAY_HPP
#define RUD_DS_ARRAY_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/ds/linear_view.hpp"
#include "rud/base/types.hpp"

namespace rud::ds {
    template<typename T, u32 N>
    struct Array {
        T p_data[N];

        static Array<T, N> make() {
            return {};
        }

        u32 len() const {
            return N;
        }

        const T* get(u32 index) const {
            Assert(index < N, Lit("index is outside of an array"));

            return &p_data[index];
        }

        void set(u32 index, T value) {
            Assert(index < N, Lit("index is outside of an array"));

            p_data[index] = value;
        }

        T* data() {
            return p_data;
        }
        
        const T* operator[](u32 index) const {
            return get(index);
        }

        LinearView<T> to_linear_view() {
            LinearView<T> view;

            view.ctx = this;

            view.get_func = [](void* ctx, u32 index) {
                Array<T, N>* self = static_cast<Array<T, N>*>(ctx);
                return self->get(index);
            };
            view.set_func = [] (void* ctx, u32 index, T value) { 
                Array<T, N>* self = static_cast<Array<T, N>*>(ctx);
                self->set(index, value);
            };
            view.len_func = [] (void* ctx) {
                Array<T, N>* self = static_cast<Array<T, N>*>(ctx);
                return self->len();
            };

            return view;
        }
    };
}

#endif
