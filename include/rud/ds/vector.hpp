#ifndef RUD_DS_VECTOR_HPP
#define RUD_DS_VECTOR_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/memory.hpp"
#include "rud/base/types.hpp"
#include "rud/ds/linear_view.hpp"
namespace rud::ds {
    template<typename T>
    struct Vector {
        T* p_data;
        u32 p_len;
        u32 p_cap;

        static Vector make() {
            T* data = static_cast<T*>(allocate_size(sizeof(T) * 2));
            return {data, 0, 2};
        }

        void destroy() {
            deallocate(p_data);
        }

        void push(T value) {
            if(p_len + 1 > p_cap) {
                resize(p_cap * 2);
            }

            p_data[p_len] = value;
            p_len++;
        }

        void push_front(T value) {
            if(p_len + 1 > p_cap) {
                resize(p_cap * 2);
            }

            mem_move(p_data + 1, p_data, p_len);
            p_data[0] = value;
            
            p_len++;
        
        }

        T pop() {
            Assert(p_len != 0, Lit("cannot pop from an empty vector"));
            p_len--;
            return p_data[p_len];
        }

        T pop_front() {
            Assert(p_len != 0, Lit("cannot pop from an empty vector"));
            
            T value = p_data[0];
            p_len--;
            mem_move(p_data, p_data + 1, p_len);
            return value;
        }

        void set(u32 index, T value) {
            Assert(index < p_len, Lit("index outside of a vector"));

            p_data[index] = value;
        }

        T remove(u32 index) {
            Assert(index < p_len, Lit("index outside of a vector"));
            
            T value = p_data[index];
            mem_move(p_data + index, p_data + index - 1, p_len - index - 1);
            p_len--;
            return value;
        }

        void clear() {
            p_len = 0;
        }


        T* peek() const {
            Assert(p_len != 0, Lit("cannot peek empty vector"));
            return &p_data[p_len-1];
        }

        T* peek_front() const {
            Assert(p_len != 0, Lit("cannot peek empty vector"));
            return &p_data[0];
        }

        const T* get(u32 index) const {
            return &p_data[index];
        }

        u32 len() const {
            return p_len;
        }

        u32 cap() const {
            return p_cap;
        }

        void resize(u32 new_capacity) {
            Assert(new_capacity > p_len, Lit("Vector::resize would be less than its current length"));

            p_data = static_cast<T*>(reallocate(p_data, sizeof(T) * new_capacity));
            p_cap = new_capacity;
        }
        
        const T* operator[](u32 index) const {
            return get(index);
        }
        
        LinearView<T> to_linear_view() const {
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
                return self->len(); 
            };

            return view;
        }
    };
}

#endif
