# Deadlock Prevention

## Day Overview

A **deadlock** occurs when two or more threads each hold a lock and wait for the
other's lock, so none can make progress. Classic causes are inconsistent lock
ordering and circular wait chains. C++17's `std::scoped_lock` eliminates most
deadlocks by atomically acquiring multiple mutexes in a safe, consistent order
and releasing them automatically when the scope ends.

## Learning Objectives

- Recognise deadlock scenarios caused by inconsistent mutex acquisition order.
- Name the four Coffman conditions and explain why breaking any one prevents deadlock.
- Use `std::scoped_lock` to lock two or more mutexes simultaneously without deadlock.
- Understand why manual lock ordering is fragile and how `scoped_lock` improves on it.
- Implement a thread-safe money-transfer function between `Account` objects.
- Compute a consistent total balance across multiple accounts using per-account locking.
- Stress-test concurrent transfers and verify that the total balance is preserved.

## Task Summary

Build a simple banking simulation:

1. An `Account` struct holds a `std::mutex` and an `int balance`.
2. A `transfer(from, to, amount)` function locks both accounts with `std::scoped_lock` and moves money only if the sender has sufficient funds.
3. A `total_balance(accounts)` function sums all balances under individual locks.
4. A multi-threaded stress test spawns several threads performing thousands of random transfers, then asserts the total balance is unchanged.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| `Account` struct | Contains `mutable std::mutex mtx` and `int balance`. Constructed with an explicit initial balance. |
| `transfer()` | `void transfer(Account& from, Account& to, int amount)` — uses `std::scoped_lock` on both mutexes; transfers only if `from.balance >= amount`. |
| `total_balance()` | `int total_balance(const std::vector<std::unique_ptr<Account>>& accounts)` — locks each account individually, returns the sum. |
| Ownership | Accounts are held via `std::vector<std::unique_ptr<Account>>` since `Account` is non-copyable/non-movable (contains `std::mutex`). |
| Thread safety | No data races; `scoped_lock` prevents deadlock during transfers. |

## How to Run

From the repository root:

```bash
make day=26 test       # build and run tests
make day=26 asan       # AddressSanitizer build
make day=26 valgrind   # Valgrind memcheck
make day=26 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study the header `exercise.hpp` to understand the public interface.
3. Implement `transfer()` and `total_balance()` in `exercise.cpp`.
4. Run `make day=26 test` and iterate until all assertions pass.
5. Run the ASan and Valgrind targets to confirm no data races or leaks.
6. Answer the self-check questions in `notes.md` without peeking at references.
