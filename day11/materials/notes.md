# Static Polymorphism — Notes

## A. Dynamic vs Static Polymorphism

Dynamic polymorphism uses **virtual functions** and a **vtable** (virtual
method table). Each polymorphic object contains a hidden pointer to its
class's vtable. When you call a virtual function through a base-class
pointer or reference, the program follows the vtable pointer at run time to
find the correct function to invoke. This allows heterogeneous containers
(`std::vector<Base*>`) but carries costs: an extra pointer per object, an
indirect function call that the optimiser generally cannot inline, and
potential cache misses when the vtable is not in cache.

Static polymorphism resolves dispatch **entirely at compile time**. The
compiler knows the concrete type of every object, so it can emit a direct
call — or inline the function body completely. There is no vtable pointer,
no indirection, and no run-time cost beyond what a hand-written
non-polymorphic call would incur.

| Aspect | Dynamic | Static (CRTP) |
|---|---|---|
| Dispatch time | Run time | Compile time |
| Mechanism | vtable pointer + indirect call | Template instantiation + `static_cast` |
| Object size | Includes hidden vptr (typically 8 bytes) | No hidden pointer — can be as small as 1 byte |
| Inlining | Generally prevented by indirection | Fully available |
| Heterogeneous containers | Yes (`vector<Base*>`) | No (each derived type is unrelated at the type level) |
| Compile time | Faster | Slower (template instantiation) |
| Error messages | Clear | Can be verbose (template errors) |

---

## B. CRTP Basics

The **Curiously Recurring Template Pattern** (CRTP) is the canonical way to
achieve static polymorphism in C++. The idea: a base class is a *class
template* that takes its own derived class as the template parameter.

```cpp
template <typename Derived>
class Base {
public:
    void interface_method() const {
        // Downcast to the derived type at compile time
        static_cast<const Derived*>(this)->implementation();
    }
};

class Concrete : public Base<Concrete> {
public:
    void implementation() const {
        // actual work
    }
};
```

Key mechanics:

1. `Base<Derived>` is parameterised on `Derived`, so it *knows* the
   concrete type at compile time.
2. `static_cast<const Derived*>(this)` is safe because `Derived` publicly
   inherits from `Base<Derived>` — the cast merely adjusts the static type
   without any run-time check.
3. Because the compiler sees the concrete type, it can inline
   `implementation()` directly into `interface_method()`.
4. Each derived class creates a **separate base-class instantiation**:
   `Base<Dog>` and `Base<Cat>` are distinct, unrelated types. This is why
   you cannot store dogs and cats in the same `vector<Base*>` — there is no
   single, non-template `Base` type.

---

## C. Advantages and Trade-offs

**Advantages**

- **Zero overhead** — no vtable, no indirect call, no extra pointer per
  object.
- **Full inlining** — the compiler sees the concrete function body and can
  optimise across the call boundary.
- **Smaller objects** — without a vptr, trivial classes can be as small as
  1 byte (the minimum required by the standard).
- **Constexpr-friendly** — because everything is resolved at compile time,
  CRTP methods can participate in constant expressions.

**Trade-offs**

- **No heterogeneous containers** — you cannot mix `Dog` and `Cat` in a
  single container through a common base pointer (without type erasure or
  `std::variant`).
- **Template complexity** — error messages can be long and hard to read;
  the pattern itself is non-obvious to newcomers.
- **Longer compile times** — every unique derived type causes a new
  template instantiation.
- **Binary size** — heavy template use can increase the size of the
  compiled binary due to multiple instantiations.
- **Less flexible at run time** — the concrete type must be known at
  compile time; you cannot choose a derived class based on user input
  without additional machinery.

---

## D. Refactoring: Virtual to CRTP

Consider a traditional virtual hierarchy:

```cpp
class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const = 0;
    virtual std::string sound() const = 0;
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!\n"; }
    std::string sound() const override { return "Woof!"; }
};
```

To convert this to CRTP:

1. **Replace the abstract base class** with a class template parameterised
   on the derived type.
2. **Remove all `virtual` and `override`** keywords.
3. **Implement dispatch** in the base via `static_cast<const Derived*>(this)`.
4. **Rename derived implementations** to `speak_impl` / `sound_impl` (or
   any convention) so the base's public interface (`speak`, `sound`) can
   forward to them.
5. **Replace base-pointer-based free functions** with function templates
   that accept any type satisfying the interface.

The result:

```cpp
template <typename Derived>
class AnimalBase {
public:
    void speak() const {
        static_cast<const Derived*>(this)->speak_impl();
    }
    std::string sound() const {
        return static_cast<const Derived*>(this)->sound_impl();
    }
};

class Dog : public AnimalBase<Dog> {
public:
    void speak_impl() const { std::cout << "Woof!" << std::endl; }
    std::string sound_impl() const { return "Woof!"; }
};

template <typename Animal>
void make_it_speak(const Animal& a) {
    a.speak();
}
```

No virtual functions, no vtable, no indirection.

---

## E. Self-Check Questions

Test your understanding — try to answer without looking at the notes above.

1. What run-time mechanism does dynamic polymorphism use to dispatch
   virtual function calls?
2. Why can the compiler not usually inline a virtual function call made
   through a base-class pointer?
3. What does CRTP stand for, and what is the general shape of the pattern?
4. In a CRTP base class, why is `static_cast<const Derived*>(this)` safe?
5. Why are `Base<Dog>` and `Base<Cat>` unrelated types even though they
   share the same template?
6. Name two performance benefits of CRTP over virtual dispatch.
7. Why can you not store `Dog` and `Cat` objects in a single
   `std::vector<AnimalBase*>` when using CRTP?
8. What happens to `sizeof(Dog)` when you remove all virtual functions and
   the class has no data members?
9. When might you prefer dynamic polymorphism over CRTP despite the
   performance cost?
10. How would you add a new animal (e.g. `Cow`) to a CRTP-based hierarchy?
    What files need to change?
