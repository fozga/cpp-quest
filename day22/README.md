# C++20 Concepts

## Day Overview

C++20 **Concepts** let you express template requirements directly in the language,
replacing the old SFINAE / `std::enable_if` idiom with clear, readable
constraints. When a type fails to satisfy a concept the compiler produces a
concise diagnostic that names the unsatisfied requirement instead of pages of
substitution-failure noise.

## Learning Objectives

After completing this day you will be able to:

- Define a concept using the `concept` keyword and a compile-time predicate.
- Use a concept as a template parameter constraint (`template <Numeric T>`).
- Apply `requires` clauses to further constrain templates.
- Combine standard type traits with concepts to restrict templates to numeric types.
- Implement a generic `Matrix<T>` class template constrained by a `Numeric` concept.
- Provide element access via `operator()` and element-wise addition via `operator+`.

## Task Summary

Implement a `Numeric` concept that admits only arithmetic types, then build a
`Matrix<T>` class template constrained by that concept.  The matrix stores its
data in a flat `std::vector<T>`, exposes `rows()`, `cols()`, `operator()` for
element access, and supports element-wise addition via a free `operator+`.

## Implementation Requirements

```cpp
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols, T init = T{});

    std::size_t rows() const noexcept;
    std::size_t cols() const noexcept;

    T& operator()(std::size_t r, std::size_t c);
    const T& operator()(std::size_t r, std::size_t c) const;
};

template <Numeric T>
Matrix<T> operator+(const Matrix<T>& a, const Matrix<T>& b);
```

- `operator+` throws `std::invalid_argument` if the matrices have different dimensions.
- All template definitions live in the header (`exercise.hpp`).
- `exercise.cpp` only includes the header.

## How to Run

From the repository root:

```bash
make day=22 test       # build and run tests
make day=22 asan       # AddressSanitizer build
make day=22 valgrind   # Valgrind memcheck
make day=22 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` for the theory behind concepts and `requires` clauses.
2. Open `solution/exercise.hpp` and study the `Numeric` concept and `Matrix` template.
3. Try instantiating `Matrix<std::string>` and observe the clear concept-violation diagnostic.
4. Run the tests to confirm integer and floating-point matrices work correctly.
5. Review `materials/links.md` for deeper reading.
