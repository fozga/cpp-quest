# Custom Iterators

## Day Overview

Welcome to **Day 16** of the 40-day C++ quest. Today you build a custom
iterator from scratch — the fundamental interface that connects containers to
the entire standard algorithms library.

Iterators are the glue between containers and algorithms in C++. They
generalise the concept of a pointer — providing a uniform interface to
dereference, advance, and compare positions — so that generic code never needs
to know the underlying data structure. When you build a custom container you
must also supply custom iterators so that it plugs into range-based `for`,
`<algorithm>`, and `<numeric>` seamlessly.

This day focuses on iterator categories (input, forward, bidirectional,
random-access, contiguous), the traits the standard library expects, and
sentinel-based `begin`/`end` pairs. You will implement a forward iterator for
a fixed-capacity ring buffer and prove it works with the standard library.

---

## Learning Objectives

By the end of this day you will be able to:

- Describe the responsibilities of an iterator: dereference, increment,
  comparison, and category identification.
- Distinguish iterator categories and know which operations each level adds.
- Implement a forward iterator as a nested class inside a container.
- Provide the five standard iterator trait typedefs (`value_type`,
  `difference_type`, `reference`, `pointer`, `iterator_category`).
- Integrate the custom iterator with range-based `for` loops.
- Use the custom iterator with `std::accumulate` and other `<numeric>`
  algorithms.
- Understand logical-to-physical index mapping in a ring buffer and how the
  iterator hides that complexity from callers.

---

## Task Summary

Implement `RingBuffer<T>`, a fixed-capacity circular buffer backed by a
`std::vector<T>`. The buffer tracks a `head` index and a `size`; when full,
new elements overwrite the oldest. Provide `push_back`, `size`, `empty`,
`capacity`, and a nested forward `iterator` class that traverses elements in
logical (oldest-to-newest) order. `begin()` and `end()` return iterators
delimiting the current contents.

---

## Implementation Requirements

| Component | Purpose |
|---|---|
| `RingBuffer<T>(cap)` | Construct buffer with fixed capacity `cap` |
| `push_back(value)` | Append; overwrites oldest element when full |
| `size()` | Number of elements currently stored |
| `empty()` | Whether the buffer is empty |
| `capacity()` | Maximum number of elements |
| `iterator` (nested) | Forward iterator with `*`, `->`, pre/post `++`, `==`, `!=` |
| `begin()` / `end()` | Return iterators to first and past-the-end logical positions |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=16 test

# Build with AddressSanitizer and run
make day=16 asan

# Build a normal binary then run under Valgrind
make day=16 valgrind

# Remove compiled artefacts
make day=16 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to review iterator categories and traits.
2. **Study** the `RingBuffer` class skeleton in `solution/exercise.hpp`.
3. **Implement** the ring buffer operations and the nested iterator.
4. **Compile** with `make day=16 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass.
6. **Run** under ASan (`make day=16 asan`) and Valgrind
   (`make day=16 valgrind`) to confirm clean execution.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper exploration.
