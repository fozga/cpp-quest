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

FileHandle::FileHandle(int handle) {
    // TODO: Store handle as owned descriptor.
    (void)handle;
}

FileHandle::~FileHandle() {
    // TODO: Release descriptor if valid. Do not throw.
}

FileHandle::FileHandle(FileHandle&& other) noexcept {
    // TODO: Transfer ownership from other and invalidate other.
}

FileHandle& FileHandle::operator=(FileHandle&& other) noexcept {
    // TODO: Handle self-assignment, release current resource,
    // then transfer ownership from other and invalidate other.
    return *this;
}

// ── FileHandle public interface ─────────────────────────────────────────

bool FileHandle::is_valid() const noexcept {
    // TODO: Return true when fd_ is a valid descriptor.
    return false;
}

int FileHandle::get() const {
    // TODO: Return fd_ or throw std::runtime_error when invalid.
    throw std::runtime_error("TODO: implement FileHandle::get()");
}

void FileHandle::reset(int new_handle) {
    // TODO: Close current descriptor (if valid), then store new_handle.
}
