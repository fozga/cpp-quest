#pragma once

#include <memory>
#include <vector>
#include <cstddef>
#include <new>
#include <cstdint>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Node
// ---------------------------------------------------------------------------

struct Node {
    int value;
    explicit Node(int v);
};

// ---------------------------------------------------------------------------
// SimplePool — fixed-slot arena with a free list
// ---------------------------------------------------------------------------

class SimplePool {
public:
    SimplePool(std::size_t capacity, std::size_t slot_size);

    void* allocate(std::size_t bytes);
    void  deallocate(void* ptr, std::size_t bytes) noexcept;

    std::size_t allocation_count() const noexcept;

private:
    std::size_t              slot_size_;
    std::size_t              capacity_;
    std::vector<std::byte>   storage_;
    void*                    free_head_;
    std::size_t              alloc_count_;

    void init_free_list();
};

// ---------------------------------------------------------------------------
// PoolAllocator<T> — minimal C++20 allocator backed by a SimplePool
// ---------------------------------------------------------------------------

template <typename T>
class PoolAllocator {
public:
    using value_type = T;

    explicit PoolAllocator(SimplePool& pool) noexcept : pool_(&pool) {}

    template <typename U>
    PoolAllocator(const PoolAllocator<U>& other) noexcept : pool_(other.pool_) {}

    T* allocate(std::size_t n) {
        return static_cast<T*>(pool_->allocate(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) noexcept {
        pool_->deallocate(p, n * sizeof(T));
    }

    template <typename U>
    struct rebind {
        using other = PoolAllocator<U>;
    };

    template <typename U>
    friend bool operator==(const PoolAllocator& lhs,
                           const PoolAllocator<U>& rhs) noexcept {
        return lhs.pool_ == rhs.pool_;
    }

    template <typename U>
    friend bool operator!=(const PoolAllocator& lhs,
                           const PoolAllocator<U>& rhs) noexcept {
        return !(lhs == rhs);
    }

    // Pool pointer is public so the rebind constructor can access it.
    SimplePool* pool_;
};

// ---------------------------------------------------------------------------
// Factory function
// ---------------------------------------------------------------------------

std::vector<std::shared_ptr<Node>> make_nodes_with_pool(std::size_t count,
                                                        SimplePool& pool);
