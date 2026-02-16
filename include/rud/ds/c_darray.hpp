#ifndef RUD_DS_VECTOR_HPP
#define RUD_DS_VECTOR_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/memory.hpp"
#include "rud/base/types.hpp"
#include "rud/ds/linear_view.hpp"

// #define CAP_INCREASE cap + (cap >> 1)
#define CapIncrease(prev) prev * 2 
namespace rud::ds {
    template<typename T>
    struct C_DArray {
        T* data;
        u32 len;
        u32 cap;

        static inline C_DArray make() {
            T* data = static_cast<T*>(allocate_size(sizeof(T) * 2));
            return {data, 0, 2};
        }
        
        static inline C_DArray make(u32 cap) {
            T* data = static_cast<T*>(allocate_size(sizeof(T) * cap));
            return {data, 0, cap};
        }

        inline void destroy() {
            deallocate(data);
        }

        inline T* destroy_to_array() {
            return data;
        }

        inline void push(T value) {
            if(len == cap) [[unlikely]] {
                resize(CapIncrease(cap));
            }

            data[len] = value;
            len++;
        }

        inline void push_front(T value) { 
            if(len == cap) { 
                resize(CapIncrease(cap)); 
            } 
            mem_move(data + 1, data, len * sizeof(T)); 
            data[0] = value;
            len++;
        }

        inline T* push_uninitialized() {
            if(len == cap) resize(CapIncrease(cap));
            return &data[len++];
        }
        
        inline T* push_front_uninitialized() {
            if(len == cap) {
                resize(CapIncrease(cap));
            }

            mem_move(data + 1, data, len * sizeof(T));
            
            len++;
            return &data[0];
        }

        inline T pop() {
            Assert(len != 0, Lit("cannot pop from an empty vector"));
            len--;
            return data[len];
        }

        inline T pop_front() {
            Assert(len != 0, Lit("cannot pop from an empty vector"));
            
            T value = data[0];
            len--;
            mem_move(data, data + 1, len * sizeof(T));
            return value;
        }

        inline void set(u32 index, T value) {
            Assert(index < len, Lit("index outside of a vector"));

            data[index] = value;
        }

        inline T remove(u32 index) {
            Assert(index < len, Lit("index outside of a vector"));
            
            T value = data[index];
            mem_copy(data + index, 
                    data + index + 1,
                    (len - index - 1) * sizeof(T)
            );

            len--;
            return value;
        }

        inline void clear() {
            len = 0;
        }


        inline T* peek() const {
            Assert(len != 0, Lit("cannot peek empty vector"));
            return &data[len-1];
        }

        inline T* peek_front() const {
            Assert(len != 0, Lit("cannot peek empty vector"));
            return &data[0];
        }

        inline T* get(u32 index) const {
            return &data[index];
        }

        inline void resize(u32 new_capacity) {
            data = static_cast<T*>(reallocate(data, sizeof(T) * new_capacity));
            cap = new_capacity;
        }
        
        inline T* operator[](u32 index) const {
            return get(index);
        }
        
        inline LinearView<T> to_linear_view() {
            LinearView<T> view;

            view.ctx = this;

            view.get_func = [](void* ctx, u32 index) {
                C_DArray<T>* self = static_cast<C_DArray<T>*>(ctx);
                return self->get(index);
            };
            view.set_func = [] (void* ctx, u32 index, T value) { 
                C_DArray<T>* self = static_cast<C_DArray<T>*>(ctx);
                self->set(index, value); 
            };
            view.len_func = [] (void* ctx) {
                C_DArray<T>* self = static_cast<C_DArray<T>*>(ctx);
                return self->len;
            };

            return view;
        }
    };
}

#undef CapIncrease

#endif
