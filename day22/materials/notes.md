# C++20 Concepts — Study Notes

## A. What Are Concepts?

A **concept** is a named compile-time predicate on template parameters.  It
evaluates to `true` or `false` during template argument substitution and
controls whether a template is viable.

```cpp
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;
```

- The `concept` keyword introduces the definition.
- The right-hand side is any constant expression that yields `bool`.
- Standard type traits (`<type_traits>`) are the most common building blocks.

Concepts were introduced in **C++20** and are supported by GCC 10+, Clang 10+,
and MSVC 19.28+.

## B. Using Concepts

### Constrained template parameter

Place the concept name where you would normally write `typename`:

```cpp
template <Numeric T>
T add(T a, T b) { return a + b; }
```

### Trailing requires clause

Attach a `requires` clause after the function signature:

```cpp
template <typename T>
T add(T a, T b) requires Numeric<T> { return a + b; }
```

### Inline requires expression

You can also write an ad-hoc constraint without a named concept:

```cpp
template <typename T>
    requires std::is_arithmetic_v<T>
T add(T a, T b) { return a + b; }
```

All three forms are semantically equivalent; the constrained-parameter form is
the most concise for simple constraints.

## C. Benefits over SFINAE

| Aspect | SFINAE / `enable_if` | Concepts |
|--------|----------------------|----------|
| Readability | Verbose, nested templates | Clean, declarative syntax |
| Error messages | Pages of substitution failures | Names the unsatisfied concept |
| Composability | Chaining `enable_if` conditions is awkward | Combine concepts with `&&` and `||` |
| Overload ordering | Manual mutual exclusion required | Compiler ranks more-constrained overloads automatically |

### Example: SFINAE vs Concept

SFINAE style:
```cpp
template <typename T>
std::enable_if_t<std::is_arithmetic_v<T>, T>
square(T x) { return x * x; }
```

Concept style:
```cpp
template <Numeric T>
T square(T x) { return x * x; }
```

The concept version is shorter, easier to read, and produces better diagnostics
when called with a non-arithmetic type.

## D. The Numeric Concept

```cpp
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;
```

`std::is_arithmetic_v<T>` is `true` for all integral types (`int`, `char`,
`bool`, `unsigned long`, etc.) and all floating-point types (`float`, `double`,
`long double`).  It is `false` for pointers, references, enums, classes, and
anything else.

This single trait is sufficient to guard a numeric `Matrix` against accidental
instantiation with incompatible types like `std::string`.

## E. Matrix Design

The `Matrix<T>` class stores elements in a single `std::vector<T>` using
row-major layout.  The element at row `r`, column `c` maps to index
`r * cols_ + c`.

Key design points:

- **Constructor** — Takes `rows`, `cols`, and an optional `init` value; fills
  the vector with `rows * cols` copies of `init`.
- **Element access** — `operator()(r, c)` returns a reference (const and
  non-const overloads) for natural mathematical indexing.
- **Addition** — The free `operator+` creates a new matrix of the same
  dimensions and adds elements pairwise.  It throws
  `std::invalid_argument` if the dimensions do not match.
- **Header-only** — Because `Matrix` is a class template, all definitions must
  be visible at the point of instantiation, so everything lives in the header.

## F. Self-Check Questions

1. What keyword introduces a concept definition, and what must the right-hand side evaluate to?
2. Name three syntactic positions where you can apply a concept constraint to a template.
3. How does the compiler's diagnostic differ when a concept constraint fails versus when SFINAE fails?
4. What does `std::is_arithmetic_v<T>` cover? Is `bool` arithmetic? Is `char`?
5. Why must all `Matrix<T>` member definitions appear in the header rather than in `exercise.cpp`?
6. What happens at compile time if you write `Matrix<std::string> m(2, 2);`?
7. How does the compiler rank two overloads when one is more constrained than the other?
8. Can you combine two concepts with `&&`?  Give an example.
9. What is the difference between a `requires` clause and a `requires` expression?
10. Why does `operator+` for `Matrix` throw an exception instead of using a `static_assert` for dimension checking?
