# Static Polymorphism

## Day Overview

Welcome to **Day 11** of the 40-day C++ quest. Today you move from
**dynamic (run-time) polymorphism** — virtual functions, vtables, and
pointer indirection — to **static (compile-time) polymorphism** powered by
templates and a classic technique called the **Curiously Recurring Template
Pattern (CRTP)**.

With dynamic polymorphism the compiler emits an indirect call through a
vtable pointer every time you invoke a virtual function. This is flexible
(you can store heterogeneous objects in a single container via base
pointers), but the indirection has a cost: the call cannot be inlined, and
every polymorphic object carries an extra hidden pointer.

Static polymorphism eliminates both costs. The base class is a *class
template* parameterised on the derived type, and method dispatch is
resolved entirely at compile time via `static_cast`. The compiler sees the
concrete type, can inline freely, and no vtable pointer is stored in the
object. The trade-offs are increased template complexity and the
impossibility of storing unrelated derived types in a single homogeneous
container without type erasure.

---

## Learning Objectives

By the end of this day you will be able to:

- Explain the difference between dynamic (virtual/vtable) and static
  (template/compile-time) polymorphism.
- Describe the Curiously Recurring Template Pattern (CRTP) and its
  canonical syntax.
- Implement a CRTP base class that dispatches to derived implementations
  via `static_cast<const Derived*>(this)`.
- Write concrete derived classes that supply the required `_impl` methods.
- Write a generic free-function template that operates on any CRTP-derived
  type.
- Articulate the performance benefits of static polymorphism (zero
  overhead, full inlining).
- Identify the trade-offs (template complexity, longer compile times, no
  heterogeneous containers without type erasure).
- Refactor a virtual-function hierarchy into an equivalent CRTP design.

---

## Task Summary

Refactor a small animal hierarchy from virtual dispatch to CRTP:

| Component | Role |
|---|---|
| `AnimalBase<Derived>` | CRTP base template — provides `speak()` and `sound()` that forward to the derived class |
| `Dog` | Inherits `AnimalBase<Dog>`, implements `speak_impl()` and `sound_impl()` |
| `Cat` | Inherits `AnimalBase<Cat>`, implements `speak_impl()` and `sound_impl()` |
| `make_it_speak(a)` | Free function template — calls `a.speak()` on any `AnimalBase`-derived type |
| `get_sound(a)` | Free function template — returns `a.sound()` as a `std::string` |

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| Virtual functions | **None** — the entire point is compile-time dispatch |
| CRTP base | `AnimalBase<Derived>` uses `static_cast<const Derived*>(this)` |
| Derived classes | `Dog` and `Cat` each provide `speak_impl()` and `sound_impl()` |
| Free functions | `make_it_speak` and `get_sound` are function templates |
| Header-only templates | All template definitions live in `exercise.hpp` |
| `exercise.cpp` | Intentionally minimal (just `#include "exercise.hpp"`) |
| Warnings | Must compile with `-Wall -Wextra` and produce **zero** warnings |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=11 test

# Build with AddressSanitizer and run
make day=11 asan

# Build a normal binary then run under Valgrind
make day=11 valgrind

# Remove compiled artefacts
make day=11 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand CRTP and how it replaces
   virtual dispatch.
2. **Open** `solution/exercise.hpp` and study the `AnimalBase` template
   and derived classes.
3. **Compile** with `make day=11 test` — the tests should pass
   immediately since everything is defined in the header.
4. **Experiment** — try adding a third animal (e.g. `Cow`) to reinforce
   the pattern.
5. **Run** under ASan (`make day=11 asan`) and Valgrind
   (`make day=11 valgrind`) to confirm clean execution.
6. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
7. **Browse** the links in `materials/links.md` for deeper exploration.
