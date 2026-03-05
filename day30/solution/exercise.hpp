#pragma once
#include <cstddef>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

class SmallAny {
    struct placeholder {
        virtual ~placeholder() = default;
        virtual void clone_to(void* buf, bool& is_inline) const = 0;
        virtual std::unique_ptr<placeholder> clone_heap() const = 0;
        virtual const std::type_info& type() const noexcept = 0;
    };

    template <typename T>
    struct holder : placeholder {
        T value;
        explicit holder(T v) : value(std::move(v)) {}

        void clone_to(void* buf, bool& dst_inline) const override {
            if constexpr (sizeof(holder<T>) <= buf_size &&
                          alignof(holder<T>) <= buf_align) {
                ::new (buf) holder<T>(value);
                dst_inline = true;
            } else {
                (void)buf;
                dst_inline = false;
            }
        }

        std::unique_ptr<placeholder> clone_heap() const override {
            return std::make_unique<holder<T>>(value);
        }

        const std::type_info& type() const noexcept override {
            return typeid(T);
        }
    };

    static constexpr std::size_t buf_size = 32;
    static constexpr std::size_t buf_align = alignof(std::max_align_t);

    alignas(buf_align) unsigned char storage_[buf_size]{};
    placeholder* ptr_ = nullptr;
    bool inline_ = false;

    template <typename T>
    static constexpr bool fits_inline =
        (sizeof(holder<T>) <= buf_size) && (alignof(holder<T>) <= buf_align);

    void destroy() noexcept {
        if (ptr_) {
            if (inline_) {
                ptr_->~placeholder();
            } else {
                delete ptr_;
            }
            ptr_ = nullptr;
            inline_ = false;
        }
    }

    void copy_from(const SmallAny& other) {
        if (!other.ptr_) return;
        if (other.inline_) {
            other.ptr_->clone_to(storage_, inline_);
            ptr_ = reinterpret_cast<placeholder*>(storage_);
        } else {
            auto heap = other.ptr_->clone_heap();
            ptr_ = heap.release();
            inline_ = false;
        }
    }

    void move_from(SmallAny& other) noexcept {
        if (!other.ptr_) return;
        if (other.inline_) {
            other.ptr_->clone_to(storage_, inline_);
            ptr_ = reinterpret_cast<placeholder*>(storage_);
            other.destroy();
        } else {
            ptr_ = other.ptr_;
            inline_ = false;
            other.ptr_ = nullptr;
            other.inline_ = false;
        }
    }

    template <typename T>
    friend T* any_cast(SmallAny* a) noexcept;
    template <typename T>
    friend const T* any_cast(const SmallAny* a) noexcept;

public:
    SmallAny() noexcept = default;

    template <typename T>
    SmallAny(T value) { // NOLINT — intentionally implicit
        using HT = holder<std::decay_t<T>>;
        if constexpr (fits_inline<std::decay_t<T>>) {
            auto* p = ::new (storage_) HT(std::move(value));
            ptr_ = p;
            inline_ = true;
        } else {
            ptr_ = new HT(std::move(value));
            inline_ = false;
        }
    }

    SmallAny(const SmallAny& other) { copy_from(other); }
    SmallAny(SmallAny&& other) noexcept { move_from(other); }

    SmallAny& operator=(const SmallAny& other) {
        if (this != &other) {
            destroy();
            copy_from(other);
        }
        return *this;
    }

    SmallAny& operator=(SmallAny&& other) noexcept {
        if (this != &other) {
            destroy();
            move_from(other);
        }
        return *this;
    }

    ~SmallAny() { destroy(); }

    bool has_value() const noexcept { return ptr_ != nullptr; }

    const std::type_info& type() const noexcept {
        return ptr_ ? ptr_->type() : typeid(void);
    }

    void reset() noexcept { destroy(); }

    bool is_inline() const noexcept { return inline_; }
};

template <typename T>
T* any_cast(SmallAny* a) noexcept {
    if (!a || !a->ptr_ || a->ptr_->type() != typeid(T)) return nullptr;
    return &static_cast<SmallAny::holder<T>*>(a->ptr_)->value;
}

template <typename T>
const T* any_cast(const SmallAny* a) noexcept {
    if (!a || !a->ptr_ || a->ptr_->type() != typeid(T)) return nullptr;
    return &static_cast<const SmallAny::holder<T>*>(a->ptr_)->value;
}

template <typename T>
T any_cast(SmallAny& a) {
    auto* p = any_cast<T>(&a);
    if (!p) throw std::bad_cast();
    return *p;
}

template <typename T>
T any_cast(const SmallAny& a) {
    auto* p = any_cast<T>(&a);
    if (!p) throw std::bad_cast();
    return *p;
}
