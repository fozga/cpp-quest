#include "exercise.hpp"

// ---------------------------------------------------------------------------
// Node
// ---------------------------------------------------------------------------

Node::Node(int v) : value(v) {}

// ---------------------------------------------------------------------------
// SimplePool
// ---------------------------------------------------------------------------

SimplePool::SimplePool(std::size_t capacity, std::size_t slot_size)
    : slot_size_(slot_size),
      capacity_(capacity),
      storage_(capacity * slot_size),
      free_head_(nullptr),
      alloc_count_(0)
{
    init_free_list();
}

void SimplePool::init_free_list() {
    free_head_ = nullptr;
    for (std::size_t i = capacity_; i > 0; --i) {
        void* slot = storage_.data() + (i - 1) * slot_size_;
        *static_cast<void**>(slot) = free_head_;
        free_head_ = slot;
    }
}

void* SimplePool::allocate(std::size_t bytes) {
    if (bytes > slot_size_ || free_head_ == nullptr) {
        throw std::bad_alloc();
    }
    void* slot = free_head_;
    free_head_ = *static_cast<void**>(free_head_);
    ++alloc_count_;
    return slot;
}

void SimplePool::deallocate(void* ptr, std::size_t /*bytes*/) noexcept {
    if (ptr == nullptr) return;
    *static_cast<void**>(ptr) = free_head_;
    free_head_ = ptr;
    --alloc_count_;
}

std::size_t SimplePool::allocation_count() const noexcept {
    return alloc_count_;
}

// ---------------------------------------------------------------------------
// make_nodes_with_pool
// ---------------------------------------------------------------------------

std::vector<std::shared_ptr<Node>> make_nodes_with_pool(std::size_t count,
                                                        SimplePool& pool) {
    PoolAllocator<Node> alloc(pool);
    std::vector<std::shared_ptr<Node>> nodes;
    nodes.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        nodes.push_back(std::allocate_shared<Node>(alloc, static_cast<int>(i)));
    }
    return nodes;
}
