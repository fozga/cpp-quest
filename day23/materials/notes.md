# SFINAE to Concepts — Notes

## A. From SFINAE to Concepts: Side-by-Side

### SFINAE (Day 21)

```cpp
template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::string>
serialize(T value) {
    std::ostringstream oss;
    oss << "int:" << value;
    return oss.str();
}
```

The constraint is buried inside the return type.  If `T` is not integral the
overload is silently removed (SFINAE), but the resulting error message when *no*
overload matches is often a wall of template instantiation backtraces.

### Concepts (Day 23)

```cpp
template <IntegralSerializable T>
std::string serialize_concept(T value) {
    std::ostringstream oss;
    oss << "int:" << value;
    return oss.str();
}
```

The constraint is on the template parameter itself.  When a type fails the
concept the compiler reports exactly which concept was not satisfied, making the
error immediately actionable.

### Mapping

| SFINAE element | Concept equivalent |
|---|---|
| `std::enable_if_t<std::is_integral_v<T>, R>` | `template <IntegralSerializable T>` with return type `R` |
| `std::enable_if_t<!std::is_integral_v<T>, R>` | A separate concept like `FloatingSerializable` |
| Mutually exclusive `enable_if` pairs | Compiler auto-ranks more-constrained overloads |

## B. requires Clauses vs Constrained Template Parameters

### Constrained parameter

```cpp
template <IntegralSerializable T>
std::string serialize_concept(T value);
```

Short and clean — the concept replaces `typename` directly.

### Trailing requires clause

```cpp
template <typename T>
std::string serialize_concept(T value) requires IntegralSerializable<T>;
```

Equivalent, but slightly more verbose.  Useful when the constraint is complex
or involves multiple parameters.

### Inline requires expression

```cpp
template <typename T>
    requires std::is_integral_v<T>
std::string serialize_concept(T value);
```

Avoids naming a concept at all.  Good for one-off constraints but hurts
readability if repeated across many overloads.

All three forms produce the same compiled output; choose the one that best
communicates intent.

## C. Readability and Diagnostics

### Readability

- SFINAE constraints are typically 2–3 lines of nested template machinery.
- Concept constraints read like plain English: `template <Numeric T>`.
- Concepts compose naturally: `template <typename T> requires Numeric<T> && Copyable<T>`.

### Diagnostics

With SFINAE, a failed substitution often produces errors like:

```
error: no matching function for call to 'serialize(std::vector<int>)'
note: candidate template ignored: substitution failure [with T = std::vector<int>]:
      ... 12 lines of enable_if expansion ...
```

With concepts the error is:

```
error: no matching function for call to 'serialize_concept(std::vector<int>)'
note: constraints not satisfied
note: because 'std::is_integral_v<std::vector<int>>' evaluated to false
```

The concept version names the exact predicate that failed and skips the
template-machinery noise.

### Overload ordering

When two concept-constrained overloads are both viable, the compiler prefers
the more constrained one (subsumption).  With SFINAE, you must manually craft
mutually exclusive conditions.

## D. Self-Check Questions

1. What is the mechanical change needed to convert an `enable_if`-based constraint into a concept?
2. Why do concept-based overloads not need mutually exclusive conditions the way `enable_if` pairs do?
3. What does the compiler report when you call `serialize_concept` with a type that satisfies no concept?
4. Can a single function template have both a constrained template parameter and a trailing `requires` clause?
5. Is `bool` considered integral by `std::is_integral_v`?  What about `char`?
6. Why is the `std::string` overload of `serialize_concept` a non-template function rather than a concept-constrained template?
7. How does concept subsumption determine which overload is preferred when multiple constraints are satisfied?
8. Could you replace `IntegralSerializable` and `FloatingSerializable` with a single `Arithmetic` concept?  What trade-off would that involve?
9. What happens if you define the template overloads in `exercise.cpp` instead of the header?
10. How would you add a concept-constrained overload for pointer types?
