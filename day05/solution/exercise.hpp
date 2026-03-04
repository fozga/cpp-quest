#pragma once

#include <cstddef>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

template<typename T>
class DynamicArray {
private:
    T*          data_     = nullptr;
    std::size_t size_     = 0;
    std::size_t capacity_ = 0;

public:
    // -- Default constructor --------------------------------------------------
    DynamicArray() = default;

    // -- Size constructor -----------------------------------------------------
    explicit DynamicArray(std::size_t count)
        : data_{new T[count]()}, size_{count}, capacity_{count}
    {}

    // -- Initializer-list constructor -----------------------------------------
    DynamicArray(std::initializer_list<T> init)
        : data_{new T[init.size()]}, size_{init.size()}, capacity_{init.size()}
    {
        std::copy(init.begin(), init.end(), data_);
    }

    // -- Destructor -----------------------------------------------------------
    ~DynamicArray() {
        delete[] data_;
    }

    // -- Copy constructor -----------------------------------------------------
    DynamicArray(const DynamicArray& other)
        : data_{other.size_ ? new T[other.size_] : nullptr},
          size_{other.size_},
          capacity_{other.size_}
    {
        std::copy(other.data_, other.data_ + other.size_, data_);
    }

    // -- Copy-assignment operator ---------------------------------------------
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            T* new_data = other.size_ ? new T[other.size_] : nullptr;
            std::copy(other.data_, other.data_ + other.size_, new_data);
            delete[] data_;
            data_     = new_data;
            size_     = other.size_;
            capacity_ = other.size_;
        }
        return *this;
    }

    // -- Move constructor -----------------------------------------------------
    DynamicArray(DynamicArray&& other) noexcept
        : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_}
    {
        other.data_     = nullptr;
        other.size_     = 0;
        other.capacity_ = 0;
    }

    // -- Move-assignment operator ---------------------------------------------
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_     = other.data_;
            size_     = other.size_;
            capacity_ = other.capacity_;
            other.data_     = nullptr;
            other.size_     = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // -- Observers ------------------------------------------------------------
    std::size_t size()     const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    bool        empty()    const noexcept { return size_ == 0; }

    // -- Element access -------------------------------------------------------
    T&       operator[](std::size_t i)       { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    // -- push_back (lvalue) ---------------------------------------------------
    void push_back(const T& value) {
        if (size_ == capacity_) {
            std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
            T* new_data = new T[new_cap];
            for (std::size_t i = 0; i < size_; ++i)
                new_data[i] = std::move(data_[i]);
            new_data[size_] = value;
            delete[] data_;
            data_     = new_data;
            capacity_ = new_cap;
        } else {
            data_[size_] = value;
        }
        ++size_;
    }

    // -- push_back (rvalue) ---------------------------------------------------
    void push_back(T&& value) {
        if (size_ == capacity_) {
            std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
            T* new_data = new T[new_cap];
            for (std::size_t i = 0; i < size_; ++i)
                new_data[i] = std::move(data_[i]);
            new_data[size_] = std::move(value);
            delete[] data_;
            data_     = new_data;
            capacity_ = new_cap;
        } else {
            data_[size_] = std::move(value);
        }
        ++size_;
    }
};
