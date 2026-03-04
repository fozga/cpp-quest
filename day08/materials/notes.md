# Vtables & Memory Leaks — Study Notes

## A. Dynamic Polymorphism and Vtables

In C++ when a class declares or inherits at least one **virtual** function, the
compiler generates a **vtable** (virtual function table) for that class. The
vtable is a static array of function pointers, one slot per virtual function,
pointing to the most-derived override.

Every object of a polymorphic class contains a hidden **vptr** (virtual
pointer) that is set by the constructor to point at the class's vtable. When
you call a virtual function through a base-class pointer or reference, the
compiler emits code that:

1. Follows the object's vptr to its vtable.
2. Indexes into the correct slot.
3. Calls the function pointer found there.

This indirection is called **dynamic dispatch**. It allows the program to
resolve the correct function override at run time rather than compile time.

Key details:

- Each class with virtual functions has exactly **one vtable** (shared by all
  objects of that class).
- Each object carries exactly **one vptr** (typically 8 bytes on a 64-bit
  system), adding to the object's size.
- Constructors set the vptr to the vtable of the class currently being
  constructed. If `Derived` inherits from `Base`, the object's vptr points to
  `Base`'s vtable while `Base::Base()` runs, and is updated to `Derived`'s
  vtable when `Derived::Derived()` runs.
- Destructors reverse this: when `Derived::~Derived()` finishes, the vptr is
  reset to `Base`'s vtable before `Base::~Base()` runs.

---

## B. Destructors and Virtuality

### Virtual Destructors

When you declare `virtual ~Base()`, the destructor participates in dynamic
dispatch just like any other virtual function. This means that `delete
base_ptr` will call the destructor of the actual (most-derived) type, not just
`Base::~Base()`.

### Destruction Order

C++ guarantees that destructors run from the **most-derived class upward**:

1. `Derived::~Derived()` runs first — releases Derived's own resources.
2. Derived's member sub-objects are destroyed in reverse declaration order.
3. `Base::~Base()` runs — releases Base's own resources.
4. Base's member sub-objects are destroyed in reverse declaration order.

This order ensures that each level can safely access its own members during
cleanup.

### What Happens Without a Virtual Destructor

If `Base` does **not** declare its destructor virtual:

```cpp
Base* p = new Derived;
delete p;   // UNDEFINED BEHAVIOUR if ~Base is not virtual
```

The compiler statically resolves `delete p` to call `Base::~Base()` only.
`Derived::~Derived()` never runs. Any resources owned by `Derived` (heap
allocations, file handles, locks) are leaked. Formally this is **undefined
behaviour** per the standard (§7.6.2.9/2).

---

## C. When to Use Virtual Destructors

### The Rule

> If a class is intended to be used polymorphically — i.e., you will ever
> hold a `Base*` or `Base&` that might point to a derived object and later
> destroy it through that handle — then **`Base` must have a virtual
> destructor**.

A common shorthand: **if the class has at least one virtual function, its
destructor should be virtual too.**

### The Cost

- **Per-object cost**: one vptr (typically 8 bytes).
- **Per-call cost**: one indirect function call instead of a direct call.
- **Per-class cost**: one vtable (a static table of pointers).

For classes that are already polymorphic (they have other virtual functions),
adding a virtual destructor costs nothing extra — the vptr is already there.
The marginal cost is one additional slot in the vtable.

### When You Do NOT Need a Virtual Destructor

- Classes that are **not** designed for inheritance or polymorphic use.
- Classes that are inherited but **never deleted through a base pointer**
  (e.g., CRTP bases, private inheritance for implementation reuse).
- Classes that use `protected` or non-virtual destructors to prevent base
  deletion (a deliberate design choice).

---

## D. Memory Leak Scenario

Consider this hierarchy:

```cpp
struct Resource {
    int* data_;
    Resource() : data_(new int[1000]) {}
    ~Resource() { delete[] data_; }   // NOT virtual
};

struct BigResource : Resource {
    std::string* label_;
    BigResource() : label_(new std::string("big")) {}
    ~BigResource() { delete label_; }
};
```

Now:

```cpp
Resource* r = new BigResource;
delete r;  // calls ~Resource() only
           // ~BigResource() never runs → label_ leaked
```

**Symptoms**:

- AddressSanitizer reports a memory leak.
- Valgrind's `--leak-check=full` shows "definitely lost" blocks.
- Resource counters (if any) never reach zero.
- In long-running programs, memory usage grows without bound.

**Fix**: make `~Resource()` virtual:

```cpp
virtual ~Resource() { delete[] data_; }
```

Now `delete r` dispatches through the vtable, calling `~BigResource()` first
(which deletes `label_`), then `~Resource()` (which deletes `data_`).

---

## E. Self-Check Questions

1. What is stored in a vtable, and how many vtables does a single class have?
2. Where is the vptr stored, and when is it initialised?
3. In what order do destructors run for a three-level hierarchy `A → B → C`
   when you delete a `C` object?
4. What happens at run time if you `delete` a `Derived*` through a `Base*`
   when `~Base` is not virtual?
5. Why does the C++ standard call this undefined behaviour, not just "skips
   derived destructor"?
6. A class has no virtual functions but is used as a base. Should you add a
   virtual destructor? Under what circumstances?
7. What is the per-object memory overhead of making a class polymorphic?
8. If `Base` already has a virtual `log()` method, what is the marginal cost
   of also making the destructor virtual?
9. How can you use AddressSanitizer or Valgrind to detect the missing virtual
   destructor bug?
10. How does RAII relate to virtual destructors in a polymorphic hierarchy?
