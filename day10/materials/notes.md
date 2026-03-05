# The Diamond Problem — Notes

## A. Multiple Inheritance Basics

C++ allows a class to inherit from more than one base class:

```cpp
class Derived : public Base1, public Base2 { };
```

The derived object contains a subobject for each base, laid out in declaration
order. Member functions from both bases are accessible. Name collisions between
the bases must be resolved with explicit qualification (`Base1::foo()` vs
`Base2::foo()`).

---

## B. The Diamond Problem

A **diamond** forms when two intermediate classes share a common ancestor and a
fourth class inherits from both intermediates:

```
      Device
      /    \
InputDevice  OutputDevice
      \    /
      IODevice
```

Without special treatment, `IODevice` contains **two** `Device` subobjects —
one through `InputDevice` and one through `OutputDevice`. This causes:

1. **Ambiguous member access** — `iodev.name()` could refer to either copy. The
   compiler rejects the call unless you qualify it
   (`iodev.InputDevice::name()`).
2. **Duplicated state** — changes to one `Device` subobject are invisible to
   the other, which is almost never the intended semantics.
3. **Wasted memory** — two copies of the same base occupy space for no benefit.

---

## C. Virtual Inheritance

Virtual inheritance tells the compiler to share a **single instance** of the
common base across all paths:

```cpp
class InputDevice  : public virtual Device { };
class OutputDevice : public virtual Device { };
class IODevice     : public InputDevice, public OutputDevice { };
```

Key rules:

- The `virtual` keyword appears on the **intermediate** classes, not on the
  most-derived class.
- Only **one** `Device` subobject exists inside `IODevice`.
- The **most-derived class** (`IODevice`) is responsible for directly calling
  the `Device` constructor in its member initializer list. Any `Device(...)`
  calls in `InputDevice` or `OutputDevice` init lists are **ignored** when the
  object is being constructed as part of an `IODevice`.
- When `InputDevice` is constructed **standalone** (not as part of a more-derived
  object), its own `Device(...)` initializer **is** used.

Constructor order with virtual bases:

1. Virtual base subobjects are constructed first, in depth-first left-to-right
   declaration order.
2. Non-virtual base subobjects are constructed next, in declaration order.
3. The most-derived class body executes last.

For `IODevice`, the order is:
`Device` → `InputDevice` → `OutputDevice` → `IODevice`.

---

## D. Trade-offs

Virtual inheritance is not free:

| Concern | Impact |
|---|---|
| **Memory layout** | The compiler adds hidden pointers (vbase pointers or offsets in the vtable) to locate the shared subobject. |
| **Construction complexity** | The most-derived class must know about the virtual base, even if it is many levels up the hierarchy. |
| **Runtime cost** | Accessing virtual base members may involve an extra indirection. |
| **Design coupling** | Adding virtual inheritance after the fact can break ABI and requires all intermediate classes to opt in. |

Virtual inheritance is a deliberate design tool. It is most commonly seen in
interface-style hierarchies (e.g., `std::iostream` inherits virtually from
`std::ios_base` through `istream` and `ostream`). Prefer composition or
single inheritance when the diamond shape is not essential.

---

## E. Self-Check Questions

Test your understanding — try answering without looking at the notes above.

1. What shape of inheritance graph gives rise to the diamond problem?
2. How many `Device` subobjects does `IODevice` contain without virtual
   inheritance? With virtual inheritance?
3. Which keyword on which classes enables virtual inheritance?
4. When `IODevice` is constructed, which class is responsible for initializing
   the `Device` subobject? Why?
5. What happens to the `Device(...)` call in `InputDevice`'s initializer list
   when `InputDevice` is constructed as part of an `IODevice`?
6. In what order are constructors called when constructing an `IODevice`?
7. What hidden data does the compiler add to support virtual inheritance, and
   what is its purpose?
8. Why is accessing a member of a virtual base potentially slower than accessing
   a member of a non-virtual base?
9. Give a real-world Standard Library example that uses virtual inheritance.
10. When should you prefer composition over virtual inheritance?
