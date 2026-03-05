#pragma once
#include <vector>
#include <cstddef>
#include <type_traits>
#include <stdexcept>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols, T init = T{})
        : rows_(rows), cols_(cols), data_(rows * cols, init) {}

    std::size_t rows() const noexcept { return rows_; }
    std::size_t cols() const noexcept { return cols_; }

    T& operator()(std::size_t r, std::size_t c) { return data_[r * cols_ + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return data_[r * cols_ + c]; }

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<T> data_;
};

template <Numeric T>
Matrix<T> operator+(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols())
        throw std::invalid_argument("Matrix dimensions mismatch");
    Matrix<T> result(a.rows(), a.cols());
    for (std::size_t r = 0; r < a.rows(); ++r)
        for (std::size_t c = 0; c < a.cols(); ++c)
            result(r, c) = a(r, c) + b(r, c);
    return result;
}
