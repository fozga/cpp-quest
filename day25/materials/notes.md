# Asynchronous Results — Notes

## A. `std::promise` and `std::future`

A **promise** is the write end of a one-shot channel; a **future** is the read end.
The promise holds either a value or an exception. Calling `get_future()` on a
promise returns the unique future bound to it — each promise can produce exactly
one future.

```cpp
std::promise<int> prom;
std::future<int> fut = prom.get_future();   // bound pair

prom.set_value(42);          // producer writes
int result = fut.get();      // consumer reads (blocks until ready)
```

The future's `get()` call blocks the calling thread until the promise has been
fulfilled. If the promise is destroyed without setting a value, `get()` throws
`std::future_error` with `broken_promise`.

## B. Life Cycle

1. **Producer** creates a `std::promise<T>`.
2. Producer calls `prom.get_future()` to obtain the paired `std::future<T>`.
3. Producer **moves** (or shares via `shared_ptr`) the promise into a worker thread.
4. The worker computes a result and calls `prom.set_value(result)` — or
   `prom.set_exception(std::current_exception())` on failure.
5. **Consumer** calls `fut.get()`, which blocks until the value (or exception) is
   available, then returns it (or rethrows).

When using a detached thread, the promise must remain alive until `set_value` is
called. A common pattern is to wrap it in a `std::shared_ptr<std::promise<T>>` and
capture that shared pointer in the thread's lambda.

## C. Single-Use Semantics

- `future::get()` may only be called **once**. After the call the future is no
  longer valid (`future::valid()` returns `false`).
- `promise::set_value()` or `promise::set_exception()` may only be called **once**.
  A second call throws `std::future_error` with `promise_already_satisfied`.
- If you need multiple consumers to read the same result, convert the future to a
  `std::shared_future<T>` via `future::share()`.

## D. Avoiding Dangling References

When a function creates a local vector and passes it to a detached thread by
reference, the vector may be destroyed before the thread reads it. Always **copy**
(or move) the data into the thread's lambda capture:

```cpp
std::thread([prom, vals = values]() {   // vals is a copy
    int sum = std::accumulate(vals.begin(), vals.end(), 0);
    prom->set_value(sum);
}).detach();
```

## E. Exception Propagation

A promise can transport an exception to the consumer thread:

```cpp
try {
    // risky computation ...
    prom.set_value(result);
} catch (...) {
    prom.set_exception(std::current_exception());
}
```

The consumer's `fut.get()` rethrows the stored exception as if it were thrown
locally.

---

## Self-Check Questions

1. What happens if you call `future::get()` a second time on the same future?
2. What exception is thrown if a promise is destroyed without calling `set_value()` or `set_exception()`?
3. Why must a promise be **moved** (not copied) into a worker thread?
4. How does `std::shared_future` differ from `std::future`?
5. What happens if you call `set_value()` twice on the same promise?
6. Why is it dangerous to capture a local variable by reference in a detached thread's lambda?
7. How does wrapping a promise in `std::shared_ptr` help with detached threads?
8. Can `future::get()` return before the promise has been fulfilled? Why or why not?
9. What is the role of `std::current_exception()` when used with `set_exception()`?
10. If you need the result of an async computation in multiple places, what should you use instead of `std::future`?
