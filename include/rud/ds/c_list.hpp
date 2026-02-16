#ifndef RUD_DS_LIST_HPP
#define RUD_DS_LIST_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/ds/node.hpp"
#include "rud/base/memory.hpp"

namespace rud::ds {
    template<typename T>
    struct C_List {
        Node<T>* head;
        Node<T>* tail;
        u32 len;

        static C_List<T> make() {
            return {.head = nullptr, .tail = nullptr, .len = 0};
        }
        
        void destroy() {
            Node<T>* node = head;
            while(node != nullptr) {
                Node<T>* remove_node = node;
                node = node->next;
                deallocate(remove_node);
            }
        }

        void push(T value) {
            Node<T>* new_node = allocate<Node<T>>(Node<T>::make(value));

            if(head == nullptr) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }

            len++;
        }

        void push_front(T value) {
            Node<T>* new_node = allocate<Node<T>>(Node<T>::make(value));
            
            new_node->next = head;
            head = new_node;

            if(tail == nullptr) {
                tail = new_node;
            }
            
            len++;
        }

        T pop() {
            Assert(tail != nullptr, Lit("cannot pop from an empty list"));
            
            T value = tail->value;
            
            if(len == 1) {
                deallocate(head);
                len = 0;
                head = nullptr;
                tail = nullptr;
                return value;
            }

            Node<T>* iter_node = head;
            while(iter_node->next != tail) {
                iter_node = iter_node->next;
            }
            
            deallocate(iter_node->next);
            
            tail = iter_node;
            iter_node->next = nullptr;
        
            len--;
            return value;
        }

        T pop_front() {
            Assert(head != nullptr, Lit("cannot pop from an empty list"));

            T value = head->value;
            if(len == 1) {
                deallocate(head);
                len = 0;
                head = nullptr;
                tail = nullptr;
                return value;
            }
            
            Node<T>* remove_node = head;
            head = head->next;
            deallocate(remove_node);

            len--;

            return value;
        }

        T remove(u32 index) {
            if(index == 0) {
                return pop_front();
            }
            Assert(index < len, Lit("index is outside of the list"));

            Node<T>* iter_node = head;
            for(u32 i = 0; i < index-1; i++) {
                iter_node = iter_node->next;
            }
            
            Node<T>* remove_node = iter_node->next;
            
            T value = remove_node->value;
            
            iter_node->next = remove_node->next;
            deallocate(remove_node);
            
            len--;

            return value;
        }

        void clear() {
            Node<T>* node = head;
            while(node != nullptr) {
                Node<T>* remove_node = node;
                node = node->next;
                deallocate(remove_node);
            }
            head = nullptr;
            tail = nullptr;
            len = 0;
        }

        T* peek() const {
            Assert(tail != nullptr, Lit("cannot peek into an empty list"));

            return &tail->value;
        }

        T* peek_front() const {
            Assert(head != nullptr, Lit("cannot peek into an empty list"));

            return &head->value;
        }

        T* get(u32 index) const {
            Assert(index < len, Lit("index is outside of the list"));

            Node<T>* iter_node = head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            return &iter_node->value;
        }

        void set(u32 index, T value) {
            Assert(index < len, Lit("index is outside of the list"));

            Node<T>* iter_node = head;
            for(u32 i = 0; i < index; i++) {
                iter_node = iter_node->next;
            }

            iter_node->value = value;
        }

        T* operator[](u32 index) const {
            return get(index);
        }
        
        LinearView<T> to_linear_view() const {
            LinearView<T> view;

            view.ctx = this;

            view.get_func = [](void* ctx, u32 index) {
                C_List<T>* self = static_cast<C_List<T>*>(ctx);
                return self->get(index);
            };
            view.set_func = [] (void* ctx, u32 index, T value) { 
                C_List<T>* self = static_cast<C_List<T>*>(ctx);
                self->set(index, value); 
            };
            view.len_func = [] (void* ctx) {
                C_List<T>* self = static_cast<C_List<T>*>(ctx);
                return self->len; 
            };

            return view;
        }
    };
}

#endif
