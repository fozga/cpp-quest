# Day 09 — Notes: Safe Subclassing

---

## A. The Problem of Accidental Overrides

When you intend to override a base-class virtual function but get the
signature slightly wrong, C++ silently declares a **new** function instead.
Polymorphic calls through a base pointer will never reach it.

Common mistakes:

| Intended override | Actual declaration | Bug |
|---|---|---|
| `void draw() const` | `void draw()` | Missing `const` — new function |
| `void draw(int x)` | `void draw(double x)` | Different parameter type |
| `std::string name() const` | `std::string Name() const` | Capitalisation typo |
| `bool operator==(const T&) const` | `bool operator==(T&) const` | Missing `const` on parameter |

In every case the compiler is perfectly happy — it creates a separate
virtual (or non-virtual) function in the derived class. The base-class
version is called at run time, leading to silent, hard-to-diagnose bugs.

Before C++11 the only defence was careful code review. Now we have
`override`.

---

## B. The `override` Specifier

### Syntax

`override` is placed **after** the parameter list (and after any `const` /
`noexcept` qualifiers), before the function body or `= 0`:

```cpp
class Button : public Widget {
public:
    std::string name() const override;   // OK — matches Widget::name()
    void draw() const override;          // OK — matches Widget::draw()
};
```

### What the Compiler Checks

When `override` is present the compiler verifies:

1. There is an **accessible virtual function** with the same name in a
   direct or indirect base class.
2. The **signature matches exactly** — parameter types, `const`-ness,
   ref-qualifiers, and return type (with allowance for covariant returns).

If either check fails, compilation fails with a clear error message.

### Readability

Even without the compiler check, `override` serves as **documentation**.
A reader immediately knows: "this function intentionally replaces a
base-class version." Omitting `override` on a function that *does*
override is a code smell flagged by most static analysers and linters
(`-Wsuggest-override` in GCC / Clang).

### Best Practice

**Always write `override` on every overriding function.** There is no
downside and the safety benefit is significant.

---

## C. The `final` Specifier

`final` has two distinct uses:

### 1. Preventing Further Override of a Virtual Function

```cpp
class ToggleButton : public Button {
public:
    void draw() const override final;
    // or equivalently: void draw() const final;
};
```

Any class that inherits from `ToggleButton` and tries to override `draw()`
will trigger a compile-time error. This is useful when a derived class has
carefully tuned behaviour that must not be silently replaced.

Note: when `final` is used on a function, `override` is implied if the
function is already virtual in a base class; however, writing both
`override final` is idiomatic and maximally explicit.

### 2. Preventing Derivation from a Class

```cpp
class ToggleButton final : public Button {
    // ...
};
```

No class may inherit from `ToggleButton`. Attempting to do so is a
compile-time error. Reasons to mark a class `final`:

- **Design intent** — the class is a leaf in the hierarchy and was not
  designed for extension.
- **Invariant protection** — subclasses could violate internal invariants
  that the class relies on.
- **Devirtualisation optimisation** — the compiler knows no further
  overrides exist and can replace virtual dispatch with direct calls,
  improving performance in tight loops.

### Combining Both

A class can be `final` *and* have individual `final` methods. When the
class itself is `final`, marking individual methods `final` is technically
redundant but still communicates intent clearly.

---

## D. Design Guidelines

### Always Use `override`

There is **no situation** where omitting `override` on an overriding
function is beneficial. Make it a habit; enable compiler warnings
(`-Wsuggest-override`) to catch any you miss.

### When to Use `final`

Use `final` on a **class** when:

- It is a concrete leaf class not designed for subclassing.
- Its correct behaviour depends on invariants that subclasses could break.
- You want to enable devirtualisation for performance-critical paths.

Use `final` on a **function** when:

- The behaviour must not change in further subclasses (e.g., a
  security-critical check, a rendering pipeline step with strict
  contracts).
- The function's implementation is tightly coupled to internal state that
  deeper subclasses should not access.

### When *Not* to Use `final`

Do not reflexively make every class `final`. Library classes intended for
extension should remain open. Apply `final` as a deliberate design
decision, not a default.

---

## E. Self-Check Questions

Answer these from memory after studying the material above.

1. What compile-time guarantee does the `override` specifier provide?
2. Give an example of a signature mismatch that `override` would catch but
   the compiler would silently accept without it.
3. Where does `override` appear in a function declaration relative to
   `const` and `noexcept`?
4. What are the two distinct uses of the `final` specifier?
5. Can a class marked `final` still have virtual functions? Why might it?
6. If `ToggleButton` is marked `final`, is it redundant to also mark
   `ToggleButton::draw()` as `final`? Why might you do it anyway?
7. How can `final` help the compiler generate faster code?
8. Name a situation where marking a class `final` would be a poor design
   choice.
9. What GCC/Clang warning flag encourages consistent use of `override`?
10. In a three-level hierarchy A → B → C, if B::foo() is `final`, what
    happens if C tries to override foo()?
