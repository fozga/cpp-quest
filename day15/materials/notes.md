# Advanced Lambdas — Study Notes

## A. Lambdas as Closure Objects

Every lambda expression causes the compiler to generate an unnamed class (the
*closure type*). The class has:

- An `operator()` whose parameter list and body match the lambda.
- Data members corresponding to each captured variable.

```cpp
int x = 5;
auto fn = [x](int y) { return x + y; };
// roughly equivalent to:
// struct __lambda_1 {
//     int x;
//     int operator()(int y) const { return x + y; }
// };
```

Because the closure is a regular object, it can be stored, copied, and passed
around like any other value.

## B. Capture-by-Value vs Capture-by-Reference

| Mode | Syntax | Lifetime | Mutation |
|---|---|---|---|
| By value | `[x]` | Independent copy | Not allowed (operator is `const`) |
| By reference | `[&x]` | Alias to original | Allowed; changes visible outside |
| Default value | `[=]` | Copies all used | Same as `[x]` |
| Default ref | `[&]` | References all used | Same as `[&x]` |

**Lifetime pitfall:** a lambda that captures by reference can outlive the
referenced variable, producing a dangling reference. This is a common source of
undefined behaviour when lambdas are returned from functions or stored in
containers.

```cpp
std::function<int()> make_counter_BAD() {
    int n = 0;
    return [&n]() { return ++n; };   // BUG: n destroyed on return
}
```

## C. Mutable Lambdas

By default, the compiler-generated `operator()` is `const`, so captured-by-value
members cannot be modified:

```cpp
int n = 0;
auto inc = [n]() { ++n; };          // ERROR: n is const inside operator()
```

Adding `mutable` removes the `const` qualifier from `operator()`, allowing the
lambda to modify its *internal copies*. The original variables are untouched:

```cpp
int n = 0;
auto inc = [n]() mutable { return ++n; };
inc();   // returns 1 (internal copy is now 1)
inc();   // returns 2
// n is still 0 here
```

## D. Capture Initializers (C++14)

A capture initializer creates a new data member in the closure, initialised by an
arbitrary expression. This enables:

- **Moving** a resource into the closure:

  ```cpp
  auto ptr = std::make_unique<Widget>();
  auto fn = [p = std::move(ptr)]() { p->do_work(); };
  ```

- **Computing** a derived value at capture time:

  ```cpp
  double base = 3.14;
  auto fn = [diameter = 2.0 * base]() { return diameter; };
  ```

- **Renaming** for clarity:

  ```cpp
  auto fn = [&out = std::cerr]() { out << "debug\n"; };
  ```

The initialiser expression is evaluated once, when the lambda is created.

## E. Lambdas with Internal State and Copying

A mutable lambda with captured-by-value state behaves like a small stateful
object. Copying the lambda copies all its data members, producing an independent
clone whose state subsequently diverges:

```cpp
auto counter = [n = 0]() mutable { return ++n; };
counter();           // 1
counter();           // 2

auto counter2 = counter;   // copies current state (n == 2)
counter();           // 3
counter2();          // 3  — same value, independent from here on
counter2();          // 4
counter();           // 4  — each has its own n
```

When state must be *shared* rather than copied, capture a `std::shared_ptr`
instead; all copies of the lambda then point to the same heap-allocated state.

## F. Self-Check Questions

1. What does the compiler generate when it encounters a lambda expression?
2. Why is the call operator of a lambda `const` by default?
3. What happens if you capture a local variable by reference and the lambda
   outlives that variable?
4. How does adding `mutable` change the closure's call operator?
5. If a mutable lambda captures `n` by value and you call it three times, what is
   the value of the *original* `n`?
6. Write a capture initializer that moves a `std::unique_ptr<int>` into a lambda.
7. What happens to the internal state when you copy a mutable lambda?
8. How can two copies of a lambda *share* the same mutable state?
9. What is the type of the object returned by a lambda expression — can you name
   it explicitly?
10. Why might you prefer `std::function` over `auto` when storing a lambda in
    a struct member or returning it from a function?
