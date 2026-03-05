# Variadic Templates — Study Notes

## A. Parameter Packs

A **template parameter pack** captures zero or more template arguments:

```cpp
template <typename... Ts>       // Ts is a template parameter pack
void f(const Ts&... args);      // args is a function parameter pack
```

- `typename... Ts` introduces the pack at the template level.
- `const Ts&... args` introduces the corresponding function parameter pack.
- The pack can hold any number of types (including zero).
- You expand a pack by placing `...` after an expression that contains it: `args...` expands to a comma-separated list of the pack elements.

### sizeof...

`sizeof...(Ts)` (or `sizeof...(args)`) returns the number of elements in the pack as a `constexpr std::size_t`. It does **not** evaluate the arguments.

```cpp
template <typename... Ts>
constexpr std::size_t count(const Ts&...) {
    return sizeof...(Ts);
}
// count(1, 'a', 3.14) == 3
```

## B. Fold Expressions (C++17)

Fold expressions reduce a parameter pack over a binary operator in a single expression, without recursion.

### Four forms

| Form | Syntax | Expansion (for pack `e1, e2, e3`) |
|------|--------|-----------------------------------|
| Unary right fold | `(args op ...)` | `e1 op (e2 op e3)` |
| Unary left fold | `(... op args)` | `(e1 op e2) op e3` |
| Binary right fold | `(args op ... op init)` | `e1 op (e2 op (e3 op init))` |
| Binary left fold | `(init op ... op args)` | `((init op e1) op e2) op e3` |

### Stream output with a fold

A left fold over the comma operator is the idiomatic way to perform a side-effecting operation on every pack element:

```cpp
template <typename... Ts>
void print_all(std::ostream& os, const Ts&... args) {
    ((os << args << ' '), ...);   // comma-fold: evaluated left to right
    os << '\n';
}
```

The outer parentheses are required syntax for a fold expression. The comma operator guarantees left-to-right evaluation, so arguments are printed in order.

### Logic folds

```cpp
template <typename... Ts>
bool all_true(Ts... args) {
    return (... && args);    // left fold: ((e1 && e2) && e3)
}

template <typename... Ts>
bool any_true(Ts... args) {
    return (... || args);
}
```

### Arithmetic folds

```cpp
template <typename... Ts>
auto sum(Ts... args) {
    return (... + args);     // left fold over +
}
```

## C. Variadic Logging: Recursion vs Fold

### C++11 recursive approach

Before fold expressions, variadic templates required a base case and a recursive step:

```cpp
// base case
void log(std::ostream& os) {
    os << '\n';
}

// recursive case
template <typename T, typename... Rest>
void log(std::ostream& os, const T& first, const Rest&... rest) {
    os << first << ' ';
    log(os, rest...);   // peel off one argument at a time
}
```

Each call peels off one argument and recurses. The compiler generates N+1 function instantiations for N arguments.

### C++17 fold expression approach

```cpp
template <typename... Ts>
void log(std::ostream& os, const Ts&... args) {
    ((os << args << ' '), ...);
    os << '\n';
}
```

A single function template, no recursion, no base case. The fold expression handles the expansion directly.

## D. Common Pitfalls

1. **Empty packs** — A unary fold over `+` with an empty pack is ill-formed (except for `&&`, `||`, and `,` which have defined values for empty packs: `true`, `false`, and `void()` respectively). Use a binary fold with an explicit init value when empty packs are possible.

2. **Operator precedence in folds** — The expression inside the fold is expanded textually. Wrapping sub-expressions in parentheses avoids surprises. For example, `((os << args << ' '), ...)` groups the stream operations correctly before the comma fold.

3. **Overloading conflicts** — If you provide a non-template overload alongside a variadic template, the non-template overload is preferred when arguments match exactly. This can silently suppress the template version if you're not careful.

4. **Pack expansion context** — Packs can only be expanded in specific contexts: function arguments, template arguments, initialiser lists, fold expressions, and a few others. Attempting to expand a pack in an unsupported context is a compile error.

5. **Forwarding with variadic templates** — When writing a perfect-forwarding variadic function, use `Ts&&... args` with `std::forward<Ts>(args)...`. Forgetting `std::forward` loses rvalue-ness and can cause unnecessary copies.

## E. Self-Check Questions

1. What does `typename... Ts` introduce, and where does the `...` go when you expand the pack?
2. What is the difference between a unary fold and a binary fold?
3. Which operators have defined behaviour for empty packs in a unary fold?
4. Why does the variadic logging example use a comma-fold rather than a left fold over `<<`?
5. How many function instantiations does the C++11 recursive approach generate for `log(os, 1, 2, 3)`?
6. What does `sizeof...(Ts)` return, and is it evaluated at compile time or run time?
7. What happens if you write `(... + args)` and the pack is empty?
8. How would you modify `log_line` to separate arguments with commas instead of spaces?
9. Why must variadic function templates be defined in the header rather than a `.cpp` file?
10. What is the role of `std::forward` in a perfect-forwarding variadic template, and what goes wrong without it?
