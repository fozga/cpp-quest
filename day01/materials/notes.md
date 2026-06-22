# Day 01 — Notes: Pure RAII & Rule of 5

---

## A. RAII — Resource Acquisition Is Initialization

**RAII** is a C++ idiom where every resource (memory, file descriptor, lock,
socket, etc.) is bound to an object's lifetime:

- The **constructor** acquires (or receives) the resource.
- The **destructor** releases it.
- Because C++ guarantees that destructors run when an object goes out of
  scope — including during stack unwinding from an exception — the resource
  is released *exactly once*, *automatically*.

### Object Lifetime = Resource Lifetime

```text
{                          // scope begins
    FileHandle fh(open_pseudo_file("data.bin"));  // ctor acquires
    use(fh);               // resource is valid here
}                          // dtor runs → resource released
```

There is no matching "close" call for the programmer to remember. If an
exception is thrown anywhere inside the scope, the destructor still runs,
and the resource is still released.

### RAII and Exception Safety

A function is **exception-safe** when it does not leak resources or leave
the program in an invalid state if an exception is thrown. RAII provides
exception safety almost for free: because destructors are invoked during
stack unwinding, acquired resources cannot leak.

Three commonly cited exception-safety levels:

| Level | Guarantee |
|---|---|
| **No-throw** | The operation never throws (e.g., destructors, `swap`). |
| **Strong** | If the operation throws, the program state is unchanged. |
| **Basic** | If the operation throws, invariants are preserved but the state may have changed. |

### 1) No-Throw Guarantee (Strongest)

The operation is guaranteed not to emit exceptions.

What this means in practice:

- The caller never needs `try/catch` for that operation.
- The operation can be safely used during stack unwinding.
- It is ideal for cleanup paths (`~T()`, `close`, `reset`, `swap`).

Typical examples:

- Destructors should not throw.
- Move operations are often marked `noexcept` so containers can move elements
   during reallocation.
- `swap` for resource-owning types should be `noexcept` whenever possible.

Design note:

If an operation might fail internally, convert failure into a non-throwing
policy (log, error code, status flag) rather than throwing.

### 2) Strong Guarantee (Commit-or-Rollback)

If an exception occurs, the observable state is exactly as it was before the
operation started.

This is often called a **transactional** behavior:

- Either the operation fully succeeds (commit),
- or it fails and leaves no partial changes (rollback).

Common implementation pattern:

1. Build new state in temporaries (which may throw).
2. Commit changes with a no-throw step (usually `swap`).

Example sketch:

```cpp
void set_name(std::string s) {   // pass-by-value may throw before body runs
      name_.swap(s);               // no-throw commit step
}
```

If construction of `s` throws, `name_` is unchanged. If construction succeeds,
`swap` commits safely.

### 3) Basic Guarantee (Minimum Acceptable)

If an exception occurs:

- No resource leaks occur.
- Object invariants remain valid.
- But some state changes may already be visible.

In short, the object is still usable, but its exact value may differ from the
pre-call value.

Typical scenario:

- A multi-step update succeeds for early steps, then later allocation fails.
- The object still satisfies class invariants, but ends in a partially updated
   value.

This level is acceptable for many mutating operations when strong guarantee
would be too expensive.

### Mental Model

- **No-throw**: "cannot fail by exception"
- **Strong**: "all-or-nothing"
- **Basic**: "valid-but-possibly-changed"

When documenting APIs, explicitly state which guarantee each mutating function
provides. Ambiguity here leads to subtle bugs in caller code.

---

## B. The Rule of Three / Five / Zero

C++ can implicitly generate five **special member functions**:

| # | Function | Signature sketch |
|---|---|---|
| 1 | Destructor | `~T()` |
| 2 | Copy constructor | `T(const T&)` |
| 3 | Copy assignment operator | `T& operator=(const T&)` |
| 4 | Move constructor | `T(T&&) noexcept` |
| 5 | Move assignment operator | `T& operator=(T&&) noexcept` |

### Rule of Three (C++98)

