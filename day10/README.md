# The Diamond Problem

## Day Overview

Welcome to **Day 10** of the 40-day C++ quest. Today you tackle **multiple
inheritance** and its most infamous pitfall: the **diamond problem**.

When a class inherits from two bases that share a common ancestor, the derived
class ends up with **two copies** of the ancestor's subobject. Any attempt to
access a member from the shared ancestor becomes **ambiguous** — the compiler
cannot decide which copy you mean.

C++ solves this with **virtual inheritance**. By marking the intermediate bases
as `virtual`, the compiler ensures that only a **single shared instance** of the
common ancestor exists in the most-derived object. The trade-off is that the
**most-derived class** becomes responsible for directly initializing the virtual
base, regardless of how many intermediate classes sit between them.

Today's exercise models a realistic IO hierarchy:
`Device → InputDevice, OutputDevice → IODevice`, where `Device` is inherited
through two paths. You will see the ambiguity first-hand and resolve it with
virtual inheritance.

---

## Learning Objectives

By the end of this day you will be able to:

- Draw the diamond inheritance shape and identify the duplicated base subobject.
- Explain why non-virtual multiple inheritance creates **two** copies of the
  common base and how that causes ambiguous member access.
- Apply `virtual` inheritance syntax to intermediate classes so the common base
  is shared.
- Describe the constructor initialization order when virtual bases are involved:
  the most-derived class constructs the virtual base first, before any
  non-virtual bases.
- Write an `IODevice` class that correctly initializes a single `Device`
  subobject through virtual inheritance.
- Identify the memory-layout and performance trade-offs of virtual inheritance.
- Articulate when multiple inheritance (and virtual inheritance) is a deliberate,
  justified design choice versus an accidental complexity.

---

## Task Summary

Implement an IO device hierarchy with four classes:

| Class | Role |
|---|---|
| `Device` | Common base — holds a `name_` and provides default `read()` / `write()` |
| `InputDevice` | Virtually inherits from `Device`, overrides `read()` |
| `OutputDevice` | Virtually inherits from `Device`, overrides `write()` |
| `IODevice` | Inherits from both `InputDevice` and `OutputDevice`, overrides both |

`IODevice` must have exactly **one** `Device` subobject. The test suite verifies
this by checking that `dev.name()` is unambiguous and that `Device&` binds
without error.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| Virtual base | `Device` is a virtual base of both `InputDevice` and `OutputDevice` |
| Single subobject | `IODevice` contains exactly one `Device`; `name()` is unambiguous |
| Constructor order | `IODevice` initializes `Device` directly (most-derived constructs virtual base) |
| Common member | `Device::name_` (protected `std::string`) illustrates shared state |
| `read()` / `write()` | Virtual functions overridden at each level; return or print identifying strings |
| `demo_io_device` | Free function that exercises `read()` and `write()` on an `IODevice&` |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=10 test

# Build with AddressSanitizer and run
make day=10 asan

# Build a normal binary then run under Valgrind
make day=10 valgrind

# Remove compiled artefacts
make day=10 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand the diamond problem and virtual
   inheritance.
2. **Open** `solution/exercise.hpp` and study the class hierarchy declarations.
3. **Implement** each member function in `solution/exercise.cpp`, starting with
   `Device`, then `InputDevice`, then `OutputDevice`, and finally `IODevice`.
   Pay close attention to which class initializes `Device` in its member
   initializer list.
4. **Compile** with `make day=10 test` — fix any compiler errors (ambiguity
   errors are the most common here).
5. **Run** the tests and make every assertion pass. The critical assertion is
   that a single, unambiguous `Device` subobject exists inside `IODevice`.
6. **Run** under ASan (`make day=10 asan`) and Valgrind
   (`make day=10 valgrind`) to confirm clean memory behaviour.
7. **Answer** the self-check questions in `materials/notes.md` without looking
   at the notes.
8. **Browse** the links in `materials/links.md` for deeper understanding.
