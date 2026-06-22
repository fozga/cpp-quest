#pragma once

#include <string>

/*
Day 01 Task: Pure RAII + Rule of 5

Goal:
Implement a small RAII class named FileHandle that owns a pseudo file
descriptor (int). The class should model unique ownership and release
the resource automatically.

What your implementation must achieve:
1. Use -1 as the invalid sentinel value.
2. Destructor must release the descriptor if valid (and must not throw).
3. Copy constructor and copy assignment must stay deleted.
4. Move constructor and move assignment must transfer ownership and leave
    the moved-from object invalid.
5. is_valid() should report whether the object currently owns a descriptor.
6. get() should return the raw descriptor or throw std::runtime_error when
    the handle is invalid.
7. reset(new_handle) should close the old descriptor (if any) and store
    new_handle.
8. open_pseudo_file() / close_pseudo_file() should simulate open/close
    behavior for testing.

Tip:
Follow exception-safety rules: no leaks, valid object state after errors,
and noexcept move operations.
*/

// Simulates OS-level open/close for a file descriptor.
int  open_pseudo_file(const std::string& name);
void close_pseudo_file(int fd);

class FileHandle {
    int fd_ {-1};

public:
    FileHandle() = default;

    explicit FileHandle(int handle);

    ~FileHandle();

    // Unique ownership - copying is not allowed.
    FileHandle(const FileHandle&)            = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& other) noexcept;
    FileHandle& operator=(FileHandle&& other) noexcept;

    bool is_valid() const noexcept;

    int get() const;

    void reset(const int new_handle = -1);
};
