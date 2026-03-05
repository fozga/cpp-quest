# Custom Iterators

## Day Overview

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

## Learning Objectives

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

## Task Summary

Implement `RingBuffer<T>`, a fixed-capacity circular buffer backed by a
`std::vector<T>`. The buffer tracks a `head` index and a `size`; when full,
new elements overwrite the oldest. Provide `push_back`, `size`, `empty`,
`capacity`, and a nested forward `iterator` class that traverses elements in
logical (oldest-to-newest) order. `begin()` and `end()` return iterators
delimiting the current contents.

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

## How to Run

From the repository root:

```bash
make day=16 test       # build and run tests
make day=16 asan       # AddressSanitizer build
make day=16 valgrind   # Valgrind memcheck
make day=16 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to review iterator categories and traits.
2. Study the `RingBuffer` class skeleton in `exercise.hpp`.
3. Implement the ring buffer operations and the nested iterator.
4. Run `make day=16 test` and iterate until all assertions pass.
5. Run the sanitizer and valgrind targets to verify memory safety.