If you define *any* of the destructor, copy constructor, or copy assignment
operator, you almost certainly need to define all three.

### Rule of Five (C++11)

Extend the Rule of Three by adding the move constructor and move assignment
operator. If you manage a resource that requires a custom destructor, define
(or delete) all five.

### Rule of Zero

If your class does **not** directly manage a raw resource — i.e., it only
holds members whose types already manage themselves (e.g., `std::string`,
`std::vector`, `std::unique_ptr`) — do **not** write any of the five special
members. Let the compiler generate them. This is the *preferred* design.

### When You Must Implement the Rule of 5

You must provide custom special members when your class directly owns a
**raw resource** that the compiler does not know how to copy, move, or
release correctly. Examples:

- A raw pointer obtained from `new` or `malloc`.
- A POSIX file descriptor (an `int` that must be `close()`-d).
- A handle from a C library (`sqlite3*`, `FILE*`, etc.).

---

## C. Design Guidelines for a RAII File Handle

### Interface

```cpp
class FileHandle {
    int fd_ = -1;            // -1 is the invalid sentinel
public:
    FileHandle() = default;  // default: invalid handle
    explicit FileHandle(int handle);
    ~FileHandle();

    FileHandle(const FileHandle&)            = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& other) noexcept;
    FileHandle& operator=(FileHandle&& other) noexcept;

    bool is_valid() const noexcept;
    int  get() const;
    void reset(int new_handle = -1);
};
```

### Factory (Simulated)

`open_pseudo_file(name)` returns a non-negative integer simulating a kernel
file descriptor. `close_pseudo_file(fd)` simulates releasing it.

### Destructor

Calls `close_pseudo_file(fd_)` if `fd_ != -1`. Must be `noexcept`
(destructors are implicitly `noexcept` in C++11 and later).

### Move Transfers Ownership

The move constructor steals the descriptor from the source and sets the
source's `fd_` to `-1`. After a move, the source is in a valid-but-empty
state. This mirrors how `std::unique_ptr` works.

### Copy Is Deleted — Unique Ownership

A file descriptor cannot be meaningfully duplicated without an OS call like
`dup()`. Because `FileHandle` models *unique ownership*, copying is
forbidden. Deleting copy operations prevents accidental double-close.

---

## D. Exception Safety Considerations

### No-Throw Destructor

The destructor must never throw. If `close_pseudo_file` could fail, the
destructor should swallow the error (e.g., log it). Throwing from a
destructor during stack unwinding calls `std::terminate`.

### Strong Guarantee for Assignment

Move assignment should release the old resource, then take ownership of the
new one. A safe pattern:

```cpp
FileHandle& operator=(FileHandle&& other) noexcept {
    if (this != &other) {
        reset();                    // release current resource
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}
```

Because the entire body is `noexcept`, no exception can leave the object in
a half-moved state.

### `noexcept` Move

Move operations should be `noexcept`. Standard containers (e.g.,
`std::vector`) will only use move operations during reallocation if they are
marked `noexcept`. Without the specifier, the container falls back to
copying — which, for a deleted-copy type, means a compile error.

---

## E. Self-Check Questions

Answer these from memory after studying the material above.

1. What does the acronym RAII stand for, and what two events does it bind
   to the object's constructor and destructor?
2. Why does RAII automatically provide a degree of exception safety?
3. List the five special member functions covered by the Rule of 5.
4. Under what circumstances should you follow the Rule of 0 instead of
   the Rule of 5?
5. Why is the copy constructor deleted in the `FileHandle` class rather
   than implemented?
6. What happens if a destructor throws an exception during stack unwinding?
7. Why should move constructors and move assignment operators be marked
   `noexcept`?
8. What value does `FileHandle` use as its invalid sentinel, and why is
   that value conventional for POSIX file descriptors?
9. Describe the steps that `reset(int new_handle)` must perform to avoid
   leaking the previously held resource.
10. If you removed the self-assignment check from `operator=(FileHandle&&)`,
    what bug could occur?
