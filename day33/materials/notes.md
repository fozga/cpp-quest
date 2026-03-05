# Debugging with Tools — Notes

## A. Sanitizers vs Valgrind

**AddressSanitizer (ASan)** is a *compiler-based* tool. You add
`-fsanitize=address` to both compile and link steps; the compiler inserts
instrumentation directly into the binary. Because the checks are inlined, ASan
is fast — typically a 2× slowdown compared to an uninstrumented build.

**Valgrind (Memcheck)** uses *dynamic binary instrumentation*. It translates the
compiled machine code at runtime into an intermediate representation, instruments
it, and executes the result on a synthetic CPU. No recompilation is needed, but
the overhead is large — around 10–30× slowdown.

| Aspect | ASan | Valgrind |
|--------|------|----------|
| Mechanism | Compiler instrumentation | Dynamic binary translation |
| Recompilation required | Yes | No |
| Typical slowdown | ~2× | ~10–30× |
| Memory overhead | ~3× address space | ~2× RAM |
| Detects leaks | Yes (LeakSanitizer) | Yes (--leak-check=full) |
| Detects uninitialised reads | No (use MSan for that) | Yes |
| Platform support | GCC, Clang (Linux, macOS) | Linux, macOS (x86/ARM) |

**Trade-offs**: ASan is better for fast iteration during development; Valgrind
is useful when you cannot recompile (third-party binaries) or need uninitialised-
value detection without a separate MSan build.

## B. Typical Memory Bugs

1. **Heap buffer overflow** — writing past the end of a dynamically allocated
   array. ASan reports `heap-buffer-overflow`; Valgrind reports
   `Invalid write of size N`.

2. **Use-after-free** — accessing memory after it has been deallocated. ASan
   reports `heap-use-after-free`; Valgrind reports `Invalid read of size N` with
   an "Address ... is ... inside a block of size ... free'd" note.

3. **Double delete** — calling `delete` or `delete[]` on a pointer that was
   already freed. ASan reports `attempting double-free`; Valgrind reports
   `Invalid free`.

4. **Memory leak** — allocating memory with `new` and never calling `delete`.
   ASan's integrated LeakSanitizer reports at exit; Valgrind's summary shows
   "definitely lost: N bytes".

5. **Stack buffer overflow** — writing past a local array. ASan detects this;
   Valgrind often cannot because it does not instrument stack accesses.

6. **Uninitialised memory read** — reading from allocated but never written
   memory. Valgrind (Memcheck) detects this; ASan does not (use MSan instead).

## C. Reading Tool Output

Both tools produce **stack traces** that show the call chain leading to the
error. To make those traces useful:

- Compile with **`-g`** to include debug symbols (file names and line numbers).
- Compile with **`-O0`** to disable optimisations that reorder or inline code,
  ensuring the reported line numbers match the source.
- Compile with **`-fno-omit-frame-pointer`** (recommended with ASan) so that
  stack unwinding is reliable.

### Example ASan output (simplified)

```
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x6020000000d8
WRITE of size 4 at 0x6020000000d8 thread T0
    #0 Buffer::push_back(int) exercise.cpp:40
    #1 main tests.cpp:12
```

### Example Valgrind output (simplified)

```
==12345== Invalid write of size 4
==12345==    at 0x40123A: Buffer::push_back(int) (exercise.cpp:40)
==12345==    by 0x401456: main (tests.cpp:12)
==12345==  Address 0x5a1f0d8 is 0 bytes after a block of size 40 alloc'd
```

In both cases, the key information is the **function name**, **source file**,
and **line number**. Fix the code at the indicated location.

## D. Self-Check Questions

1. What compiler flag enables AddressSanitizer?
2. Does Valgrind require recompilation of the program?
3. Why should you compile with `-g` when using ASan or Valgrind?
4. What is the typical runtime slowdown when running under Valgrind?
5. Which tool detects reads of uninitialised memory — ASan or Valgrind?
6. What ASan error message indicates a write past the end of a heap allocation?
7. In Valgrind output, what does "definitely lost" mean in the leak summary?
8. Why is `-fno-omit-frame-pointer` recommended when building with ASan?
9. Can ASan and Valgrind be used on the same binary at the same time? Why or why not?
10. If your tests pass but ASan reports a leak, is the program correct? Explain.
