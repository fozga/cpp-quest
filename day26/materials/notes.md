# Deadlock Prevention — Study Notes

## A. The Four Coffman Conditions

A deadlock can occur only when **all four** conditions hold simultaneously:

| # | Condition | Meaning |
|---|---|---|
| 1 | **Mutual exclusion** | At least one resource is held in a non-sharable mode. |
| 2 | **Hold and wait** | A thread holds one resource while waiting to acquire another. |
| 3 | **No preemption** | Resources cannot be forcibly taken from a thread; only the holder can release. |
| 4 | **Circular wait** | A cycle exists in the wait-for graph: T1 waits for T2, T2 waits for T1 (or longer chains). |

Breaking **any one** of these conditions prevents deadlock. In practice, the
most common strategy in C++ is to eliminate *circular wait* through consistent
lock ordering.

## B. Lock Ordering and `std::scoped_lock`

### Manual lock ordering

If every thread always acquires mutex A before mutex B, a circular wait cannot
form between A and B. However, maintaining a manual ordering discipline across a
large codebase is fragile and error-prone.

### `std::scoped_lock` (C++17)

`std::scoped_lock` accepts any number of `Lockable` objects and locks them all
using a deadlock-avoidance algorithm (internally equivalent to `std::lock`):

```cpp
std::scoped_lock lock(mutex_a, mutex_b);
// both mutexes held — released automatically at scope exit
```

Key properties:

- **Deadlock-free**: uses a try-and-back-off strategy so threads never form a circular wait.
- **Exception-safe**: if construction succeeds, the destructor releases all locks (RAII).
- **Zero-overhead for a single mutex**: specialised to behave like `lock_guard` when given one mutex.
- **Variadic**: works with two, three, or more mutexes in a single call.

### `std::lock_guard` vs `std::scoped_lock`

| | `std::lock_guard` | `std::scoped_lock` |
|---|---|---|
| Number of mutexes | Exactly one | Zero or more |
| Deadlock avoidance | N/A (single lock) | Built-in for multiple locks |
| C++ standard | C++11 | C++17 |

Prefer `scoped_lock` for any new code. Use `lock_guard` only when you need to
lock exactly one mutex and want to be explicit about it.

## C. Banking Transfer Example

Consider two accounts, A and B, and two threads:

```
Thread 1: transfer(A, B, 50)   →  lock A, then lock B
Thread 2: transfer(B, A, 30)   →  lock B, then lock A
```

Without `scoped_lock`, Thread 1 holds A and waits for B while Thread 2 holds B
and waits for A — **deadlock**. With `scoped_lock`:

```cpp
void transfer(Account& from, Account& to, int amount) {
    std::scoped_lock lock(from.mtx, to.mtx);   // safe order chosen internally
    if (from.balance >= amount) {
        from.balance -= amount;
        to.balance += amount;
    }
}
```

Both mutexes are acquired atomically in a deadlock-free manner regardless of the
argument order.

## D. Self-Check Questions

1. Name the four Coffman conditions for deadlock.
2. Which Coffman condition does `std::scoped_lock` break, and how?
3. What happens if you replace `std::scoped_lock` with two separate `lock_guard` calls in the transfer function?
4. Why is `Account::mtx` declared `mutable`?
5. Can `std::scoped_lock` lock more than two mutexes at once?
6. What is the difference between `std::lock` and `std::scoped_lock`?
7. Why does the `transfer` function check `from.balance >= amount` inside the lock rather than before locking?
8. If one thread calls `transfer(A, B, 10)` and another calls `transfer(A, B, 20)` simultaneously, is deadlock possible? Why or why not?
9. Why are accounts stored as `std::unique_ptr<Account>` in a vector instead of directly as `Account` objects?
10. What guarantees does `std::scoped_lock`'s destructor provide if an exception is thrown while the locks are held?
