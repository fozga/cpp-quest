#include "exercise.hpp"

#include <stdexcept>

static int s_next_fd    = 0;
static int s_close_count = 0;

int open_pseudo_file(const std::string& /*name*/) {
    return s_next_fd++;
}

void close_pseudo_file(int fd) {
    if (fd < 0)
        return;
    ++s_close_count;
}

int get_close_count() {
    return s_close_count;
}

void FileDescriptorDeleter::operator()(int* fd) const noexcept {
    if (fd) {
        if (*fd >= 0)
            close_pseudo_file(*fd);
        delete fd;
    }
}

FileHandlePtr make_file_handle(const std::string& name) {
    int fd = open_pseudo_file(name);
    return FileHandlePtr(new int(fd));
}

bool is_valid_handle(const FileHandlePtr& h) noexcept {
    return h && *h >= 0;
}

int get_handle(const FileHandlePtr& h) {
    if (!is_valid_handle(h))
        throw std::runtime_error("invalid file handle");
    return *h;
}
