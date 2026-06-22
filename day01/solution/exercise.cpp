#include "exercise.hpp"
#include <exception>
#include <iostream>

// ── Pseudo-OS helpers ───────────────────────────────────────────────────

static int next_fd = 0;

int open_pseudo_file(const std::string& /*name*/) {
    return next_fd++;
}

void close_pseudo_file(int fd) {
    if (fd < 0) return;
    if (fd >= next_fd){
        throw std::runtime_error("Trying to close file which was not opened.");
    }
    // In a real OS this would release kernel resources.
    (void)fd;
}

// ── FileHandle special members ──────────────────────────────────────────

FileHandle::FileHandle(int handle) : fd_{handle} {}

FileHandle::~FileHandle() {
    if (is_valid())
    {
        try
        {
            close_pseudo_file(fd_);
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] Destructor failed: " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "[ERROR] Destructor failed with an unknown exception." << std::endl;
        }
    }
}

FileHandle::FileHandle(FileHandle&& other) noexcept : fd_{other.fd_} {
        other.fd_ = -1;
}

FileHandle& FileHandle::operator=(FileHandle&& other) noexcept {
    if (this != &other){
        
        try
        {
            close_pseudo_file(fd_);
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] Move assignment failed: " << ex.what() << std::endl;
        }
        this->fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

// ── FileHandle public interface ─────────────────────────────────────────

bool FileHandle::is_valid() const noexcept {
    if (fd_ >= 0){
        return true;
    }
    return false;
}

int FileHandle::get() const {
    if (!is_valid()){
        throw std::runtime_error("Invalid file handle");
    }
    else
    {
        return fd_;
    }
}

void FileHandle::reset(const int new_handle) {
    if (new_handle == fd_)
    {
        return;
    }
    if (is_valid())
    {
        close_pseudo_file(fd_);
    }
    fd_ = new_handle;
}
