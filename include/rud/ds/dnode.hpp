#ifndef RUD_DS_DNODE_HPP
#define RUD_DS_DNODE_HPP

namespace rud {
    template<typename T>
    struct DNode {
        T value;
        DNode* next;
        DNode* prev;

        static DNode<T> make(T value) {
            return {value, nullptr};
        }
        
        static DNode<T> make(T value, DNode* next) {
            return {value, next};
        }
        
        static DNode<T> make(T value, DNode* next, DNode* prev) {
            return {value, next, prev};
        }
    };
    
}

#endif
