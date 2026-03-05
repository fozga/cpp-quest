#include "exercise.hpp"
#include <cassert>
#include <utility>

// BUG HISTORY: The original version forgot to delete[] data_ in the destructor,
// causing a memory leak detectable by ASan/Valgrind. The corrected version below
// properly releases the allocation.

Buffer::Buffer(std::size_t capacity)
    : capacity_(capacity), size_(0), data_(capacity > 0 ? new int[capacity] : nullptr) {}

Buffer::~Buffer() {
    delete[] data_;
}

Buffer::Buffer(Buffer&& other) noexcept
    : capacity_(other.capacity_), size_(other.size_), data_(other.data_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

void Buffer::push_back(int value) {
    assert(size_ < capacity_);
    data_[size_++] = value;
}

std::size_t Buffer::size() const noexcept { return size_; }
std::size_t Buffer::capacity() const noexcept { return capacity_; }

int& Buffer::operator[](std::size_t index) {
    assert(index < size_);
    return data_[index];
}

const int& Buffer::operator[](std::size_t index) const {
    assert(index < size_);
    return data_[index];
}
