#pragma once

#include <utility> // std::move, std::exchange

// UniqueResource is a generic RAII wrapper for "handles".
// A handle can be: FILE*, int fd, socket handle, camera SDK pointer, etc.
//
// You provide a Deleter type that defines:
// - invalid(): what "no resource" looks like
// - is_valid(h): how to check if handle is valid
// - operator()(h): how to release it
template <typename Handle, typename Deleter>
class UniqueResource {
public:
    // Default: owns nothing (invalid handle).
    UniqueResource() noexcept
        : handle_(Deleter::invalid()), deleter_() {}

    // Take ownership of an existing handle.
    explicit UniqueResource(Handle h, Deleter d = Deleter{}) noexcept
        : handle_(h), deleter_(std::move(d)) {}

    // Destructor releases the handle if it is valid.
    ~UniqueResource() noexcept {
        reset(); // reset() handles safe release.
    }

    // No copy: prevents double-release.
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;

    // Move: transfer ownership.
    UniqueResource(UniqueResource&& other) noexcept
        : handle_(std::exchange(other.handle_, Deleter::invalid())),
          deleter_(std::move(other.deleter_)) {}

    // Move assignment: release current, then take ownership.
    UniqueResource& operator=(UniqueResource&& other) noexcept {
        if (this != &other) {
            reset(); // release current resource first
            deleter_ = std::move(other.deleter_);
            handle_ = std::exchange(other.handle_, Deleter::invalid());
        }
        return *this;
    }

    // Access the raw handle (ownership stays here).
    Handle get() const noexcept { return handle_; }

    // True if we currently own a valid handle.
    bool valid() const noexcept { return deleter_.is_valid(handle_); }

    // Enables: if (resource) { ... }
    explicit operator bool() const noexcept { return valid(); }

    // Release ownership to the caller (no cleanup after this).
    Handle release() noexcept {
        return std::exchange(handle_, Deleter::invalid());
    }

    // Replace the current handle:
    // - releases old handle if valid
    // - stores new handle
    void reset(Handle new_handle = Deleter::invalid()) noexcept {
        if (valid()) {
            deleter_(handle_); // release old handle
        }
        handle_ = new_handle;  // store new handle
    }

private:
    Handle handle_;   // the resource handle we own
    Deleter deleter_; // how to check/release it
};
