# Move Constructors — Study Notes

## A. Value Categories

Every expression in C++ has a **value category**. The two you encounter most
often are:

- **lvalue** — an expression that refers to a persistent object with a name
  and an address you can take. Examples: variables, elements of an array,
  dereferenced pointers.
- **rvalue** — an expression that refers to a temporary or a value about to
  expire. Examples: integer literals, the return value of a function that
  returns by value, the result of `std::move(x)`.

### Rvalue References (`T&&`)

An **rvalue reference** is declared with `T&&`. It can bind to rvalues but not
to lvalues (without a cast). Its purpose is to *detect* that a value is
temporary so the program can steal its resources instead of copying them.

```cpp
int x = 42;
int&  lr = x;            // OK — lvalue reference binds to lvalue
int&& rr = 42;           // OK — rvalue reference binds to rvalue
// int&& bad = x;        // ERROR — rvalue reference cannot bind to lvalue
int&& ok  = std::move(x); // OK — std::move casts x to an rvalue
```

### Universal (Forwarding) References

In a template, `T&&` can also be a **universal reference** (also called a
forwarding reference) when `T` is deduced:

```cpp
template<typename T>
void f(T&& arg);  // universal reference — can bind to lvalue OR rvalue
```

This is different from a concrete type like `std::string&&`, which is always an
rvalue reference. The distinction matters for perfect forwarding but is beyond
the scope of today's exercise.

---

## B. Move Constructor and Move-Assignment Operator

### Signatures

```cpp
class Widget {
public:
    Widget(Widget&& other) noexcept;             // move constructor
    Widget& operator=(Widget&& other) noexcept;  // move-assignment operator
};
```

### Implementation Pattern: Steal and Null

The move constructor *steals* the source's resources and then sets the source
to a safe empty state:

```cpp
Widget::Widget(Widget&& other) noexcept
    : data_{other.data_}, size_{other.size_}
{
    other.data_ = nullptr;
    other.size_ = 0;
}
```

Move assignment must additionally clean up whatever the target currently owns:

```cpp
Widget& Widget::operator=(Widget&& other) noexcept {
    if (this != &other) {
        delete[] data_;           // release current resource
        data_ = other.data_;     // steal
        size_ = other.size_;
        other.data_ = nullptr;   // null source
        other.size_ = 0;
    }
    return *this;
}
```

### Effect on Implicit Generation

Declaring *any* of the following suppresses the compiler-generated move
operations:

- A user-declared copy constructor
- A user-declared copy-assignment operator
- A user-declared destructor

This is part of the **Rule of Five**: if you define any one of the five special
member functions (destructor, copy ctor, copy assign, move ctor, move assign),
you should define or explicitly default all five.

---

## C. Why `noexcept` Matters

The standard library relies on `noexcept` to decide whether it is safe to move
elements during operations like `std::vector::push_back` and
`std::vector::resize`.

- If the move constructor is **`noexcept`**, `std::vector` will move elements
  to new storage during reallocation. This is O(1) per element.
- If the move constructor **can throw**, `std::vector` falls back to *copying*
  elements, because a throwing move that fails partway through would leave the
  vector in an unrecoverable state.

**Always mark move constructors and move-assignment operators `noexcept`** when
they only perform pointer/integral assignments, which cannot throw.

---

## D. Dynamic Array Design

A minimal dynamic array manages three members:

| Member | Role |
|---|---|
| `T* data_` | Pointer to heap-allocated buffer |
| `std::size_t size_` | Number of live elements |
| `std::size_t capacity_` | Number of elements the buffer can hold |

### Push-back and Growth

When `size_ == capacity_`, `push_back` must allocate a larger buffer. A common
strategy is to double the capacity (with a minimum of 1 when the array is
empty):

```
new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
```

Steps:
1. Allocate `new T[new_cap]`.
2. Move (or copy) existing elements into the new buffer.
3. Construct the new element in the next slot.
4. `delete[]` the old buffer.
5. Update `data_`, `size_`, `capacity_`.

### Exception Safety

- The copy constructor allocates a new buffer and copies each element. If any
  copy throws, the partially filled buffer is leaked unless wrapped in
  RAII or cleaned up in a catch block.
- Move operations do not allocate, so they naturally provide the **nothrow
  guarantee** and should be marked `noexcept`.
- `push_back` should allocate the new buffer *before* modifying the array's
  state so that a failed allocation leaves the array unchanged (**strong
  guarantee**).

---

## E. Move vs Copy — Practical Examples

### Returning by Value

```cpp
DynamicArray<int> make_array() {
    DynamicArray<int> a{1, 2, 3};
    return a;  // compiler may apply NRVO; otherwise move ctor is used
}
```

Because the returned object is about to be destroyed, the compiler can elide
the copy entirely (NRVO) or, if elision is not possible, use the move
constructor.

### Explicit `std::move`

```cpp
DynamicArray<int> a{10, 20, 30};
DynamicArray<int> b = std::move(a);  // move ctor — a is now empty
```

After the move, `a` is in a valid-but-empty state: `a.size() == 0`,
`a.empty() == true`.

### Passing to a Function

```cpp
void consume(DynamicArray<int> arr);  // takes by value

DynamicArray<int> x{1, 2, 3};
consume(std::move(x));  // moves x into the parameter; x is empty afterward
```

---

## F. Self-Check Questions

1. What value category does the expression `x + 1` have, where `x` is an
   `int`?
2. Can an rvalue reference (`int&&`) bind directly to an lvalue without a
   cast?
3. What does `std::move(x)` actually do at runtime?
4. Why should move constructors be marked `noexcept`?
5. After `auto b = std::move(a);`, is it legal to call `a.size()`?
6. If a class defines a destructor but no move constructor, will the compiler
   generate an implicit move constructor?
7. In a `push_back` that needs to reallocate, should the new buffer be
   allocated before or after modifying the array's size?
8. What is the difference between `T&&` in `void f(std::string&& s)` and
   `T&&` in `template<typename T> void f(T&& s)`?
9. Why does `std::vector` copy elements during reallocation if the element's
   move constructor is not `noexcept`?
10. What is the Rule of Five, and how does it relate to move semantics?
