# C++20 Ranges — Notes

## A. Ranges and Views

A **range** is any type that provides `begin()` and `end()` — all standard
containers qualify. A **view** is a special kind of range that is:

- **Non-owning** — it references existing data rather than copying it.
- **Lightweight** — O(1) move/copy, so passing views around is cheap.
- **Lazy** — elements are computed on demand when the view is iterated, not
  when the view is constructed.

The standard view adaptors live in `std::views` (alias for
`std::ranges::views`). Common adaptors include:

| Adaptor | Effect |
|---|---|
| `views::filter(pred)` | Keeps elements for which `pred` returns `true` |
| `views::transform(fn)` | Applies `fn` to each element |
| `views::take(n)` | Yields the first `n` elements |
| `views::drop(n)` | Skips the first `n` elements |
| `views::reverse` | Iterates in reverse order |
| `views::keys` / `views::values` | Projects key or value from pair-like elements |

## B. Range-Based Algorithms

The `<algorithm>` header received range-aware overloads under the
`std::ranges` namespace. These overloads accept a range directly instead of
an iterator pair:

```cpp
std::ranges::sort(vec);                       // no need for vec.begin(), vec.end()
std::ranges::for_each(vec, [](int n){ … });
auto cnt = std::ranges::count_if(vec, pred);
```

Range algorithms also support *projections* — an extra callable applied to
each element before the algorithm's main operation:

```cpp
std::ranges::sort(people, std::less{}, &Person::age);  // sort by age
```

## C. Pipe Syntax and Composition

Views compose with the pipe operator `|`. Each stage in the pipeline
produces a new view without allocating storage:

```cpp
auto result = numbers
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; })
    | std::views::take(5);
```

`result` is a lazy view — nothing is computed until you iterate over it
(e.g., with a range-based `for` loop).

Pipe syntax is syntactic sugar: `range | views::filter(pred)` is equivalent
to `views::filter(pred)(range)`.

## D. Collecting Results

Views are lazy and non-owning, so to materialise the results into a
container you must iterate explicitly:

```cpp
std::vector<int> collected;
for (auto&& x : some_view) {
    collected.push_back(x);
}
```

C++23 introduces `std::ranges::to<Container>()` for a more concise
approach:

```cpp
auto collected = some_view | std::ranges::to<std::vector>();  // C++23
```

Until your toolchain supports C++23, the loop-and-push-back pattern is the
standard technique.

## Self-Check Questions

1. What two member functions make a type satisfy the *range* concept?
2. How does a *view* differ from a general range?
3. What happens when you construct a `views::filter` pipeline — are
   elements evaluated immediately?
4. Can you modify elements through a `views::transform` view?
5. What is the time complexity of copying or moving a view?
6. How does `std::ranges::count_if` differ from the classic
   `std::count_if` in terms of the arguments it accepts?
7. What does the pipe operator (`|`) do when applied between a range and a
   view adaptor?
8. Why might a `views::filter` view not model `sized_range` even if the
   underlying range does?
9. In C++20, what is the standard way to materialise a view into a
   `std::vector`?
10. What is a *projection* in the context of range-based algorithms?
