#pragma once

#include <cstddef>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <vector>

template <typename T>
class RingBuffer {
public:
    explicit RingBuffer(std::size_t cap) : data_(cap), head_(0), size_(0) {}

    void push_back(const T& value) {
        if (size_ < data_.size()) {
            data_[(head_ + size_) % data_.size()] = value;
            ++size_;
        } else {
            data_[head_] = value;
            head_ = (head_ + 1) % data_.size();
        }
    }

    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    std::size_t capacity() const noexcept { return data_.size(); }

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator() = default;

        reference operator*() const { return buffer_->data_[buffer_->to_physical(logical_)]; }
        pointer operator->() const { return &(**this); }

        iterator& operator++() { ++logical_; return *this; }
        iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

        bool operator==(const iterator& o) const { return buffer_ == o.buffer_ && logical_ == o.logical_; }
        bool operator!=(const iterator& o) const { return !(*this == o); }

    private:
        friend class RingBuffer;
        iterator(RingBuffer* buf, std::size_t logical) : buffer_(buf), logical_(logical) {}
        RingBuffer* buffer_ = nullptr;
        std::size_t logical_ = 0;
    };

    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size_); }

private:
    std::vector<T> data_;
    std::size_t head_;
    std::size_t size_;

    std::size_t to_physical(std::size_t logical) const noexcept {
        return (head_ + logical) % data_.size();
    }
};
