# Perfect Forwarding — Notes

## A. Forwarding (Universal) References

In a **deduced template context**, `T&&` does not mean "rvalue reference to
`T`". It means *forwarding reference* (Scott Meyers's original term was
*universal reference*). The key requirement is that `T` must be deduced by the
compiler for that specific call:

```cpp
template <typename T>
void wrapper(T&& arg);          // forwarding reference

void take_rvalue(Widget&& w);   // plain rvalue reference (Widget is concrete)
```

A forwarding reference can bind to **both** lvalues and rvalues:

- When an lvalue of type `Widget` is passed, `T` is deduced as `Widget&`, and
  reference collapsing turns `Widget& &&` into `Widget&`.
- When an rvalue of type `Widget` is passed, `T` is deduced as `Widget`, and
  `T&&` stays `Widget&&`.

### Reference Collapsing Rules

| Combination | Result |
|---|---|
| `T& &`   | `T&`  |
| `T& &&`  | `T&`  |
| `T&& &`  | `T&`  |
| `T&& &&` | `T&&` |

The only combination that produces an rvalue reference is `&& &&`.

## B. Motivation

Many standard library facilities need to construct or invoke something on
behalf of the caller without knowing (or caring about) the exact argument
types:

- `std::make_unique<T>(args...)` constructs a `T` on the heap.
- `std::make_shared<T>(args...)` does the same with shared ownership.
- `std::vector::emplace_back(args...)` constructs an element in-place.
- `std::invoke(f, args...)` calls any callable uniformly.

Without perfect forwarding these functions would need an exponential number of
overloads (one for each combination of const/non-const, lvalue/rvalue per
parameter). Forwarding references plus `std::forward` reduce this to a single
variadic template.

**Performance benefit:** unnecessary copies are avoided. If the caller passes a
temporary string, that temporary is moved all the way through to the final
destination instead of being copied at each intermediate layer.

**Correctness benefit:** if an argument is an lvalue the caller still owns, it
must *not* be moved from. Perfect forwarding preserves the distinction
automatically.

## C. `std::forward`

`std::forward<T>(arg)` is a conditional cast:

- If `T` is an lvalue reference (`Widget&`), it returns an lvalue reference —
  no move.
- If `T` is a non-reference (`Widget`), it casts `arg` to an rvalue reference
  — enabling a move.

The canonical usage pattern:

```cpp
template <typename T, typename... Args>
T make(Args&&... args) {
    return T(std::forward<Args>(args)...);
}
```

**Never use `std::move` in place of `std::forward`** inside a forwarding
function. `std::move` unconditionally casts to an rvalue, which would move from
lvalue arguments that the caller still needs.

**Never use `std::forward` outside a forwarding context.** It relies on the
deduced `T` carrying the lvalue/rvalue information. If you use it with a
concrete type it degenerates into a regular cast.

## D. Common Pitfalls

### 1. Double Move

Forwarding the same parameter pack more than once in a single expression moves
from an already-moved object the second time:

```cpp
template <typename... Args>
void bad(Args&&... args) {
    foo(std::forward<Args>(args)...);
    bar(std::forward<Args>(args)...);  // BUG: args already moved
}
```

### 2. Forwarding Inside Loops

The same problem arises when forwarding inside a loop body — only the first
iteration is safe:

```cpp
template <typename T>
void bad_loop(T&& val) {
    for (int i = 0; i < 3; ++i)
        sink(std::forward<T>(val));  // BUG on iterations 2+
}
```

### 3. Lambda Capture of Forwarding References

Capturing a forwarding reference by reference in a lambda is fragile. If the
lambda outlives the function call, it holds a dangling reference to the
caller's temporary:

```cpp
template <typename T>
auto bad_capture(T&& val) {
    return [&val]() { use(val); };  // dangling if val was a temporary
}
```

C++20 introduced `std::forward<T>(val)` inside init-captures to move the
value into the lambda when appropriate:

```cpp
template <typename T>
auto good_capture(T&& val) {
    return [v = std::forward<T>(val)]() { use(v); };
}
```

### 4. Lifetime Extension Does Not Cross Function Boundaries

A temporary bound to a forwarding reference parameter has its lifetime extended
only for the duration of the function call. Storing a reference to it beyond
that point is undefined behaviour.

## E. `decltype(auto)` for Return-Type Preservation

When wrapping a callable, the wrapper should return *exactly* what the callable
returns — including references:

```cpp
template <typename F, typename... Args>
decltype(auto) call_wrapper(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}
```

`decltype(auto)` deduces the return type using `decltype` rules:

- If the callable returns `int`, the wrapper returns `int`.
- If the callable returns `int&`, the wrapper returns `int&`.
- If the callable returns `int&&`, the wrapper returns `int&&`.

Using plain `auto` would strip references and top-level cv-qualifiers, silently
changing the wrapper's contract.

## F. Self-Check Questions

1. What distinguishes a forwarding reference from a plain rvalue reference?
2. What are the four reference-collapsing rules, and which combination produces
   an rvalue reference?
3. Why does `std::forward<T>` need the explicit template argument `T`?
4. What goes wrong if you replace `std::forward` with `std::move` in a
   forwarding function that receives lvalue arguments?
5. Why is forwarding the same parameter pack twice in one statement dangerous?
6. How does `decltype(auto)` differ from plain `auto` as a return type?
7. Why can't lifetime extension of temporaries cross a function-call boundary?
8. In `std::make_unique<Widget>(name, 42)`, if `name` is an `std::string`
   lvalue, what value category does the `Widget` constructor see for that
   argument?
9. How would you safely capture a forwarding reference inside a lambda that
   outlives the enclosing function?
10. Why do `emplace`-style functions (e.g., `vector::emplace_back`) typically
    outperform `push`-style functions for non-trivial types?
