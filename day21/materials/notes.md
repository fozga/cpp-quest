# SFINAE Constraints — Notes

## A. What Is SFINAE?

**Substitution Failure Is Not An Error.**  When the compiler tries to
instantiate a function template and the substitution of template arguments
produces an invalid type *in the immediate context* of the substitution, that
template is simply removed from the overload set instead of causing a hard
compilation error.  Other viable overloads (if any) are still considered.

Key points:

- SFINAE applies only to failures in the *immediate context* — errors inside
  the function body are still hard errors.
- It works during overload resolution, before a function body is ever
  instantiated.
- Without SFINAE many generic-programming idioms would be impossible in C++.

## B. `std::enable_if` and `std::enable_if_t`

`std::enable_if` is a metafunction defined in `<type_traits>`:

```cpp
template <bool Condition, typename T = void>
struct enable_if {};            // primary: no nested type

template <typename T>
struct enable_if<true, T> {
    using type = T;             // partial specialisation for true
};
```

When `Condition` is `false`, `enable_if<false, T>::type` does not exist —
substitution fails, and SFINAE kicks in.

The alias `std::enable_if_t<Cond, T>` is shorthand for
`typename std::enable_if<Cond, T>::type`.

### Common Placement Patterns

1. **Return-type SFINAE** (used in this exercise):

   ```cpp
   template <typename T>
   std::enable_if_t<std::is_integral_v<T>, std::string>
   serialize(T value);
   ```

2. **Extra template parameter (default)**:

   ```cpp
   template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
   std::string serialize(T value);
   ```

3. **Function-parameter SFINAE** (less common):

   ```cpp
   template <typename T>
   std::string serialize(T value,
       std::enable_if_t<std::is_integral_v<T>>* = nullptr);
   ```

All three achieve the same overload-set filtering.

## C. Combining Type Traits with SFINAE

Because `enable_if_t` takes a boolean, any `constexpr bool` expression works:

```cpp
// Accept only arithmetic, non-bool types
template <typename T>
std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>, std::string>
format_number(T value);
```

You can compose traits with `&&`, `||`, and `!` to build arbitrarily precise
constraints.  The type traits from Day 19 (`is_integral_v`, `is_pointer_v`,
`is_same_v`, etc.) are the building blocks.

## D. Mutual Exclusion of Overloads

When writing two overloads differentiated by SFINAE, make sure their conditions
are **mutually exclusive and collectively exhaustive** for the types you intend
to support:

```cpp
// Overload 1: integral
template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::string> serialize(T v);

// Overload 2: everything else
template <typename T>
std::enable_if_t<!std::is_integral_v<T>, std::string> serialize(const T& v);
```

If conditions overlap, the call becomes ambiguous.  If neither condition is
satisfied for a given type, no overload is viable and compilation fails.

## E. Limitations and Readability

SFINAE is powerful but has well-known downsides:

- **Verbose syntax** — `enable_if_t` expressions clutter declarations.
- **Cryptic errors** — when no overload matches, the compiler dumps the entire
  candidate set with substitution-failure details.
- **Immediate-context rule** — failures deeper than the function signature
  (e.g., inside a function body) are hard errors, not SFINAE.

These pain points directly motivated the C++20 **Concepts** feature, which
provides a first-class syntax for constraining templates.  You will explore
Concepts on Days 22–23.

---

## Self-Check Questions

1. What does the acronym SFINAE stand for, and what does the rule say?
2. What happens when `std::enable_if<false, int>::type` is referenced?
3. Name three locations in a template declaration where `enable_if` can be placed.
4. Why must the two `serialize` overloads have complementary `enable_if` conditions?
5. Is `bool` considered an integral type by `std::is_integral_v`?  Why does that matter here?
6. What is the difference between a substitution failure in the *immediate context* and a hard error?
7. How would you constrain a template to accept only floating-point types?
8. Why must template function definitions appear in the header rather than a `.cpp` file?
9. What C++20 feature was introduced to replace most uses of SFINAE?
10. If you accidentally write two overloads whose `enable_if` conditions overlap, what compiler error do you expect?
