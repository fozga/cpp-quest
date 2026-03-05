#pragma once
#include <cstddef>

class Buffer {
public:
    explicit Buffer(std::size_t capacity);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    void push_back(int value);
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    int& operator[](std::size_t index);
    const int& operator[](std::size_t index) const;

private:
    std::size_t capacity_ = 0;
    std::size_t size_ = 0;
    int* data_ = nullptr;
};
