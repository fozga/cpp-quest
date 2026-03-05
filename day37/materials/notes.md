# Dependency Inversion — Notes

## A. The Dependency Inversion Principle

The DIP is the "D" in SOLID. It has two parts:

1. **High-level modules should not depend on low-level modules.** Both should depend on abstractions.
2. **Abstractions should not depend on details.** Details should depend on abstractions.

In practice this means your business logic (`OrderProcessor`) never mentions a concrete logger class directly. Instead it accepts a reference or pointer to an abstract `Logger` interface. The concrete implementation (`ConsoleLogger`, `FileLogger`, etc.) is chosen by the caller — typically `main()` or a composition root.

## B. Ports and Adapters

* A **port** is an interface (abstract base class) that defines a contract your core logic needs — e.g. "something that can log messages".
* An **adapter** is a concrete class that fulfils that contract for a specific technology — e.g. writing to `stdout`, a file, a network socket, or an in-memory buffer.

The core module only knows about the port. Adapters are plugged in from the outside, keeping the core free of I/O details.

## C. Constructor Injection in C++

The most common way to supply a dependency is through the constructor:

```cpp
class OrderProcessor {
public:
    explicit OrderProcessor(Logger& logger);
    // ...
private:
    Logger& logger_;
};
```

Storing a reference means the caller owns the logger's lifetime. This is simple and avoids heap allocation. Alternatively you can accept `std::unique_ptr<Logger>` when the class should own the dependency, or `std::shared_ptr<Logger>` for shared ownership.

## D. Benefits of DIP

* **Testability** — swap in a fake or mock without touching production code.
* **Modularity** — add new adapters (e.g. `JsonFileLogger`) without changing the core.
* **Reduced coupling** — high-level and low-level modules evolve independently.
* **Clearer architecture** — the dependency graph points inward toward abstractions.

## E. Pure-Virtual Interfaces in C++

An abstract base class with only pure-virtual methods serves as an interface:

```cpp
struct Logger {
    virtual ~Logger() = default;
    virtual void info(const std::string& msg) = 0;
};
```

The virtual destructor ensures derived objects are destroyed correctly when deleted through a base pointer. The `= 0` makes the method pure-virtual, preventing direct instantiation of `Logger`.

---

## Self-Check Questions

1. What are the two parts of the Dependency Inversion Principle?
2. Why should a high-level module avoid instantiating its own dependencies?
3. What is the difference between a port and an adapter?
4. Why does the `Logger` struct need a virtual destructor?
5. What does `= 0` on a virtual method declaration mean?
6. How does constructor injection differ from setter injection?
7. Why is storing a reference (`Logger&`) simpler than storing a `unique_ptr<Logger>` for dependency injection?
8. How does DIP improve testability?
9. Can you add a new logging backend (e.g. to a file) without modifying `OrderProcessor`? Why?
10. What would break if `OrderProcessor` directly constructed a `ConsoleLogger` inside its own constructor?
