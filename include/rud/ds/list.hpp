#ifndef RUD_DS_LIST_HPP
#define RUD_DS_LIST_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/ds/node.hpp"
#include "rud/memory.hpp"

namespace rud::ds {
    template<typename T>
    struct List {
        Node<T>* p_head;
        Node<T>* p_tail;
        u32 p_len;

        static List<T> make() {
            return {.p_head = nullptr, .p_tail = nullptr, .p_len = 0};
        }

        void push(T value) {
            Node<T>* new_node = allocate<Node<T>>(Node<T>::make(value));

            if(p_head == nullptr) {
                p_head = new_node;
                p_tail = new_node;
            } else {
                p_tail->next = new_node;
                p_tail = new_node;
            }

            p_len++;
        }

        void push_front(T value) {
            Node<T>* new_node = allocate<Node<T>>(Node<T>::make(value));
            
            new_node->next = p_head;
            p_head = new_node;

            if(p_tail == nullptr) {
                p_tail = new_node;
            }
            
            p_len++;
        }

        T pop() {
            Assert(p_tail != nullptr, Lit("cannot pop from an empty list").temp());
            
            T value = p_tail->value;
            
            if(p_len == 1) {
                deallocate(p_head);
                p_len = 0;
                p_head = nullptr;
                p_tail = nullptr;
                return value;
            }

            Node<T>* iter_node = p_head;
            while(iter_node->next != p_tail) {
                iter_node = iter_node->next;
            }
            
            deallocate(iter_node->next);
            
            p_tail = iter_node;
            iter_node->next = nullptr;
        
            p_len--;
            return value;
        }

        T pop_front() {
            Assert(p_head != nullptr, Lit("cannot pop from an empty list").temp());

            T value = p_head->value;
            if(p_len == 1) {
                deallocate(p_head);
                p_len = 0;
                p_head = nullptr;
                p_tail = nullptr;
                return value;
            }
            
            Node<T>* remove_node = p_head;
            p_head = p_head->next;
            deallocate(remove_node);

            p_len--;

            return value;
        }

        T* peek() {
            Assert(p_tail != nullptr, Lit("cannot peek into an empty list").temp());

            return &p_tail->value;
        }

        T* peek_front() {
            Assert(p_head != nullptr, Lit("cannot peek into an empty list").temp());

            return &p_head->value;
        }

        const T* get(u32 index) {
            Assert(index < p_len, Lit("index is outside of the list").temp());

            Node<T>* iter_node = p_head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            return &iter_node->value;
        }

        T* set(u32 index) {
            Assert(index < p_len, Lit("index is outside of the list").temp());

            Node<T>* iter_node = p_head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            return &iter_node->value;
        }

        T remove(u32 index) {
            Assert(index < p_len, Lit("index is outside of the list").temp());

            Node<T>* iter_node = p_head;
            for(u32 i = 0; i < index-1; i++) {
                iter_node = iter_node->next;
            }
            
            Node<T>* remove_node = iter_node->next;
            
            T value = remove_node->value;
            
            iter_node->next = remove_node->next;
            deallocate(remove_node);
            
            p_len--;

            return value;
        }

        void clear() {
            Node<T>* node = p_head;
            while(node != nullptr) {
                Node<T>* remove_node = node;
                node = node->next;
                deallocate(remove_node);
            }
            p_head = nullptr;
            p_tail = nullptr;
            p_len = 0;
        }

        inline u32 len() {
            return p_len;
        }

        void destroy() {
            Node<T>* node = p_head;
            while(node != nullptr) {
                Node<T>* remove_node = node;
                node = node->next;
                deallocate(remove_node);
            }
        }
        
        T* operator[](u32 index) {
            return set(index);
        }
        
        const T* operator[](u32 index) const {
            return get(index);
        }
        
        LinearView<T> to_linear_view() {
            LinearView<T> view;

            view.ctx = this;

            view.get_func = [](void* ctx, u32 index) {
                List<T>* self = static_cast<List<T>*>(ctx);
                return self->get(index);
            };
            view.set_func = [] (void* ctx, u32 index) { 
                List<T>* self = static_cast<List<T>*>(ctx);
                return self->set(index); 
            };
            view.len_func = [] (void* ctx) {
                List<T>* self = static_cast<List<T>*>(ctx);
                return self->len(); 
            };

            return view;
        }
    };
}

#endif
