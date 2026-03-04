#pragma once
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <utility>

class Any {
public:
    Any() noexcept = default;
    Any(const Any& other) : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}
    Any(Any&& other) noexcept : ptr_(std::move(other.ptr_)) {}

    template <typename T>
    Any(T value) : ptr_(std::make_unique<holder<std::decay_t<T>>>(std::move(value))) {}

    Any& operator=(const Any& other) {
        if (this != &other)
            ptr_ = other.ptr_ ? other.ptr_->clone() : nullptr;
        return *this;
    }

    Any& operator=(Any&& other) noexcept {
        ptr_ = std::move(other.ptr_);
        return *this;
    }

    ~Any() = default;

    bool has_value() const noexcept { return ptr_ != nullptr; }

    const std::type_info& type() const noexcept {
        return ptr_ ? ptr_->type() : typeid(void);
    }

    void reset() noexcept { ptr_.reset(); }

private:
    struct placeholder {
        virtual ~placeholder() = default;
        virtual std::unique_ptr<placeholder> clone() const = 0;
        virtual const std::type_info& type() const noexcept = 0;
    };

    template <typename T>
    struct holder : placeholder {
        T value;
        explicit holder(T v) : value(std::move(v)) {}
        std::unique_ptr<placeholder> clone() const override {
            return std::make_unique<holder<T>>(value);
        }
        const std::type_info& type() const noexcept override {
            return typeid(T);
        }
    };

    std::unique_ptr<placeholder> ptr_;

    template <typename T> friend T* any_cast(Any* a) noexcept;
    template <typename T> friend const T* any_cast(const Any* a) noexcept;
};

template <typename T>
T* any_cast(Any* a) noexcept {
    if (!a || !a->ptr_ || a->ptr_->type() != typeid(T)) return nullptr;
    return &static_cast<Any::holder<T>*>(a->ptr_.get())->value;
}

template <typename T>
const T* any_cast(const Any* a) noexcept {
    if (!a || !a->ptr_ || a->ptr_->type() != typeid(T)) return nullptr;
    return &static_cast<const Any::holder<T>*>(a->ptr_.get())->value;
}

template <typename T>
T any_cast(Any& a) {
    auto* p = any_cast<T>(&a);
    if (!p) throw std::bad_cast();
    return *p;
}

template <typename T>
T any_cast(const Any& a) {
    auto* p = any_cast<T>(&a);
    if (!p) throw std::bad_cast();
    return *p;
}
