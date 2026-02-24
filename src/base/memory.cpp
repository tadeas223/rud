#include "rud/base/memory.hpp"

#include "rud/base/result.hpp"
#include "rud/base/env.hpp"
#include "rud/base/system.hpp"
#include "rud/os/platform/memory.hpp"
#include <limits>

using namespace rud::os;

namespace rud {
    void mem_copy(void* dest, const void* src, u64 size) {
        while(size--) {
            reinterpret_cast<u8*>(dest)[size] = reinterpret_cast<const u8*>(src)[size];
        }
    }

    void mem_move(void* dest, const void* src, u64 size) {
        if(dest > src) {
            while(size--) {
                reinterpret_cast<u8*>(dest)[size] = reinterpret_cast<const u8*>(src)[size];
            }
        } else {
            for(u32 i = 0; i < size; ++i) {
                reinterpret_cast<u8*>(dest)[i] = reinterpret_cast<const u8*>(src)[i];
            }
        }
    }

    void mem_set(void* dest, u8 value, u64 size) {
        while(size--) {
            reinterpret_cast<u8*>(dest)[size] = value;
        }
    }

    bool mem_equals(const void* ptr1, const void* ptr2, u64 size) {
        while(size--) {
            if(reinterpret_cast<const u8*>(ptr1)[size] != reinterpret_cast<const u8*>(ptr2)[size]) {
                return false;
            }
        }
        return true;
    }
    
    u64 mem_align_forward(u64 value, u64 align) {
        u64 mod = value % align;
        
        if(mod == 0) {
            return value;
        }

        return value + (align - mod);
    }
   
#define COMMIT 4
#define RESERVE 1024
#define ALIGN sizeof(void*)
#define NODE_ALIGN mem_align_forward(sizeof(AllocNode), ALIGN)

    struct AllocNode {
        AllocNode* next;
        AllocNode* prev;
        u64 size;
        bool used;
    };

    struct Allocator {
        AllocNode* zero;
        AllocNode* head;
        AllocNode* tail;
        u64 commit_pos;
        u64 reserve_pos;
        u8* reserve;
    };

    static Allocator allocator = {};
    
    static Result<Allocator, OsAllocError> allocator_make() {
        u64 reserve_size = mem_page() * RESERVE;
        Result<void*, OsAllocError> r_reserve = mem_reserve(reserve_size);
        if(!r_reserve.is_ok()) {
            return Result<Allocator, OsAllocError>::make_error(r_reserve.unwrap_error()); 
        }
        void* reserve = r_reserve.unwrap();
        
        u64 commit_size =  mem_page() * COMMIT;
        Result<void, OsAllocError> r_commit = mem_commit(reserve, commit_size);
        if(!r_commit.is_ok()) {
            return Result<Allocator, OsAllocError>::make_error(r_commit.unwrap_error()); 
        }
        
        AllocNode* zero = static_cast<AllocNode*>(reserve);
        *zero = {
            nullptr,
            nullptr,
            NODE_ALIGN,
            true
        };

        AllocNode* node = reinterpret_cast<AllocNode*>(reinterpret_cast<u8*>(reserve) + NODE_ALIGN);
        *node = {
            nullptr,
            nullptr,
            commit_size,
            false
        };

        return Result<Allocator, OsAllocError>::make_ok({
                zero,
                node,
                node,
                commit_size,
                reserve_size,
                static_cast<u8*>(reserve)
        });
    }

    static Result<void, OsAllocError> allocator_commit(Allocator* allocator) {
        u64 commit_size =  mem_page() * COMMIT;
        Result<void, OsAllocError> r_commit = mem_commit(allocator->reserve + allocator->commit_pos, commit_size);
        if(!r_commit.is_ok()) {
            return Result<void, OsAllocError>::make_error(r_commit.unwrap_error()); 
        }

        AllocNode* new_tail = reinterpret_cast<AllocNode*>(allocator->reserve + allocator->commit_pos);
        *new_tail = {
            nullptr, 
            allocator->tail, 
            commit_size,
            false
        };
        
        allocator->commit_pos += commit_size;
        
        allocator->tail->next = new_tail;
        allocator->tail = new_tail;

        return Result<void, OsAllocError>::make_ok();
    }

