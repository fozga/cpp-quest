# Custom Iterators — Notes

## A. Iterator Basics

An iterator is a generalised pointer. It provides three core capabilities:

1. **Dereference** (`*it`) — access the element at the current position.
2. **Increment** (`++it`) — advance to the next element.
3. **Comparison** (`it == end`) — test whether two positions are equivalent.

The standard library organises iterators into categories, each adding
operations on top of the previous:

| Category | Key operations | Example containers |
|---|---|---|
| Input | `*it` (read), `++it`, `==` | `std::istream_iterator` |
| Forward | + multi-pass guarantee | `std::forward_list` |
| Bidirectional | + `--it` | `std::list`, `std::set` |
| Random Access | + `it + n`, `it - it`, `<` | `std::vector`, `std::deque` |
| Contiguous | + elements in contiguous memory | `std::vector`, `std::array` |

Algorithms specify the minimum category they require. A forward iterator
satisfies any algorithm that asks for an input iterator, and so on up the
hierarchy.

## B. Forward Iterator Requirements

A type models the forward iterator concept when it is:

- **Copy-constructible** and **copy-assignable**.
- **Multi-pass**: incrementing one copy does not invalidate another.
- Supports `*it` (returns a reference), `++it`, `it++`, `it == jt`, `it != jt`.
- Exposes `iterator_category = std::forward_iterator_tag`.

A minimal nested iterator class typically looks like:

```cpp
class iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    reference operator*()  const;
    pointer   operator->() const;
    iterator& operator++();
    iterator  operator++(int);
    bool operator==(const iterator&) const;
    bool operator!=(const iterator&) const;
};
```

Both pre-increment and post-increment must be provided. Post-increment
returns a copy of the old position, then advances.

## C. Iterator Traits

`std::iterator_traits<It>` looks up five nested typedefs:

| Typedef | Meaning |
|---|---|
| `value_type` | The element type the iterator refers to |
| `reference` | The type returned by `operator*` |
| `pointer` | The type returned by `operator->` |
| `difference_type` | Signed type for distance between iterators |
| `iterator_category` | Tag type indicating the category |

If these are defined as public `using` aliases inside the iterator class,
`std::iterator_traits` picks them up automatically. Algorithms such as
`std::accumulate` and `std::distance` rely on these traits to select optimised
code paths.

## D. Ring Buffer Traversal

A ring (circular) buffer stores elements in a fixed-size array. Two indices
govern it:

- **head** — position of the oldest element.
- **size** — number of elements currently stored (≤ capacity).

When the buffer is not yet full, new elements land at
`(head + size) % capacity`. When full, the new element overwrites the slot at
`head`, and `head` advances by one (wrapping around).

The *logical* index `i` (0 = oldest, `size - 1` = newest) maps to a
*physical* index with:

```
physical = (head + i) % capacity
```

The iterator stores a logical index and converts to physical on each
dereference, hiding the circular layout from users.

## E. Sentinels and begin / end

In C++ containers, `begin()` returns an iterator to the first element and
`end()` returns a past-the-end sentinel. For a ring buffer:

- `begin()` constructs an iterator with logical index `0`.
- `end()` constructs an iterator with logical index equal to `size`.

Incrementing from the last valid element (`logical == size - 1`) produces
the end iterator (`logical == size`), at which point dereferencing is
undefined. Comparing `it == end` is a simple integer comparison of logical
indices.

This pattern means range-based `for`:

```cpp
for (auto& x : buf) { ... }
```

expands to code that walks from `begin()` to `end()`, calling `++` and `*`
exactly as for any standard container.

## F. Self-Check Questions

1. What three operations must every iterator support?
2. Name the five standard iterator categories in order from weakest to
   strongest.
3. What does the multi-pass guarantee mean for forward iterators?
4. Which typedef tells algorithms whether an iterator is forward,
   bidirectional, or random access?
5. Why does a ring buffer need modulo arithmetic when mapping logical to
   physical indices?
6. What value does the `end()` iterator hold for a ring buffer with 4
   elements?
7. What happens to `head` when you push into a full ring buffer?
8. Why must both `operator++()` and `operator++(int)` be provided?
9. How does `std::accumulate` discover the `value_type` of a custom iterator?
10. If you copy a forward iterator and increment the copy, is the original
    still valid and dereferenceable?
