# Custom Deleters — Study Notes

## A. Recap: RAII and `unique_ptr` as a General Wrapper

In Day 1 we wrote a full RAII class for a file descriptor: constructor acquires
the resource, destructor releases it, and the Rule of Five ensures safe
copy/move semantics.  That is idiomatic C++, but writing five special members
for every resource type gets tedious.

`std::unique_ptr` already implements unique-ownership RAII for heap memory.  By
supplying a **custom deleter** we can reuse all of that machinery — move
semantics, scope-based destruction, `reset()`, `release()` — for *any*
resource, not just `new`/`delete` pairs.

## B. Anatomy of `unique_ptr<T, Deleter>`

```cpp
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr;
```

- **`T`** — the pointed-to type.  For a file descriptor we use `int`; the
  `unique_ptr` will manage an `int*` that points to a heap-allocated fd.
- **`Deleter`** — a callable type invoked when the pointer is destroyed or
  reset.  The default is `std::default_delete<T>`, which calls `delete`.

### What happens on destruction

1. If the stored pointer is non-null, the deleter is invoked: `deleter(ptr)`.
2. After the deleter returns, the `unique_ptr` itself is trivially destroyed.

### Size implications

| Deleter kind | Typical `sizeof(unique_ptr)` | Why |
|---|---|---|
| Stateless functor (empty struct) | `sizeof(T*)` — same as a raw pointer | Empty-base optimisation eliminates the deleter member. |
| Captureless lambda | `sizeof(T*)` | The lambda type is empty, same optimisation applies. |
| Function pointer (`void(*)(T*)`) | `2 × sizeof(T*)` | The pointer to the deleter function must be stored alongside the data pointer. |
| Stateful functor / capturing lambda | `sizeof(T*) + sizeof(state)` | The captured state is stored inside the `unique_ptr`. |

Prefer stateless functors or captureless lambdas when zero overhead matters.

## C. Common Deleter Patterns

### 1. Function pointer

```cpp
void close_fd(int* fd) { if (fd) { ::close(*fd); delete fd; } }
using FdPtr = std::unique_ptr<int, void(*)(int*)>;
FdPtr p(new int(open("file", O_RDONLY)), close_fd);
```

Straightforward, but the `unique_ptr` is pointer-sized larger and the function
pointer prevents inlining in some compilers.

### 2. Stateless functor (struct with `operator()`)

```cpp
struct FdDeleter {
    void operator()(int* fd) const noexcept {
        if (fd) { ::close(*fd); delete fd; }
    }
};
using FdPtr = std::unique_ptr<int, FdDeleter>;
FdPtr p(new int(open("file", O_RDONLY)));   // deleter is default-constructed
```

Zero-overhead, inlineable, and the deleter is part of the type — no need to
pass it at construction.  This is the pattern we use in today's exercise.

### 3. Lambda with `decltype`

```cpp
auto deleter = [](int* fd) { if (fd) { ::close(*fd); delete fd; } };
using FdPtr = std::unique_ptr<int, decltype(deleter)>;
FdPtr p(new int(open("file", O_RDONLY)), deleter);
```

Concise for one-off cases.  In C++20 you can use a captureless lambda in an
unevaluated context, making this slightly cleaner.  Still zero-overhead when
captureless.

### When to prefer each

- **Functor struct** — default choice for library code; self-documenting, zero
  overhead, works naturally with type aliases.
- **Lambda** — handy in local scope or tests when defining a struct feels heavy.
- **Function pointer** — necessary when the deleter must be chosen at runtime
  or comes from a C API callback.

## D. `unique_ptr<int, Deleter>` as a File Descriptor Wrapper

### Trade-offs vs a dedicated class

| Aspect | `unique_ptr` + deleter | Dedicated RAII class |
|---|---|---|
| Lines of code | Very few | 50-100+ (Rule of Five) |
| Custom interface | Limited to `unique_ptr` API (`get`, `reset`, `release`) | Fully bespoke (`read()`, `write()`, `seek()`) |
| Type safety | Wraps a raw `int*`, easy to mis-use the dereferenced value | Can enforce invariants in every member function |
| Move semantics | Free (built into `unique_ptr`) | Must implement manually |

For quick RAII wrappers around simple C handles, the `unique_ptr` approach is
often good enough.  For richer abstractions, a dedicated class is better.

### Convenience helpers

A factory function like `make_file_handle(name)` hides the allocation and
deleter plumbing.  Query functions like `is_valid_handle` and `get_handle`
provide a thin, safe interface over the raw `unique_ptr`.

## E. Self-Check Questions

1. What is the second template parameter of `std::unique_ptr` and what role
   does it play?
2. Why does a stateless-functor deleter result in zero size overhead while a
   function-pointer deleter does not?
3. If you forget to `delete` the raw pointer inside your custom deleter, what
   happens?
4. Can you copy a `unique_ptr`?  Why or why not?
5. What does `unique_ptr::release()` do, and how does it differ from
   `unique_ptr::reset()`?
6. When `make_file_handle` throws after `open_pseudo_file` but before the
   `unique_ptr` is constructed, is the descriptor leaked?  How would you
   guard against this?
7. Why is the deleter's `operator()` marked `noexcept`?
8. In what scenario would you choose a capturing-lambda deleter over a
   stateless functor?
9. How does `std::move` interact with `unique_ptr` ownership?
10. If two `unique_ptr` objects point to the same resource (double ownership),
    what goes wrong at destruction time?
