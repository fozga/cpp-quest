# Type Erasure — Notes

## A. What Is Type Erasure?

Type erasure is a design pattern in which the concrete type of an object is
hidden behind a non-template interface. The consumer interacts with a uniform
API without knowing — or depending on — the actual stored type.

C++ has several well-known examples:

- **`std::function<R(Args...)>`** wraps any callable (lambda, function pointer,
  functor) that matches the signature `R(Args...)`.
- **`std::any`** holds a single value of any copyable type, retrievable via
  `std::any_cast`.
- **`std::shared_ptr<void>`** exploits its type-erased deleter to destroy the
  originally-allocated type, even though the pointer is `void*`.

The pattern converts static (compile-time) polymorphism into dynamic (runtime)
polymorphism without requiring the stored types to share a common base class.

## B. Typical Implementation: Interface + Model

The classic implementation uses two nested types:

1. **`placeholder`** — an abstract base class (the *interface*) with pure
   virtual functions such as `clone()` and `type()`.
2. **`holder<T>`** — a concrete class template (the *model*) that derives from
   `placeholder` and stores a value of type `T`.

```text
┌────────────────────┐
│   Any              │
│  unique_ptr<placeholder>──┐
└────────────────────┘      │
                            ▼
                   ┌─────────────────┐
                   │  placeholder    │  (abstract)
                   │  virtual clone  │
                   │  virtual type   │
                   └────────┬────────┘
                            │
                   ┌────────┴────────┐
                   │  holder<T>      │  (concrete)
                   │  T value        │
                   │  clone() → make │
                   │  type() → typeid│
                   └─────────────────┘
```

The outer class (`Any`) owns a `unique_ptr<placeholder>`. Because the pointer
is to the base, the outer class is not a template — all template machinery is
confined to `holder<T>` and to the templated constructor and `any_cast`.

## C. Core Operations

### Storing a value

The templated constructor creates a `holder<decay_t<T>>` on the heap and stores
it through the `unique_ptr<placeholder>`. `std::decay_t` strips references and
cv-qualifiers so that the holder always owns a plain value.

### Copying

`Any`'s copy constructor calls `placeholder::clone()`, which is overridden in
`holder<T>` to `make_unique<holder<T>>(value)`. This achieves deep-copy
semantics without the outer class knowing `T`.

### Retrieving a value — `any_cast`

`any_cast` is a free-function template. The pointer overload:

1. Checks `a->type() == typeid(T)`.
2. If matched, performs `static_cast<holder<T>*>(ptr.get())` and returns
   `&holder->value`.
3. If mismatched, returns `nullptr`.

The reference overload delegates to the pointer overload and throws
`std::bad_cast` on failure.

## D. Trade-Offs

| Advantage | Disadvantage |
|---|---|
| Types need not share a base class. | Heap allocation for every stored value. |
| Non-template public interface. | Virtual dispatch overhead on clone/type. |
| Easy to add new storable types. | `any_cast` requires exact type match (no implicit conversions). |
| Decouples library code from user types. | RTTI (`typeid`) must be enabled. |

Small-buffer optimisation (SBO) can avoid heap allocation for small types — both
`std::any` and `std::function` implementations commonly apply it — but it adds
significant complexity.

## E. Self-Check Questions

1. What problem does type erasure solve that templates alone cannot?
2. What are the two nested types in the classic type-erasure pattern, and what role does each play?
3. Why does the `Any` class store a `unique_ptr<placeholder>` rather than a `unique_ptr<holder<T>>`?
4. How does `any_cast` determine whether the stored type matches the requested type?
5. What happens if you call `any_cast<double>` on an `Any` that holds an `int`?
6. Why is `std::decay_t` applied to the type parameter in the templated constructor?
7. How does deep-copy work without knowing the concrete type at the call site?
8. What is Small Buffer Optimisation and how does it relate to type erasure?
9. Compare type erasure with classic OOP inheritance — when would you prefer each?
10. Name two Standard Library types that use type erasure internally.
