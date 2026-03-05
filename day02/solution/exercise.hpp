#pragma once

#include <memory>
#include <string>
#include <utility>

int  open_pseudo_file(const std::string& name);
void close_pseudo_file(int fd);
int  get_close_count();

struct FileDescriptorDeleter {
    void operator()(int* fd) const noexcept;
};

using FileHandlePtr = std::unique_ptr<int, FileDescriptorDeleter>;

FileHandlePtr make_file_handle(const std::string& name);
bool          is_valid_handle(const FileHandlePtr& h) noexcept;
int           get_handle(const FileHandlePtr& h);
