#include "exercise.hpp"

// ── Pseudo-OS helpers ───────────────────────────────────────────────────

static int next_fd = 0;

int open_pseudo_file(const std::string& /*name*/) {
    return next_fd++;
}

void close_pseudo_file(int fd) {
    if (fd < 0) return;
    // In a real OS this would release kernel resources.
    (void)fd;
}

// ── FileHandle special members ──────────────────────────────────────────

FileHandle::FileHandle(int handle)
    : fd_{handle} {}

FileHandle::~FileHandle() {
    if (fd_ != -1) {
        close_pseudo_file(fd_);
    }
}

FileHandle::FileHandle(FileHandle&& other) noexcept
    : fd_{other.fd_} {
    other.fd_ = -1;
}

FileHandle& FileHandle::operator=(FileHandle&& other) noexcept {
    if (this != &other) {
        reset();
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

// ── FileHandle public interface ─────────────────────────────────────────

bool FileHandle::is_valid() const noexcept {
    return fd_ != -1;
}

int FileHandle::get() const {
    if (fd_ == -1) {
        throw std::runtime_error("FileHandle::get() called on invalid handle");
    }
    return fd_;
}

void FileHandle::reset(int new_handle) {
    if (fd_ != -1) {
        close_pseudo_file(fd_);
    }
    fd_ = new_handle;
}
