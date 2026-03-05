# Lock-Free Basics — Notes

## A. `std::atomic` Basics

`std::atomic<T>` wraps a type `T` and ensures that every read and write is
**indivisible** — no thread can observe a half-written value. The compiler and
CPU are also prevented from reordering atomic accesses in ways that would break
the guarantees specified by the chosen memory order.

Key operations:

| Operation | Example | Description |
|---|---|---|
| `load()` | `int v = a.load();` | Atomically reads the value. |
| `store(val)` | `a.store(42);` | Atomically writes a value. |
| `exchange(val)` | `int old = a.exchange(5);` | Atomically writes and returns the previous value. |
| `fetch_add(n)` | `a.fetch_add(1);` | Atomically adds `n` and returns the old value. |
| `fetch_sub(n)` | `a.fetch_sub(1);` | Atomically subtracts `n` and returns the old value. |
| `compare_exchange_weak` | See below | CAS — the foundation of lock-free algorithms. |
| `compare_exchange_strong` | See below | Like weak, but never fails spuriously. |

`std::atomic` is **lock-free** for most integral and pointer types on modern
hardware (check with `a.is_lock_free()`).

## B. Compare-And-Swap (CAS)

CAS is the building block of lock-free algorithms. The signature is:

```cpp
bool compare_exchange_weak(T& expected, T desired,
                           memory_order success,
                           memory_order failure);
```

1. Atomically compares the stored value with `expected`.
2. If equal, replaces the stored value with `desired` and returns `true`.
3. If not equal, loads the current value into `expected` and returns `false`.

**`weak` vs `strong`**: `compare_exchange_weak` may fail *spuriously* — it can
return `false` even when the stored value equals `expected`. This is cheaper on
some architectures (LL/SC). Use `weak` inside a loop (the spurious failure is
harmless because you retry). Use `strong` when you need a single attempt to be
definitive.

### CAS Loop Pattern

```cpp
int expected = atom.load();
while (!atom.compare_exchange_weak(expected, expected + 1)) {
    // `expected` is automatically reloaded on failure — just retry
}
```

This pattern is **lock-free**: if one thread's CAS fails, it is because another
thread succeeded, so system-wide progress is always made.

## C. Progress Guarantees

| Guarantee | Definition |
|---|---|
| **Wait-free** | Every thread finishes in a bounded number of steps. |
| **Lock-free** | At least one thread makes progress in a finite number of steps. |
| **Obstruction-free** | A thread makes progress if all other threads are suspended. |

A CAS loop is lock-free but not wait-free: one thread can theoretically starve
if others continuously succeed first. `fetch_add` is typically wait-free on x86.

## D. Self-Check Questions

1. What does "indivisible" mean in the context of `std::atomic` operations?
2. Why is reading a plain `int` from one thread while another writes to it undefined behaviour?
3. What are the two possible outcomes when `compare_exchange_weak` is called?
4. Why does `compare_exchange_weak` update the `expected` parameter on failure?
5. When would you prefer `compare_exchange_strong` over `compare_exchange_weak`?
6. In a CAS loop, why is the algorithm considered lock-free?
7. What is the difference between lock-free and wait-free?
8. How can you check at runtime whether a particular `std::atomic<T>` is truly lock-free?
9. Why is `fetch_add` often preferred over a CAS loop for simple increments?
10. Could a CAS loop starve a single thread indefinitely? Under what conditions?
