#ifndef RUD_DS_ARRAY_HPP
#define RUD_DS_ARRAY_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/destroyer.hpp"
#include "rud/ds/linear_view.hpp"
#include "rud/base/types.hpp"

namespace rud::ds {
    template<typename T, u32 N>
    struct Array {
        T p_data[N];
        
// get_set {{{ 
        inline const T* data() const {
            return p_data;
        }

        inline u32 len() const {
            return N;
        }
// }}}

// make_destroy {{{ 
        inline static Array<T, N> make() {
            return {};
        }
    
        inline void destroy_contents(Destroyer<T> destroyer) {
            for(u32 i = 0; i < N; ++i) {
                destroyer.invoke(p_data[i]);
            }
        }
// }}}
        
        inline void set(u32 index, T value) {
            Pre(index < N);

            p_data[index] = value;
        }
        
        inline T* get(u32 index) {
            Pre(index < N);

            return &p_data[index];
        }

        inline T* operator[](u32 index) {
            return get(index);
        }

        inline LinearView<T> to_linear_view() {
            return LinearView<T>::make(
                this, 
                [] (void* ctx) {
                    Array<T, N>* self = static_cast<Array<T, N>*>(ctx);
                    return self->len();
                },
                [](void* ctx, u32 index) {
                    Array<T, N>* self = static_cast<Array<T, N>*>(ctx);
                    return self->get(index);
                }
            );
        }
// }}}
    
    };
}

#endif
