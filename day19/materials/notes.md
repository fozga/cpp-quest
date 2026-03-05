# Type Traits — Notes

## A. Type Traits and `<type_traits>`

A type trait is a class template that exposes a compile-time constant describing a property of a type. The standard header `<type_traits>` provides dozens of ready-made traits such as `std::is_integral<T>`, `std::is_floating_point<T>`, `std::is_same<T, U>`, and `std::is_pointer<T>`.

Every standard trait provides a `::value` static member of type `bool`. Since C++17, a corresponding `_v` variable template alias is available for convenience:

```cpp
std::is_integral<int>::value   // true
std::is_integral_v<int>        // true (same thing, shorter)
```

Traits are resolved entirely at compile time — they produce no runtime code and have zero overhead.

## B. Custom Trait Structure

A custom trait follows a simple recipe:

1. **Primary template** — the general case, usually inheriting from `std::false_type`.
2. **Specialization(s)** — one or more specializations that inherit from `std::true_type` for the types that should match.

```cpp
template <typename T>
struct is_my_type : std::false_type {};

template <>
struct is_my_type<MyType> : std::true_type {};
```

`std::true_type` is a typedef for `std::integral_constant<bool, true>`. It provides `::value = true`, an implicit `operator bool()`, and other utilities. `std::false_type` is the corresponding `false` version.

**Partial specialization** allows you to match a family of types rather than one specific type. For example, you can match all pointer types:

```cpp
template <typename T>
struct is_raw_pointer : std::false_type {};

template <typename T>
struct is_raw_pointer<T*> : std::true_type {};
```

## C. String Traits and `std::basic_string`

`std::string` is a typedef for `std::basic_string<char, std::char_traits<char>, std::allocator<char>>`. The full template signature is:

```cpp
template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename Alloc  = std::allocator<CharT>>
class basic_string;
```

To write a trait that matches any `basic_string` instantiation using `char` — regardless of the `Traits` or `Alloc` parameters — use a partial specialization that fixes `CharT` to `char` and leaves the other parameters as template parameters:

```cpp
template <typename Traits, typename Alloc>
struct is_std_string<std::basic_string<char, Traits, Alloc>> : std::true_type {};
```

This matches `std::string` (which uses the default `Traits` and `Alloc`) and any custom-allocator `basic_string<char, ...>`, but rejects `std::wstring` (`basic_string<wchar_t, ...>`) and all non-string types.

## D. `static_assert` for Compile-Time Verification

`static_assert` evaluates a constant expression at compile time and emits a compiler error if the expression is `false`:

```cpp
static_assert(std::is_integral_v<int>, "int must be integral");
static_assert(!std::is_integral_v<double>, "double is not integral");
```

Since C++17, the message string is optional — the compiler will display the failed expression itself. `static_assert` is the primary tool for testing type traits because the check happens during compilation, not at runtime.

## E. Self-Check Questions

1. What is a type trait, and at what stage of compilation is it evaluated?
2. What do `std::true_type` and `std::false_type` provide beyond a `::value` member?
3. How does a `_v` variable template alias relate to the `::value` member of a trait?
4. What is the difference between a full (explicit) specialization and a partial specialization?
5. Why does the primary template of a custom predicate trait typically inherit from `std::false_type`?
6. What is the full template signature of `std::basic_string`, and how does `std::string` relate to it?
7. Why does matching `std::basic_string<char, Traits, Alloc>` as a partial specialization also match `std::string`?
8. What happens if a `static_assert` condition evaluates to `false` — is it a runtime error or a compile-time error?
9. Can `static_assert` appear inside a function body, or only at namespace scope?
10. If you wanted to extend the trait to also match `std::wstring`, what would the additional specialization look like?
