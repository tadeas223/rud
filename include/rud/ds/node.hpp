#ifndef RUD_DS_NODE_HPP
#define RUD_DS_NODE_HPP

namespace rud {
    template<typename T>
    struct Node {
        T value;
        Node* next;

        static Node<T> make(T value) {
            return {value, nullptr};
        }
        
        static Node<T> make(T value, Node* next) {
            return {value, next};
        }
    };
    
}

#endif
