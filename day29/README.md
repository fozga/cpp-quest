# Type Erasure

## Day Overview

Type erasure is a technique that hides concrete types behind a uniform, non-
template interface. Instead of exposing a template parameter to every consumer,
an erased wrapper stores just enough information (typically a virtual-dispatch
table or function pointers) to manipulate the held object at runtime.

The standard library relies on type erasure in several places:

- **`std::any`** — stores a value of any copyable type and retrieves it via
  `any_cast`.
- **`std::function`** — wraps any callable matching a given signature.

While CRTP and Concepts (Days 25–26) resolve types at compile time, type erasure
defers that resolution to runtime, trading some performance for maximum
flexibility and decoupled interfaces.

## Learning Objectives

- Explain what type erasure is and why it is useful.
- Describe the placeholder / holder\<T\> (interface + model) implementation pattern.
- Implement a simplified `Any` class that can store any copyable type.
- Use `typeid` and `std::type_info` to identify stored types at runtime.
- Implement `any_cast` free functions (pointer and reference overloads).
- Understand the cost model: heap allocation and virtual dispatch.
- Compare type erasure (runtime) with templates / Concepts (compile-time).
- Recognise where `std::any` and `std::function` apply type erasure internally.

## Task Summary

Implement a class `Any` that can store a value of any copyable type. The class
exposes `has_value()`, `type()`, and `reset()`. A set of `any_cast` free-
function overloads retrieve the stored value — pointer overloads return
`nullptr` on type mismatch; reference overloads throw `std::bad_cast`.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| `placeholder` (abstract base) | Nested struct with virtual `clone()` and `type()`. |
| `holder<T>` (concrete model) | Derives from `placeholder`, stores a `T value`. |
| Storage | `std::unique_ptr<placeholder>` inside `Any`. |
| Copy semantics | Deep-copy via `placeholder::clone()`. |
| Move semantics | Transfer ownership of the `unique_ptr`. |
| `has_value()` | Returns `true` when a value is held. |
| `type()` | Returns `typeid(T)` of the held type, or `typeid(void)` when empty. |
| `reset()` | Releases the stored value. |
| `any_cast<T>(Any*)` | Returns `T*` or `nullptr` on mismatch. |
| `any_cast<T>(Any&)` | Returns `T` by value or throws `std::bad_cast`. |

## How to Run

From the repository root:

```bash
make day=29 test       # build and run tests
make day=29 asan       # AddressSanitizer build
make day=29 valgrind   # Valgrind memcheck
make day=29 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study the header `exercise.hpp` to understand the public interface.
3. Review the placeholder / holder pattern in the header.
4. Run `make day=29 test` and verify all assertions pass.
5. Run the ASan and Valgrind targets to confirm no leaks.
6. Answer the self-check questions in `notes.md` without peeking at references.
