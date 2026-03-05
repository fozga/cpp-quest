# Modern Threads — Study Notes

## A. `std::thread` vs `std::jthread`

| | `std::thread` (C++11) | `std::jthread` (C++20) |
|---|---|---|
| Destruction of a joinable thread | Calls `std::terminate` | Calls `request_stop()` then `join()` |
| Stop token support | None | Built-in `std::stop_source` / `std::stop_token` |
| Must remember to join/detach | Yes | No — handled automatically |
| Callable signature | `void()` | `void()` **or** `void(std::stop_token)` |

`std::jthread` is a drop-in upgrade for most uses of `std::thread`. The key advantage
is safety: if an exception causes early scope exit, the `jthread` destructor still
joins the thread instead of terminating the program.

## B. `std::stop_token` — Cooperative Cancellation

When you pass a callable that takes `std::stop_token` as its first parameter,
`jthread` automatically wires it up:

```cpp
std::jthread worker([](std::stop_token st) {
    while (!st.stop_requested()) {
        // do work
    }
});
// Later — or automatically in ~jthread:
worker.request_stop();   // signals the token
// ~jthread joins after requesting stop
```

Key members:

- `stop_token::stop_requested()` — returns `true` once stop has been requested.
- `jthread::request_stop()` — asks the thread to stop cooperatively.
- `jthread::get_stop_token()` — retrieves the token externally.

The pattern is *cooperative*: the running thread must check the token; nothing
forcibly kills the thread.

## C. Typical Worker Loop Pattern

```cpp
void run(std::stop_token st) {
    while (!st.stop_requested()) {
        std::this_thread::sleep_for(interval);
        if (st.stop_requested()) break;   // re-check after waking
        do_work();
        ++counter;
    }
}
```

The double-check around `sleep_for` prevents one extra iteration after a stop is
requested while the thread was sleeping.

## D. Self-Check Questions

1. What happens when a joinable `std::thread` is destroyed without calling `join()` or `detach()`?
2. How does `std::jthread`'s destructor differ from `std::thread`'s?
3. What is the signature a callable must have to receive an automatic `std::stop_token` from `jthread`?
4. Who owns the `std::stop_source` inside a `jthread`?
5. Why is the cancellation model called *cooperative*?
6. What happens if you call `request_stop()` while the worker thread is inside `sleep_for()`?
7. Why is `std::atomic<std::size_t>` preferred over a `std::mutex`-protected counter for a simple increment?
8. Can you move a `jthread`? Can you copy one?
9. If the callable throws an exception, what happens to the `jthread`?
10. Why do we re-check `stop_requested()` immediately after `sleep_for()` returns?
