#pragma once

#include <string>
#include <utility>
#include <stdexcept>

// Simulates OS-level open/close for a file descriptor.
int  open_pseudo_file(const std::string& name);
void close_pseudo_file(int fd);

// RAII wrapper for a POSIX-like file descriptor (int handle).
// Models unique ownership: copying is deleted, moving transfers the resource.
class FileHandle {
    int fd_ = -1;

public:
    // Constructs an invalid handle.
    FileHandle() = default;

    // Takes ownership of an existing descriptor.
    explicit FileHandle(int handle);

    // Releases the descriptor if valid. Never throws.
    ~FileHandle();

    // Unique ownership — copying is not allowed.
    FileHandle(const FileHandle&)            = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Transfers ownership from `other`; leaves `other` invalid.
    FileHandle(FileHandle&& other) noexcept;
    FileHandle& operator=(FileHandle&& other) noexcept;

    // Returns true when the handle refers to an open descriptor.
    bool is_valid() const noexcept;

    // Returns the raw descriptor. Throws std::runtime_error if invalid.
    int get() const;

    // Closes the current descriptor (if valid) and stores `new_handle`.
    void reset(int new_handle = -1);
};
