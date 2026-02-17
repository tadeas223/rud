#ifndef RUD_DS_C_DLIST_HPP
#define RUD_DS_C_DLIST_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/ds/dnode.hpp"
#include "rud/base/memory.hpp"

namespace rud::ds {
    template<typename T>
    struct C_DList {
        DNode<T>* p_head;
        DNode<T>* p_tail;
        u32 p_len;

// get_set {{{ 
        inline const DNode<T>* head() const {
            return p_head;
        }
        inline const DNode<T>* tail() const {
            return p_head;
        }

        inline u32 len() const {
            return p_len;
        }
// }}}

// make_destroy {{{ 
        static C_DList<T> make() {
            return {.p_head = nullptr, .p_tail = nullptr, .p_len = 0};
        }
        
        void destroy() {
            DNode<T>* node = p_head;
            while(node != nullptr) {
                DNode<T>* remove_node = node;
                node = node->next;
                deallocate(remove_node);
            }
        }
        void destroy(void (*destroy_func)(T value)) {
            DNode<T>* node = p_head;
            while(node != nullptr) {
                DNode<T>* remove_node = node;
                node = node->next;

                destroy_func(remove_node->value);
                deallocate(remove_node);
            }
        }
// }}}

        void push(T value) {
            DNode<T>* new_node = allocate<DNode<T>>(DNode<T>::make(value));

            if(p_head == nullptr) {
                p_head = new_node;
                p_tail = new_node;
            } else {
                p_tail->next = new_node;
                new_node->prev = p_tail;
                p_tail = new_node;
            }

            p_len++;
        }

        void push_front(T value) {
            DNode<T>* new_node = allocate<DNode<T>>(DNode<T>::make(value));
            
            if(p_head == nullptr) {
                p_head = new_node;
                p_tail = new_node;
            } else {
                p_head->prev = new_node;
                new_node->next = p_head;
                p_head = new_node;
            }
            
            p_len++;
        }

        T pop() {
            Assert(p_tail != nullptr, Lit("cannot pop from an empty list"));
            
            T value = p_tail->value;
            
            if(p_len == 1) {
                deallocate(p_head);
                p_len = 0;
                p_head = nullptr;
                p_tail = nullptr;
                return value;
            }

            DNode<T>* remove_node = p_tail;

            p_tail = p_tail->prev;

            if(p_tail != nullptr) {
                p_tail->next = nullptr;
            }
            
            deallocate(remove_node);
        
            p_len--;
            return value;
        }

        T pop_front() {
            Assert(p_head != nullptr, Lit("cannot pop from an empty list"));

            T value = p_head->value;
            if(p_len == 1) {
                deallocate(p_head);
                p_len = 0;
                p_head = nullptr;
                p_tail = nullptr;
                return value;
            }
            
            DNode<T>* remove_node = p_head;
            p_head = p_head->next;
            p_head->prev = nullptr;
            deallocate(remove_node);

            p_len--;

            return value;
        }

        T remove(u32 index) {
            Assert(index < p_len, Lit("index is outside of the list"));

            DNode<T>* iter_node = p_head;
            for(u32 i = 0; i < index-1; i++) {
                iter_node = iter_node->next;
            }
            
            DNode<T>* remove_node = iter_node->next;
            
            T value = remove_node->value;
            
            iter_node->next = remove_node->next;
            deallocate(remove_node);
            
            p_len--;

            return value;
        }

        void clear() {
            DNode<T>* node = p_head;
            while(node != nullptr) {
                DNode<T>* remove_node = node;
                node = node->next;
                deallocate(remove_node);
            }
            p_head = nullptr;
            p_tail = nullptr;
            p_len = 0;
        }
        
        void clear(void (*destroy_func)(T value)) {
            DNode<T>* node = p_head;
            while(node != nullptr) {
                DNode<T>* remove_node = node;
                node = node->next;

                destroy_func(remove_node->value);
                deallocate(remove_node);
            }
            p_head = nullptr;
            p_tail = nullptr;
            p_len = 0;
        }

        void set(u32 index, T value) {
            Assert(index < p_len, Lit("index is outside of the list"));

            DNode<T>* iter_node = p_head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            iter_node->value = value;
        }

        T* peek() {
            Assert(p_tail != nullptr, Lit("cannot peek into an empty list"));

            return &p_tail->value;
        }

        T* peek_front() {
            Assert(p_head != nullptr, Lit("cannot peek into an empty list"));

            return &p_head->value;
        }

        T* get(u32 index) {
            Assert(index < p_len, Lit("index is outside of the list"));

            DNode<T>* iter_node = p_head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            return &iter_node->value;
        }

        T* operator[](u32 index) {
            return get(index);
        }
        
        LinearView<T> to_linear_view() {
            return LinearView<T>::make(
                this,
                [] (void* ctx) {
                    C_DList<T>* self = static_cast<C_DList<T>*>(ctx);
                    return self->p_len; 
                },
                [](void* ctx, u32 index) {
                    C_DList<T>* self = static_cast<C_DList<T>*>(ctx);
                    return self->get(index);
                }
            );
        }

    };
}

#endif
