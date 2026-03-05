# Exception Removal — Notes

## A. Exceptions vs Return-Based Error Codes

C++ exceptions use `throw` to signal an error and `try`/`catch` to handle it. When an
exception is thrown the runtime performs **stack unwinding**: it destructs local objects
frame by frame until a matching `catch` is found. This mechanism is powerful but has
trade-offs:

- **Binary size**: exception tables and RTTI metadata increase the executable.
- **Performance**: the "zero-cost" model means no overhead on the happy path, but
  throwing is orders of magnitude slower than a normal return.
- **Disabled builds**: many embedded, game-engine, and high-frequency-trading codebases
  compile with `-fno-exceptions`, making `throw` unavailable.
- **Invisible control flow**: reading a function signature gives no hint that it can fail
  or what errors it produces.

Return-based error handling (error codes, `std::optional`, discriminated unions) makes the
failure path explicit in the type system. The cost is that every caller must check the
result — but that is also the benefit.

## B. std::expected<T, E>

C++23 adds `std::expected<T, E>` in `<expected>`. It holds **either** a value of type `T`
(the success case) **or** an error of type `E` (the failure case). Think of it as
`std::variant<T, E>` with richer semantics:

```cpp
std::expected<int, std::string> parse_int(std::string_view s);
```

Key properties:

| Operation | Meaning |
|-----------|---------|
| `has_value()` / `operator bool()` | Does it hold a value? |
| `value()` | Access T (throws `bad_expected_access` if error) |
| `error()` | Access E (undefined behaviour if value) |
| `value_or(default)` | Return T or a fallback |

Construction is implicit from `T` (success) or via the `std::unexpected(e)` helper
(failure).

## C. Using Expected

Because the error type is encoded in the return type, call sites **must** inspect the
result:

```cpp
auto result = parse_packet(buf);
if (!result) {
    log("parse failed: " + to_string(result.error()));
    return;
}
process(result.value());
```

Compared with exceptions this has several advantages:

1. The function signature documents all possible errors.
2. The compiler can warn about ignored return values (`[[nodiscard]]`).
3. No hidden control-flow jumps — the error path is a regular `if`.
4. Works in `-fno-exceptions` builds (our `Expected` only uses exceptions
   internally as a safety net for misuse, not as a normal error channel).

## D. Monadic Composition

`std::expected` (C++23) supports monadic operations that let you chain fallible steps
without nested `if` checks:

- **`transform(f)`** — if value, apply `f` to it and rewrap; if error, propagate.
- **`and_then(f)`** — if value, call `f` (which itself returns an `expected`); if error,
  propagate. This is "flat map".
- **`or_else(f)`** — if error, call `f`; if value, propagate.
- **`transform_error(f)`** — apply `f` to the error.

Conceptually:

```cpp
auto result = read_header(buf)
    .and_then(validate_version)
    .and_then(extract_payload)
    .transform(decrypt);
```

Each step only executes if the previous one succeeded. This style replaces deeply nested
`if`/`else` error checks with a readable pipeline.

## E. When to Choose Exceptions vs Expected

| Criterion | Exceptions | Expected |
|-----------|-----------|----------|
| Error frequency | Rare / truly exceptional | Common / part of normal flow |
| Performance sensitivity | Acceptable latency on throw | Hot path, cannot afford unwinding |
| Build constraints | Exceptions enabled | `-fno-exceptions` or freestanding |
| API clarity | Error hidden in docs | Error visible in signature |
| Composition | try/catch nesting | Monadic chaining |

In practice many codebases use both: exceptions for programmer errors and invariant
violations, and `expected`/error codes for domain-level failures like parsing, I/O, and
validation.

## F. Self-Check Questions

1. What happens during stack unwinding when an exception is thrown?
2. Why do some C++ projects compile with `-fno-exceptions`?
3. How does `std::expected<T, E>` differ from `std::optional<T>`?
4. What does `has_value()` return when an `expected` holds an error?
5. Why is the error type `E` part of the template signature rather than always
   being `std::exception`?
6. How does `and_then` differ from `transform` in monadic composition?
7. In the packet parser, why is returning `Expected::error(ParseError::TooShort)` safer
   than `throw std::runtime_error("too short")`?
8. What is the "zero-cost exception model" and where does its cost actually appear?
9. Can `Expected<T, E>` be used when `T` and `E` are the same type? What problem
   arises?
10. When would you still prefer exceptions over `expected`?
