# Move Constructors

## Day Overview

C++ distinguishes between **copy semantics** and **move semantics**. Copying
duplicates a resource so that source and destination each own an independent
copy. Moving *transfers* ownership of an existing resource from one object to
another, leaving the source in a valid but unspecified (typically empty) state.

Move semantics are enabled by **rvalue references** (`T&&`), introduced in
C++11. When the compiler can prove that a value is about to expire (a
temporary, or an object explicitly cast with `std::move`), it selects a move
constructor or move-assignment operator instead of the copy versions. This
avoids expensive deep copies of heap-allocated buffers, file handles, and other
resources.

Move semantics connect directly to the Block 1 theme of **RAII**: an object
that owns a resource through RAII can cheaply hand that resource to a new owner
rather than duplicating it, provided the class implements a move constructor
and move-assignment operator.

## Learning Objectives

- Distinguish lvalue references (`T&`) from rvalue references (`T&&`).
- Explain when the compiler chooses a move operation over a copy.
- Understand why marking move operations `noexcept` matters for standard
  library containers (e.g., `std::vector` reallocation).
- Implement a dynamic-array container that efficiently moves its internal
  buffer instead of copying it.
- Observe the behavioural difference between a copied object and a moved-from
  object in tests.
- Recognise the "steal-and-null" pattern for move constructors and
  move-assignment operators.
- Use `std::move` to explicitly request a move when the compiler cannot deduce
  one.
- Verify resource safety with AddressSanitizer and Valgrind.

## Task Summary

Implement a class template `DynamicArray<T>` that owns a heap-allocated buffer
(`T*`), a `size_`, and a `capacity_`. Provide:

| Member | Description |
|---|---|
| Default constructor | Empty array (`nullptr`, size 0, capacity 0) |
| Size constructor | `explicit DynamicArray(std::size_t count)` — allocates and value-initialises `count` elements |
| Initializer-list constructor | `DynamicArray(std::initializer_list<T>)` |
| Destructor | `delete[]` the buffer |
| Copy constructor | Deep-copy the buffer |
| Copy-assignment operator | Deep-copy with self-assignment safety |
| Move constructor (`noexcept`) | Steal buffer, size, capacity; null the source |
| Move-assignment operator (`noexcept`) | Clean up current resources, steal from source, null source |
| `size()`, `capacity()`, `empty()` | Const observers |
| `operator[]` | Const and non-const element access |
| `push_back(const T&)` / `push_back(T&&)` | Append, growing capacity as needed |

Moved-from objects must be left in a **valid empty state** (size 0, null
buffer).

## Implementation Requirements

- **Strong exception safety** in copy operations — if `new[]` or an element
  copy throws, no resources leak.
- **Move operations steal** the buffer pointer, size, and capacity from the
  source, then set the source's members to zero/null.
- **Move operations are `noexcept`** so that `std::vector<DynamicArray<T>>`
  can use moves during reallocation.
- Use `new T[]` / `delete[]` for memory management (no `malloc`/`free`).
- Growth strategy for `push_back`: double the capacity (minimum 1 when empty).

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=05 test

# Build with AddressSanitizer and run
make day=05 asan

# Build a normal binary then run under Valgrind
make day=05 valgrind

# Remove compiled artefacts
make day=05 clean
```

## Suggested Workflow

1. Read `materials/notes.md` for background on move semantics.
2. Open `solution/exercise.hpp` and study the class skeleton.
3. Implement the default, size, and initializer-list constructors plus the
   destructor first. Run the tests to confirm basic construction works.
4. Add the copy constructor and copy-assignment operator. Test independence of
   copies.
5. Implement the move constructor and move-assignment operator. Verify that
   moved-from objects are empty.
6. Implement `push_back` (both overloads). Test growing the array.
7. Test with `DynamicArray<std::string>` to exercise non-trivial moves.
8. Run under ASan and Valgrind to confirm no leaks or undefined behaviour.
