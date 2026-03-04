# Memory Pooling — Links & References

Supplementary reading on custom allocators, memory pooling, and
`std::allocate_shared`.

- **CppReference — Allocator requirements**
  <https://en.cppreference.com/w/cpp/named_req/Allocator>
  The definitive reference for what a conforming allocator must provide.

- **CppReference — `std::allocate_shared`**
  <https://en.cppreference.com/w/cpp/memory/shared_ptr/allocate_shared>
  Specification and examples showing how `allocate_shared` uses a custom
  allocator to fuse the control block and object into one allocation.

- **Pablo Halpern — "Allocators: The Good Parts" (CppCon 2017)**
  <https://www.youtube.com/watch?v=v3dz-AKOVL8>
  Accessible talk covering the evolution of C++ allocators and practical
  guidance on writing your own.

- **Bob Steagall — "How to Write a Custom Allocator" (CppCon 2017)**
  <https://www.youtube.com/watch?v=kSWfushlvB8>
  Step-by-step walkthrough of building a minimal allocator that works with
  standard containers and smart pointers.

- **Boost.Pool Library**
  <https://www.boost.org/doc/libs/release/libs/pool/doc/html/index.html>
  Production-quality pool allocator family; useful as a reference for what a
  real pool implementation looks like.

- **CppReference — `std::pmr::monotonic_buffer_resource`**
  <https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource>
  The standard library's built-in arena-style allocator resource, introduced
  in C++17, which solves a similar problem to our `SimplePool`.
