#ifndef RUD_DS_VECTOR_HPP
#define RUD_DS_VECTOR_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/memory.hpp"
#include "rud/base/types.hpp"
#include "rud/ds/linear_view.hpp"
namespace rud::ds {
    template<typename T>
    struct Vector {
        T* data;
        u32 len;
        u32 cap;

        static Vector make() {
            T* data = static_cast<T*>(allocate_size(sizeof(T) * 2));
            return {data, 0, 2};
        }
        
        static Vector make(u32 cap) {
            T* data = static_cast<T*>(allocate_size(sizeof(T) * cap));
            return {data, 0, cap};
        }

        void destroy() {
            deallocate(data);
        }

        T* destroy_to_array() {
            return data;
        }

        void push(T value) {
            if(len + 1 > cap) {
                resize(cap * 2);
            }

            data[len] = value;
            len++;
        }

        void push_front(T value) {
            if(len + 1 > cap) {
                resize(cap * 2);
            }

            mem_move(data + 1, data, len);
            data[0] = value;
            
            len++;
        
        }

        T pop() {
            Assert(len != 0, Lit("cannot pop from an empty vector"));
            len--;
            return data[len];
        }

        T pop_front() {
            Assert(len != 0, Lit("cannot pop from an empty vector"));
            
            T value = data[0];
            len--;
            mem_move(data, data + 1, len);
            return value;
        }

        void set(u32 index, T value) {
            Assert(index < len, Lit("index outside of a vector"));

            data[index] = value;
        }

        T remove(u32 index) {
            Assert(index < len, Lit("index outside of a vector"));
            
            T value = data[index];
            mem_move(data + index, data + index - 1, len - index - 1);
            len--;
            return value;
        }

        void clear() {
            len = 0;
        }


        T* peek() const {
            Assert(len != 0, Lit("cannot peek empty vector"));
            return &data[len-1];
        }

        T* peek_front() const {
            Assert(len != 0, Lit("cannot peek empty vector"));
            return &data[0];
        }

        const T* get(u32 index) const {
            return &data[index];
        }

        void resize(u32 new_capacity) {
            Assert(new_capacity > len, Lit("Vector::resize would be less than its current length"));

            data = static_cast<T*>(reallocate(data, sizeof(T) * new_capacity));
            cap = new_capacity;
        }
        
        const T* operator[](u32 index) const {
            return get(index);
        }
        
        LinearView<T> to_linear_view() {
            LinearView<T> view;

            view.ctx = this;

            view.get_func = [](void* ctx, u32 index) {
                Vector<T>* self = static_cast<Vector<T>*>(ctx);
                return self->get(index);
            };
            view.set_func = [] (void* ctx, u32 index, T value) { 
                Vector<T>* self = static_cast<Vector<T>*>(ctx);
                self->set(index, value); 
            };
            view.len_func = [] (void* ctx) {
                Vector<T>* self = static_cast<Vector<T>*>(ctx);
                return self->len;
            };

            return view;
        }
    };
}

#endif
