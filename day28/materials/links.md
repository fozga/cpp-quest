# Relaxed Atomics — Links

Introductory and reference material for C++ memory orderings.

- [cppreference — std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)
  Definitive reference for all six memory orderings with formal descriptions.

- [Preshing — An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/)
  Visual explanations of acquire, release, and relaxed semantics.

- [Preshing — Acquire and Release Semantics](https://preshing.com/20120913/acquire-and-release-semantics/)
  Deep dive into the acquire/release pattern with hardware context.

- [Herb Sutter — atomic<> Weapons (CppCon)](https://herbsutter.com/2013/02/11/atomic-weapons-the-c-memory-model-and-modern-hardware/)
  Two-part talk on the C++ memory model, hardware realities, and common mistakes.

- [Jeff Preshing — Memory Barriers Are Like Source Control Operations](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/)
  Intuitive analogy for understanding memory fences and barriers.

- [cppreference — std::atomic::fetch_add](https://en.cppreference.com/w/cpp/atomic/atomic/fetch_add)
  API reference for atomic read-modify-write with ordering parameter.
