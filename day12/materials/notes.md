# Optional Values — Notes

## A. What is `std::optional`?

`std::optional<T>` (header `<optional>`, C++17) is a wrapper that holds either
a value of type `T` or *nothing*. Internally it stores aligned storage large
enough for `T` plus a `bool` flag indicating whether the value is engaged.

Key member functions:

| Function | Description |
|---|---|
| `has_value()` | Returns `true` if the optional contains a value. |
| `operator bool` | Same semantics as `has_value()`; enables `if (opt) …` |
| `value()` | Returns a reference to the contained value; throws `std::bad_optional_access` if empty. |
| `value_or(default)` | Returns the value if present, otherwise returns `default`. |
| `operator*` / `operator->` | Unchecked access — undefined behaviour if empty. |
| `reset()` | Destroys the contained value (if any) and disengages the optional. |
| `emplace(args…)` | Constructs a new `T` in-place inside the optional. |

Construction:

```cpp
std::optional<int> a;               // disengaged (empty)
std::optional<int> b = std::nullopt; // disengaged, explicit
std::optional<int> c = 42;          // engaged, holds 42
std::optional<int> d{std::in_place, 7}; // engaged via in-place construction
```

---

## B. Replacing Null Pointers and Sentinels

Traditional approaches to "no value":

- **Raw pointers**: `int* find(…)` returns `nullptr` on failure. The caller
  must remember to check; dereferencing `nullptr` is undefined behaviour.
- **Sentinel values**: a function returning `int` uses `-1` to mean "not
  found". Nothing in the type system prevents the caller from using `-1` as a
  real result.
- **Output parameters**: `bool get(Key, int& out)` — awkward API, easy to
  ignore the return value.

`std::optional<T>` fixes all three problems. The return type **forces** the
caller to acknowledge the possibility of absence. The compiler's type checker
participates in correctness.

---

## C. Basic Usage Patterns

### Returning optional from parsers

```cpp
std::optional<int> parse_int(std::string_view s) {
    int val{};
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), val);
    if (ec != std::errc{} || ptr != s.data() + s.size())
        return std::nullopt;
    return val;
}
```

### Conditional access with `if`

```cpp
if (auto val = parse_int(input); val.has_value()) {
    use(*val);
}
```

### Providing defaults with `value_or`

```cpp
int timeout = get_int(cfg, "timeout").value_or(30);
```

### Chaining lookups

```cpp
std::optional<std::string> name = get_string(cfg, "alias");
if (!name) name = get_string(cfg, "hostname");
```

---

## D. Performance Characteristics

- **Storage**: `sizeof(std::optional<T>)` ≈ `sizeof(T) + alignof(T)` (the
  flag is typically one byte, but alignment padding may enlarge the total).
- **No dynamic allocation**: the value lives inside the optional object
  itself. Construction and destruction happen in-place.
- **Copy/move**: copying or moving an optional copies or moves the contained
  `T`. For small, trivially-copyable types the cost is negligible.
- **Comparison**: two optionals can be compared with `==`, `<`, etc. An empty
  optional compares less than any engaged optional.

---

## E. Embedded and Real-Time Considerations

`std::optional` is particularly attractive in embedded contexts:

- It avoids heap allocation entirely, unlike `std::unique_ptr<T>`.
- It is deterministic: construction, access, and destruction have no hidden
  system calls.
- It composes well with `constexpr` (fully `constexpr` since C++20 for
  trivially-destructible types).

**Caveats**:

- For very large `T`, the optional always reserves `sizeof(T)` even when
  empty. If `T` is hundreds of bytes, consider a pointer-based approach or
  redesign the data layout.
- Some embedded toolchains lag behind the standard; verify `<optional>` is
  available or provide a polyfill.
- `value()` throws on empty access. In `-fno-exceptions` builds, use
  `has_value()` + `operator*` or `value_or()` instead.

---

## F. Self-Check Questions

1. What header provides `std::optional` and what C++ standard introduced it?
2. What happens when you call `value()` on an empty `std::optional`?
3. How does `value_or()` differ from `value()` in its failure mode?
4. Why is `std::optional` preferable to returning a raw pointer for
   "value or nothing" semantics?
5. What is the approximate size of `std::optional<int>` on a typical
   64-bit platform, and why?
6. Can `std::optional` hold a reference (e.g. `std::optional<int&>`)? Why
   or why not?
7. In what situation would `std::unique_ptr<T>` be a better choice than
   `std::optional<T>` for representing an absent value?
8. How does `std::nullopt` relate to the default-constructed state of an
   optional?
9. Why should you prefer `operator*` over `value()` in code compiled
   with exceptions disabled (`-fno-exceptions`)?
10. Describe a real-world embedded scenario where returning
    `std::optional<int>` is safer than returning `-1` as a sentinel.
