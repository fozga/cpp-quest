# Custom Views — Notes

## A. What Is a View (Recap)

A view is a non-owning, lightweight wrapper over a range. Views are cheap to copy and move because they do not own the underlying elements. The standard library provides `std::ranges::view_interface` as a CRTP base class that automatically derives convenience members (`empty`, `operator bool`, `front`, `back`, `operator[]`) from a derived class that supplies `begin()` and `end()`.

A type satisfies `std::ranges::view` if it is movable, default-initializable, and models `std::ranges::range` with O(1) move and destruction.

## B. View Adaptors vs View Types

A **view type** is a class that stores state (such as an underlying range and a predicate) and exposes `begin()` / `end()`. Examples include `std::ranges::filter_view` and `std::ranges::transform_view`.

A **view adaptor** is a callable that takes a range (and possibly extra arguments) and returns a view. Adaptors are what you use with the pipe (`|`) operator. For example, `std::views::filter(pred)` is an adaptor that returns a `filter_view`. You can write your own adaptor as a function template that wraps standard view machinery.

## C. Minimal Custom View

The simplest way to create a custom view adaptor is to write a function template that pipes the input range through one or more standard views:

```cpp
template <std::ranges::input_range R>
auto my_adaptor(R&& rng) {
    return std::forward<R>(rng)
         | std::views::filter(/* predicate */)
         | std::views::transform(/* projection */);
}
```

This approach reuses all the iterator and sentinel logic from the standard views — no need to implement `begin()` / `end()` yourself.

For more advanced cases, you can define a class that inherits from `view_interface`, stores the underlying range, and provides custom iterators. This is necessary when the view logic cannot be expressed as a composition of existing standard views.

## D. Lazy Evaluation

Views evaluate lazily: elements are produced on demand as the caller iterates. This makes views suitable for infinite sequences. `std::views::iota(n)` produces an unbounded sequence starting at `n`. Piping it through a filter and then `std::views::take(k)` yields exactly `k` matching elements without ever materialising the full sequence.

Because `take` on some implementations expects a signed difference type, you may need to cast unsigned counts to `std::ptrdiff_t` when composing with `take`.

## E. Self-Check Questions

1. What makes a range qualify as a view according to the C++20 standard?
2. What is the role of `view_interface`, and which member functions can it auto-derive?
3. How does a view adaptor differ from a view type?
4. Why is it important that views are non-owning and cheap to copy?
5. What happens when you pipe `std::views::iota(1)` through a filter without a `take` — does it terminate?
6. Why might `std::views::take` require a signed argument on some implementations?
7. How does lazy evaluation allow filtering over an infinite sequence to remain efficient?
8. When would you need to write a full custom view class instead of composing standard adaptors?
9. What concept should you use to constrain a template parameter that must be a range you can iterate at least once?
10. If you convert a lazy view to a `std::vector`, at what point are the elements actually computed?