    Result<void*, AllocError> try_allocate_size(u64 size) {
        if(allocator.reserve == nullptr) {
            Result<Allocator, OsAllocError> r_allocator = allocator_make();
            if(!r_allocator.is_ok()) {
                return Result<void*, AllocError>::make_error(AllocError::OutOfMemory);
            }

            allocator = r_allocator.unwrap();
        }

        if(size > std::numeric_limits<u64>::max() - ALIGN) {
            return Result<void*, AllocError>::make_error(AllocError::OutOfMemory);
        }
        
        if(size == 0) {
            return Result<void*, AllocError>::make_ok(reinterpret_cast<u8*>(allocator.zero) + NODE_ALIGN);
        }

        size = mem_align_forward(size, ALIGN);
        
        AllocNode* node = allocator.head;

        while(node) {
            if(node->size == NODE_ALIGN + size) {
                if(node->prev != nullptr) {
                    node->prev->next = node->next;
                } else {
                    allocator.head = node->next;
                }
                if(node->next != nullptr) {
                    node->next->prev = node->prev;
                }

                node->used = true;
                node->next = nullptr;
                node->prev = nullptr;

                return Result<void*, AllocError>::make_ok(reinterpret_cast<u8*>(node) + NODE_ALIGN);
            } else if(node->size > NODE_ALIGN + size + ALIGN) {
                AllocNode* new_node = reinterpret_cast<AllocNode*>(reinterpret_cast<u8*>(node) + NODE_ALIGN + size);
                new_node->size = node->size - size - NODE_ALIGN;
                new_node->used = false;
                new_node->next = node->next;
                new_node->prev = node->prev;

                if(node->prev != nullptr) {
                    node->prev->next = new_node;
                } else {
                    allocator.head = new_node;
                }

                if(node->next != nullptr) {
                    node->next->prev = new_node;
                }
                
                if(node == allocator.tail) {
                    allocator.tail = new_node;
                }

                node->used = true;
                node->size = NODE_ALIGN + size;
                node->next = nullptr;
                node->prev = nullptr;
                
                return Result<void*, AllocError>::make_ok(reinterpret_cast<u8*>(node) + NODE_ALIGN);
            }
                
            node = node->next;
        }

        Result<void, OsAllocError> r_commit = allocator_commit(&allocator); 
        if(!r_commit.is_ok()) {
            return Result<void*, AllocError>::make_error(AllocError::OutOfMemory);
        }

        return try_allocate_size(size);
    }
    
    Result<void*, AllocError> try_reallocate(void* ptr, u64 new_size) {
        if(new_size == 0) {
            new_size = 1;
        }

        AllocNode* node = reinterpret_cast<AllocNode*>(reinterpret_cast<u8*>(ptr) - NODE_ALIGN);
        
        Result<void*, AllocError> r_alloc = try_allocate_size(new_size);
        if(!r_alloc.is_ok()) {
            return r_alloc;
        }
        void* new_ptr = r_alloc.unwrap();

        if(new_size >= node->size) {
            mem_copy(new_ptr, ptr, node->size - NODE_ALIGN);
        } else {
            mem_copy(new_ptr, ptr, new_size);
        }

        deallocate(ptr);
        return Result<void*, AllocError>::make_ok(new_ptr);
    }


    void deallocate(void* ptr) {
        Assert(allocator.reserve != nullptr, Lit("deallocate() -> allocate was never called"));
        
        if(ptr == nullptr) {
            panic(Lit("deallocating a null pointer"));
        }

        AllocNode* node = reinterpret_cast<AllocNode*>(reinterpret_cast<u8*>(ptr) - NODE_ALIGN);
        if(node == allocator.zero) {
            return;
        }

        node->used = false;

        AllocNode* iter = allocator.head;
        AllocNode* prev = nullptr;

        while(iter != nullptr && iter < node) {
            prev = iter;
            iter = iter->next;
        }

        node->next = iter;
        node->prev = prev;

        if(iter) {
            iter->prev = node;
        } 

        if(prev) {
            prev->next = node;
        } else {
            allocator.head = node;
        }

        if (node->prev && reinterpret_cast<u8*>(node->prev) + node->prev->size == reinterpret_cast<u8*>(node)) {
            node->prev->size += node->size;
            node->prev->next = node->next;
            if (node->next) {
                node->next->prev = node->prev;
            }
            node = node->prev;
        }

        if (node->next && reinterpret_cast<u8*>(node) + node->size == reinterpret_cast<u8*>(node->next)) {
            node->size += node->next->size;
            node->next = node->next->next;
            if (node->next) {
                node->next->prev = node;
            }

            if(node->next == nullptr) {
                allocator.tail = node;
            }
        }

    }
    
    void* allocate_size(u64 size) {
        Result<void*, AllocError> ptr = try_allocate_size(size);
        if(!ptr.is_ok()) {
            #ifdef EXCEPTIONS_ENABLED
            throw AllocError::OutOfMemory;
            #else
            panic(Lit("allocation failed"));
            #endif
        }

        return ptr.unwrap();
    }
    
    void* reallocate(void* ptr, u64 new_size) {
        Result<void*, AllocError> new_ptr = try_reallocate(ptr, new_size);
        if(!new_ptr.is_ok()) {
            #ifdef EXCEPTIONS_ENABLED
            throw AllocError::OutOfMemory;
            #else
            panic(Lit("allocation failed"));
            #endif
        }

        return new_ptr.unwrap();
    }
}
