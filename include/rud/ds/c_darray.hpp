#ifndef RUD_DS_C_DARRAY_HPP
#define RUD_DS_C_DARRAY_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/memory.hpp"
#include "rud/base/types.hpp"
#include "rud/ds/linear_view.hpp"

#define CapIncrease(prev) prev * 2 

namespace rud::ds {
    template<typename T>
    struct C_DArray {
        T* p_data;
        u32 p_len;
        u32 p_cap;

// get_set {{{ 
        inline const T* data() const {
            return p_data;
        }

        inline u32 len() const {
            return p_len;
        }

        inline u32 cap() const {
            return p_cap;
        }
// }}}

// make_destroy {{{ 
        static inline C_DArray make() {
            T* p_data = static_cast<T*>(allocate_size(sizeof(T) * 2));
            return {p_data, 0, 2};
        }
        
        static inline C_DArray make(u32 p_cap) {
            T* p_data = static_cast<T*>(allocate_size(sizeof(T) * p_cap));
            return {p_data, 0, p_cap};
        }
        
        inline void destroy() {
            deallocate(p_data);
        }
       
        inline void destroy(void (*destroy_func)(T value)) {
            for(u32 i = 0; i < p_len; ++i) {
                destroy_func(p_data[i]);
            }

            deallocate(p_data);
        }

        inline T* destroy_to_pointer() {
            return p_data;
        }
// }}}

        inline void push(T value) {
            if(p_len == p_cap) [[unlikely]] {
                resize(CapIncrease(p_cap));
            }

            p_data[p_len] = value;
            p_len++;
        }

        inline void push_front(T value) { 
            if(p_len == p_cap) { 
                resize(CapIncrease(p_cap)); 
            } 
            mem_move(p_data + 1, p_data, p_len * sizeof(T)); 
            p_data[0] = value;
            p_len++;
        }

        inline T pop() {
            Pre(len() != 0);
            
            p_len--;
            return p_data[p_len];
        }

        inline T pop_front() {
            Pre(len() != 0);
            
            T value = p_data[0];
            p_len--;
            mem_move(p_data, p_data + 1, p_len * sizeof(T));
            return value;
        }

        inline void set(u32 index, T value) {
            Pre(index < p_len);

            p_data[index] = value;
        }

        inline T remove(u32 index) {
            Pre(index < p_len);
            
            if(index == 0) {
                return pop_front();
            }

            T value = p_data[index];
            mem_copy(p_data + index, 
                    p_data + index + 1,
                    (p_len - index - 1) * sizeof(T)
            );

            p_len--;
            return value;
        }

        inline void clear() {
            p_len = 0;
        }
        
        inline void clear(void (*destroy_func)(T value)) {
            for(u32 i = 0; i < p_len; ++i) {
                destroy_func(p_data[i]);
            }
            p_len = 0;
        }
        
        inline T* peek() {
            Assert(p_len != 0, Lit("cannot peek empty vector"));
            return &p_data[p_len-1];
        }

        inline T* peek_front() {
            Assert(p_len != 0, Lit("cannot peek empty vector"));
            return &p_data[0];
        }

        inline T* get(u32 index) {
            return &p_data[index];
        }

        inline void resize(u32 new_p_capacity) {
            p_data = static_cast<T*>(reallocate(p_data, sizeof(T) * new_p_capacity));
            p_cap = new_p_capacity;
        }
        
        inline T* operator[](u32 index) {
            return get(index);
        }
        
        inline LinearView<T> to_linear_view() {
            return LinearView<T>::make(
                this,
                [] (void* ctx) {
                    C_DArray<T>* self = static_cast<C_DArray<T>*>(ctx);
                    return self->p_len;
                },
                [](void* ctx, u32 index) {
                    C_DArray<T>* self = static_cast<C_DArray<T>*>(ctx);
                    return self->get(index);
                }
            );
        }

    };

}

#undef CapIncrease

#endif
