# Asynchronous Results — Links

Curated references for Day 25: `std::promise`, `std::future`, and one-shot async channels.

- [std::promise — cppreference.com](https://en.cppreference.com/w/cpp/thread/promise)
  Complete API reference for `std::promise`, including `set_value`, `set_exception`, and `get_future`.

- [std::future — cppreference.com](https://en.cppreference.com/w/cpp/thread/future)
  Complete API reference for `std::future`, covering `get()`, `wait()`, `valid()`, and `share()`.

- [std::async — cppreference.com](https://en.cppreference.com/w/cpp/thread/async)
  Higher-level wrapper that internally creates a promise/future pair; useful for comparison.

- [Futures and Promises — Bo Qian (YouTube)](https://www.youtube.com/watch?v=SZQ6-pf-5Us)
  Concise video walkthrough of the promise/future mechanism with practical examples.

- [C++ Concurrency in Action, Ch. 4 — Anthony Williams](https://www.manning.com/books/c-plus-plus-concurrency-in-action)
  In-depth treatment of futures, promises, and packaged tasks in the context of real-world concurrency patterns.
