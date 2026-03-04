# Day 06 — Notes: Enforcing RVO

---

## A. Copy Elision Basics

**Copy elision** is a compiler optimization that eliminates unnecessary
copies (and moves) of objects. When certain conditions are met, the
compiler constructs the object directly in its final destination rather
than creating it in one place and copying or moving it to another.

### Returning a Local by Value

```cpp
Widget make_widget() {
    Widget w;       // constructed here
    return w;       // compiler may elide the copy/move
}
Widget x = make_widget();  // x may be constructed in-place
```

Without elision the sequence would be: construct `w`, move-construct the
return value from `w`, move-construct `x` from the return value, destroy
temporaries. With elision, only one construction happens.

### Returning a Temporary (Prvalue)

```cpp
Widget make_widget() {
    return Widget{};  // temporary — guaranteed elision in C++17
}
```

When you return a prvalue (an unnamed temporary), the object is constructed
directly in the caller's storage. Since C++17, this is **mandatory** — the
compiler is not allowed to create a temporary and then copy it.

### The `as-if` Rule

Before C++17, copy elision was permitted but optional. The compiler could
apply it under the **as-if rule** — it was allowed to skip visible
side-effects of copy/move constructors for this specific optimization.
This was unusual because C++ normally forbids optimizations that change
observable behavior.

---

## B. RVO vs NRVO

### RVO — Return Value Optimization (Unnamed)

RVO applies when a function returns a **prvalue** (temporary):

```cpp
std::string get_greeting() {
    return std::string("hello");  // RVO — guaranteed in C++17
}
```

Since C++17, the temporary is **never** materialized. The object is
constructed directly in the caller's memory. This is sometimes called
**mandatory copy elision** or **guaranteed RVO**.

### NRVO — Named Return Value Optimization

NRVO applies when a function returns a **named local variable**:

```cpp
std::string get_greeting() {
    std::string s = "hello";
    return s;  // NRVO — permitted but not guaranteed
}
```

NRVO is **not** mandatory even in C++17. However, every major compiler
(GCC, Clang, MSVC) applies it in practice when:

- There is a single return path (or all paths return the same variable).
- The variable's type matches the function's return type exactly.
- The variable is not a function parameter.

### C++17 Mandatory Elision Rules

C++17 mandates elision in two situations:

1. **Initializing from a prvalue of the same type**: `T x = T(args);` is
   equivalent to `T x(args);`.
2. **Returning a prvalue**: `return T(args);` constructs directly in the
   caller's storage.

Mandatory elision means the copy/move constructor **need not even exist**.
A type with deleted copy and move constructors can still be returned as a
prvalue.

---

## C. Interaction with Move Semantics

### The Compiler's Priority Chain for `return`

When a function returns a local variable by value, the compiler tries the
following in order:

1. **Elision (NRVO)** — construct directly in the caller's storage. No
   constructor call at all.
2. **Implicit move** — if elision is not applied, the compiler treats the
   local variable as an rvalue and attempts a move.
3. **Copy** — if there is no viable move constructor, the compiler falls
   back to copying.

This means that `return local_var;` will **never** copy if a move
constructor exists — even without an explicit `std::move`.

### Designing Cheap-to-Move Types

For NRVO-friendly code, design types that are cheap to move:

- Prefer `std::string`, `std::vector`, and other standard containers that
  have O(1) move operations (pointer/size swap).
- Avoid large inline buffers (e.g., `std::array<char, 4096>`) in
  frequently-returned types — these cannot be moved cheaply.
- Mark move operations `noexcept` so containers and algorithms prefer
  them.

### Pre-C++17 Fallback

Before C++17, if NRVO was not applied, the compiler would fall back to
move, then copy. Code that relied on elision for non-copyable types was
not portable. C++17 made prvalue elision mandatory, closing this gap for
unnamed returns.

---

## D. Non-Copyable Return-by-Value Types

### Deleted Copy, Still Returnable

A class can delete its copy constructor and copy assignment operator yet
still be returned by value:

```cpp
class UniqueResource {
public:
    UniqueResource() = default;
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;
    UniqueResource(UniqueResource&&) noexcept = default;
    UniqueResource& operator=(UniqueResource&&) noexcept = default;
};

UniqueResource make() {
    UniqueResource r;
    return r;  // NRVO or implicit move — no copy needed
}
```

This pattern **enforces** that the compiler uses elision or move. If a
caller accidentally tries to copy, the program will not compile. This is
exactly how `std::unique_ptr` works — you can return one from a function,
but you cannot copy it.

### Factory Functions

Factory functions that return non-copyable objects by value are idiomatic
in modern C++:

```cpp
class QueryBuilder {
public:
    static QueryBuilder from_table(std::string table);
    // copy deleted, move available
};

auto q = QueryBuilder::from_table("users");  // RVO or move — never copy
```

The caller receives the fully constructed object without any overhead, and
the non-copyable constraint documents the ownership semantics.

---

## E. Fluent APIs and Value Semantics

### Method Chaining with `return *this`

A **fluent API** lets callers chain method calls:

```cpp
q.select("id").select("name").where("active = true").limit(10);
```

Each method modifies the object in place and returns a reference to
`*this`:

```cpp
QueryBuilder& select(std::string col) {
    selects_.push_back(std::move(col));
    return *this;
}
```

Returning by **reference** is correct here — each call mutates the same
object and no new object is created.

### Returning a New Value (Immutable Builder Pattern)

An alternative design returns a new object from each method call (value
semantics / immutable builder). In that case, RVO matters more because
each call creates a new object. However, the mutable-reference approach
shown above is more common for builder patterns in C++ because it avoids
unnecessary allocations.

### RVO in Builder Patterns

The typical builder pattern combines both techniques:

1. A **factory** creates the builder (RVO for the initial construction).
2. **Chained methods** modify the builder in place (`return *this` by
   reference).
3. A **build / finalize** method may return the final product by value
   (RVO again).

This gives you the fluency of chaining with the efficiency of elision at
the construction and finalization boundaries.

---

## F. Self-Check Questions

Answer these from memory after studying the material above.

1. What is copy elision, and how does it differ from a regular move?
2. What is the difference between RVO and NRVO?
3. In which C++ standard did mandatory copy elision for prvalues become
   required?
4. When a function returns a named local variable, what is the compiler's
   priority chain (elision → move → copy)?
5. Can a class with a deleted copy constructor be returned by value from a
   function? Under what conditions?
6. Why should move constructors and move assignment operators be marked
   `noexcept`?
7. In a fluent API, should chaining methods return `*this` by reference or
   by value? Why?
8. What happens if you write `return std::move(local);` inside a function
   — does it help or hurt NRVO?
9. Give an example of a standard library type that is non-copyable but
   returnable by value.
10. Why does C++17 mandatory elision mean the copy/move constructor need
    not even exist for prvalue returns?
