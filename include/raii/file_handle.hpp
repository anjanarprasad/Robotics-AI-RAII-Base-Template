#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>

class FileHandle {
public:
    explicit FileHandle(const std::string& path, const char* mode) {
        fp_ = std::fopen(path.c_str(), mode);
        if (!fp_) {
            throw std::runtime_error("FileHandle: failed to open file: " + path);
        }
    }

    ~FileHandle() noexcept {
        if (fp_) {
            std::fclose(fp_);
            fp_ = nullptr;
        }
    }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& other) noexcept
        : fp_(std::exchange(other.fp_, nullptr)) {}

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (fp_) {
                std::fclose(fp_);
            }
            fp_ = std::exchange(other.fp_, nullptr);
        }
        return *this;
    }

    std::FILE* get() const noexcept { return fp_; }

private:
    std::FILE* fp_{nullptr};
};
