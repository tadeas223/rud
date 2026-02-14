#ifndef RUD_DS_DLIST_HPP
#define RUD_DS_DLIST_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/ds/dnode.hpp"
#include "rud/base/memory.hpp"

namespace rud::ds {
    template<typename T>
    struct DList {
        DNode<T>* p_head;
        DNode<T>* p_tail;
        u32 p_len;

        static DList<T> make() {
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
            
            p_head->prev = new_node;
            new_node->next = p_head;
            p_head = new_node;

            if(p_tail == nullptr) {
                p_tail = new_node;
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
            p_tail->next = nullptr;
            
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

        T* peek() const {
            Assert(p_tail != nullptr, Lit("cannot peek into an empty list"));

            return &p_tail->value;
        }

        T* peek_front() const {
            Assert(p_head != nullptr, Lit("cannot peek into an empty list"));

            return &p_head->value;
        }

        const T* get(u32 index) const {
            Assert(index < p_len, Lit("index is outside of the list"));

            DNode<T>* iter_node = p_head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            return &iter_node->value;
        }

        void set(u32 index, T value) {
            Assert(index < p_len, Lit("index is outside of the list"));

            DNode<T>* iter_node = p_head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            iter_node->value = value;
        }

        inline u32 len() const {
            return p_len;
        }

        const T* operator[](u32 index) const {
            return get(index);
        }
        
        LinearView<T> to_linear_view() const {
            LinearView<T> view;

            view.ctx = this;

            view.get_func = [](void* ctx, u32 index) {
                DList<T>* self = static_cast<DList<T>*>(ctx);
                return self->get(index);
            };
            view.set_func = [] (void* ctx, u32 index, T value) { 
                DList<T>* self = static_cast<DList<T>*>(ctx);
                self->set(index, value); 
            };
            view.len_func = [] (void* ctx) {
                DList<T>* self = static_cast<DList<T>*>(ctx);
                return self->len(); 
            };

            return view;
        }
    };
}

#endif
