# Circular References

## Day Overview

Welcome to **Day 3** of the 40-day C++ quest. Today you confront one of the
most common pitfalls of `std::shared_ptr`: **circular references**.

`shared_ptr` keeps objects alive through **reference counting**. Every copy of
a `shared_ptr` increments the count; every destruction decrements it. The
managed object is deleted only when the count reaches zero. This works
beautifully for tree-shaped ownership — but the moment two objects each hold a
`shared_ptr` to the other, neither count can ever reach zero. Memory leaks
silently, destructors never run, and tools like Valgrind light up red.

The standard-library cure is `std::weak_ptr`: a smart pointer that *observes*
a shared object **without affecting its reference count**. By replacing one
direction of a mutual `shared_ptr` link with a `weak_ptr`, you break the cycle
and restore automatic cleanup.

---

## Learning Objectives

By the end of this day you will be able to:

- Explain the `shared_ptr` ownership model and how its reference count
  controls object lifetime.
- Describe the structure of a `shared_ptr` control block and what
  `use_count()` reports.
- Recognize circular-reference patterns in object graphs (doubly-linked lists,
  parent–child trees, observer networks).
- Explain why a reference cycle prevents deallocation even when no external
  `shared_ptr` survives.
- Describe how `weak_ptr` observes a shared object without contributing to the
  reference count.
- Use `weak_ptr::expired()` and `weak_ptr::lock()` safely.
- Refactor a leaking cycle by choosing an owning direction (`shared_ptr`) and
  a non-owning back-pointer direction (`weak_ptr`).
- Verify leak-freedom with destructor-side counters, AddressSanitizer, and
  Valgrind.

---

## Task Summary

You will build a **circular doubly-linked list** of `Node` objects. Each node
has a `next` pointer (owning, `shared_ptr`) and a `prev` pointer (non-owning,
`weak_ptr`). The `next` pointers form a ring: the last node's `next` points
back to the first.

Because `prev` is a `weak_ptr`, it does not contribute to the reference count.
The ring can therefore be cleaned up by breaking a single `next` link (the one
that closes the ring) and then releasing the nodes.

Your `clear_list` function must break this cycle and release all nodes. A
static live-node counter (incremented in the constructor, decremented in the
destructor) lets the tests verify that every node is actually destroyed.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| `Node::name` | `std::string` identifying the node |
| `Node::next` | `std::shared_ptr<Node>` — owning forward link (forms a ring) |
| `Node::prev` | `std::weak_ptr<Node>` — non-owning back link |
| Constructor | Stores name, increments live counter |
| Destructor | Decrements live counter |
| `make_cyclic_list(n)` | Creates *n* nodes, wires `next` into a ring and `prev` as weak back-links |
| `clear_list(list)` | Breaks the ring (nulls the closing `next`) then clears the vector |
| `get_live_node_count()` | Returns current live-node counter |
| `reset_live_node_count()` | Sets counter to 0 for test isolation |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=03 test

# Build with AddressSanitizer and run
make day=03 asan

# Build a normal binary then run under Valgrind
make day=03 valgrind

# Remove compiled artefacts
make day=03 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand `shared_ptr` cycles and
   `weak_ptr`.
2. **Open** `solution/exercise.hpp` and study the `Node` struct and function
   signatures.
3. **Implement** each function in `solution/exercise.cpp`, starting with the
   node constructor/destructor, then `make_cyclic_list`, then `clear_list`.
4. **Compile** with `make day=03 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass.
6. **Run** under ASan (`make day=03 asan`) and Valgrind
   (`make day=03 valgrind`) to confirm zero leaks.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper understanding.
