# Type-Safe Unions — Notes

## A. Variant Fundamentals

`std::variant<Ts...>` (header `<variant>`, C++17) is a type-safe discriminated
union. At any point in time it holds **exactly one** value whose type is one of
the listed alternatives `Ts...`.

Key properties:

- The variant **knows its active index** — an integer in `[0, sizeof...(Ts))`.
- Attempting to access the wrong alternative is a well-defined error (throws
  `std::bad_variant_access` or returns `nullptr`), never undefined behaviour.
- A default-constructed variant holds a value-initialised instance of the
  **first** alternative. If the first type is not default-constructible, the
  variant itself is not default-constructible (unless you use
  `std::monostate` as the first alternative).
- `std::variant` is a **value type**: it can be copied, moved, compared, and
  stored in containers just like any other value, provided every alternative
  supports the operation.
- Unlike polymorphic class hierarchies, a variant's set of alternatives is
  **closed** — it is fixed at compile time.

```cpp
#include <variant>
#include <string>

std::variant<int, double, std::string> v = 42;  // active: int
v = 3.14;                                        // active: double
v = "hello";                                     // active: std::string
```

### `std::monostate`

`std::monostate` is an empty struct provided by `<variant>`. Place it as the
first alternative when none of the real alternatives are default-constructible:

```cpp
struct NoDefault { NoDefault(int) {} };
std::variant<std::monostate, NoDefault> v;  // OK — holds monostate
```

---

## B. Accessing the Active Alternative

### `std::get<T>(v)` / `std::get<I>(v)`

Returns a **reference** to the stored value. Throws `std::bad_variant_access`
if the active alternative does not match.

```cpp
std::variant<int, std::string> v = 10;
int& i = std::get<int>(v);          // OK
int& j = std::get<0>(v);            // OK — same thing, by index
// std::get<std::string>(v);        // throws bad_variant_access
```

### `std::get_if<T>(&v)` / `std::get_if<I>(&v)`

Returns a **pointer** to the stored value, or `nullptr` if the alternative
does not match. Never throws.

```cpp
if (auto* p = std::get_if<int>(&v)) {
    // *p is the int
}
```

### `std::holds_alternative<T>(v)`

Returns `true` if the variant currently holds type `T`.

```cpp
if (std::holds_alternative<int>(v)) { /* ... */ }
```

### `v.index()`

Returns the zero-based index of the active alternative.

---

## C. `std::visit` and the Visitor Pattern

`std::visit` applies a callable to the active alternative of one or more
variants. The callable must accept **every** alternative — the compiler
enforces exhaustiveness at compile time.

```cpp
std::variant<int, double, std::string> v = "hello";

std::visit([](auto&& arg) {
    std::cout << arg << "\n";
}, v);
```

### The Overload Pattern

In C++17 you can build an ad-hoc visitor by inheriting from multiple lambdas:

```cpp
template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
```

In C++20 the deduction guide is no longer needed (CTAD handles it):

```cpp
template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
```

Usage:

```cpp
std::visit(overloaded{
    [](int i)                { std::cout << "int: " << i << "\n"; },
    [](double d)             { std::cout << "double: " << d << "\n"; },
    [](const std::string& s) { std::cout << "string: " << s << "\n"; },
}, v);
```

### Named Visitor Structs

For complex visitors — especially recursive ones — a named struct with
multiple `operator()` overloads is often cleaner:

```cpp
struct PrintVisitor {
    void operator()(int i) const { std::cout << i; }
    void operator()(double d) const { std::cout << d; }
    void operator()(const std::string& s) const { std::cout << s; }
};

std::visit(PrintVisitor{}, v);
```

---

## D. Modelling ASTs and Sum Types with Variant

A **sum type** (also called a tagged union or discriminated union) represents a
value that can be one of several distinct forms. `std::variant` maps directly
to this concept.

Example — a tiny expression AST:

```cpp
struct IntegerLiteral { int value; };
struct Add             { std::unique_ptr<Node> left, right; };
struct Mul             { std::unique_ptr<Node> left, right; };

struct Node {
    std::variant<IntegerLiteral, Add, Mul> data;
};
```

Because `Add` and `Mul` contain `std::unique_ptr<Node>`, the tree can be
arbitrarily deep without slicing or virtual dispatch. Evaluation is a simple
recursive `std::visit`:

```cpp
struct EvalVisitor {
    int operator()(const IntegerLiteral& lit) const { return lit.value; }
    int operator()(const Add& a) const {
        return evaluate(*a.left) + evaluate(*a.right);
    }
    int operator()(const Mul& m) const {
        return evaluate(*m.left) * evaluate(*m.right);
    }
};

int evaluate(const Node& n) {
    return std::visit(EvalVisitor{}, n.data);
}
```

Advantages over a virtual-function hierarchy:

- The set of node types is visible in one place (the variant parameter list).
- Adding a **new operation** (a new visitor) does not require touching every
  node class — the opposite trade-off from the classic OOP approach.
- No heap allocation for the variant itself (the node kinds live inline).

Trade-off: adding a **new node kind** requires updating every visitor.

---

## E. Error Handling with Variants (Preview)

`std::variant` can model success-or-error returns:

```cpp
std::variant<ResultType, ErrorType> do_something();
```

This pattern is so common that C++23 formalised it as `std::expected<T, E>`,
which you will study on **Day 14**. For now, be aware that variant-based error
handling avoids exceptions while retaining type safety.

---

## F. Self-Check Questions

Test your understanding — try to answer without looking at the notes above.

1. What happens if you call `std::get<T>` on a variant that does not currently
   hold type `T`?
2. How does `std::get_if` differ from `std::get` in its error-reporting
   mechanism?
3. What is `std::monostate` and when would you use it as a variant
   alternative?
4. Why does the compiler reject a `std::visit` call whose callable does not
   handle every alternative?
5. Describe the "overload pattern" and explain why the C++17 version needs an
   explicit deduction guide while the C++20 version does not.
6. In the AST example, why must `Add` and `Mul` use `std::unique_ptr<Node>`
   rather than `Node` directly?
7. What is the trade-off between adding new node types versus adding new
   operations when using `std::variant` compared to a virtual-function
   hierarchy?
8. Can a `std::variant` be in a state where it holds no value at all? What is
   the "valueless by exception" state?
9. How would you use `v.index()` together with a `switch` statement instead of
   `std::visit`? Why is `std::visit` generally preferred?
10. If two alternatives in a variant have the same type (e.g.,
    `variant<int, int>`), which access functions still work and which become
    ambiguous?
