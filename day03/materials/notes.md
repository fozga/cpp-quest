# Circular References — Study Notes

## A. `shared_ptr` Semantics

`std::shared_ptr<T>` is a reference-counted smart pointer. Multiple
`shared_ptr` instances can share ownership of the same heap object. Internally,
each `shared_ptr` points to:

1. **The managed object** — the `T` you allocated.
2. **A control block** — a small heap structure that stores the *strong
   reference count* (number of `shared_ptr` copies) and the *weak reference
   count* (number of `weak_ptr` copies, plus one if the strong count is
   non-zero).

When you copy a `shared_ptr`, the strong count increments. When a `shared_ptr`
is destroyed or reset, the strong count decrements. When it reaches **zero**,
the managed object's destructor runs and its memory is freed. The control block
itself survives until the weak count also reaches zero.

`use_count()` returns the current strong count. It is mainly useful for
debugging and testing — never make program-logic decisions based on its value
in production code (it is not atomic in a useful sense across threads without
additional synchronization).

Key creation idiom: prefer `std::make_shared<T>(args...)` over
`std::shared_ptr<T>(new T(args...))`. `make_shared` performs a single
allocation for both the object and the control block, improving cache locality
and exception safety.

---

## B. Cycles and Leaks

Consider two objects, `A` and `B`, where:

```
A.ptr  -->  B
B.ptr  -->  A
```

If both `ptr` members are `shared_ptr`, then:

- `A`'s strong count is at least 1 (held by `B.ptr`).
- `B`'s strong count is at least 1 (held by `A.ptr`).

Even after every *external* `shared_ptr` to `A` and `B` is destroyed, the
mutual pointers keep both counts at 1. Neither object is ever deleted.
Destructors never run. Memory leaks.

This generalizes to any cycle: `A → B → C → A`, a doubly-linked list where
both directions are `shared_ptr`, a parent holding `shared_ptr` to children
that hold `shared_ptr` back to the parent, and so on.

The insidious aspect is that the leak is **silent**. No crash, no segfault —
just a slow memory climb that may not be noticed until the process runs for
hours or days.

---

## C. `weak_ptr`

`std::weak_ptr<T>` is a non-owning observer of a `shared_ptr`-managed object.
It does **not** affect the strong reference count. Creating or destroying a
`weak_ptr` only touches the *weak* count in the control block.

### Key operations

| Operation | Effect |
|---|---|
| `weak_ptr<T> w(sp)` | Construct from a `shared_ptr`; increments weak count |
| `w.expired()` | Returns `true` if the managed object has been destroyed (strong count == 0) |
| `w.lock()` | If not expired, returns a `shared_ptr` that shares ownership (strong count incremented); otherwise returns an empty `shared_ptr` |
| `w.use_count()` | Returns the strong count (same value as the associated `shared_ptr::use_count()`) |
| `w.reset()` | Releases the observation; decrements weak count |

`lock()` is the primary safe-access pattern. The returned `shared_ptr`
guarantees the object stays alive for the duration of the locked scope:

```cpp
if (auto sp = weak.lock()) {
    // Object is alive — use sp safely.
} else {
    // Object has been destroyed.
}
```

Never dereference a `weak_ptr` directly — it has no `operator*` or
`operator->`. Always go through `lock()`.

---

## D. Design Pattern: Breaking Cycles

The standard recipe is straightforward:

1. **Choose an owning direction.** In a parent–child tree, the parent owns
   children via `shared_ptr`. In a linked list, forward links (`next`) own via
   `shared_ptr`.
2. **Use `weak_ptr` for the reverse direction.** Children hold `weak_ptr` to
   parents. Back-links (`prev`) use `weak_ptr`.
3. **Provide explicit cleanup when the topology requires it.** In a ring
   (circular list), even with `prev` as `weak_ptr`, the ring of `next`
   pointers still forms a cycle. You must break one link before the nodes can
   be freed.

### Common scenarios

- **Doubly-linked list (linear):** `next` = `shared_ptr`, `prev` = `weak_ptr`.
  Dropping the head frees the entire chain automatically.
- **Circular doubly-linked list:** Same pointer types, but the last node's
  `next` points back to the first. You must null that closing link before
  releasing external references.
- **Parent–child tree with back-pointers:** Parent holds
  `vector<shared_ptr<Child>>`, child holds `weak_ptr<Parent>`.
- **Observer / listener pattern:** The subject holds `vector<weak_ptr<Observer>>`,
  observers hold `shared_ptr<Subject>` (or a raw pointer if the subject's
  lifetime is guaranteed).

---

## E. Self-Check Questions

Answer these without looking at the notes above. If you get stuck, re-read the
relevant section.

1. What two pieces of data does a `shared_ptr` control block store?
2. When is the managed object destroyed? When is the control block destroyed?
3. If three `shared_ptr` instances and two `weak_ptr` instances share the
   same object, what does `use_count()` return?
4. Draw (on paper or in your head) the pointer graph for a doubly-linked list
   of three nodes where both `next` and `prev` are `shared_ptr`. Why does
   dropping all external pointers still leak?
5. What happens if you call `lock()` on a `weak_ptr` whose managed object has
   already been destroyed?
6. Why is `expired()` followed by `lock()` a race condition in multi-threaded
   code? What should you do instead?
7. In a circular singly-linked list (`next` only, forming a ring), can
   `weak_ptr` alone solve the cycle? Why or why not?
8. Explain why `make_shared` is preferred over constructing a `shared_ptr`
   from a raw `new` expression.
9. A tree node holds `shared_ptr<Node>` to its children and `shared_ptr<Node>`
   to its parent. Describe the minimal change to break the cycle.
10. After calling `clear_list` on a circular list, what should
    `get_live_node_count()` return, and why?
