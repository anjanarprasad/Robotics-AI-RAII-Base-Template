#pragma once

#include <utility>

template <typename F>
class ScopeExit {
public:
    explicit ScopeExit(F f) : func_(std::move(f)), active_(true) {}

    ~ScopeExit() noexcept {
        if (active_) {
            func_();
        }
    }

    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;

    ScopeExit(ScopeExit&& other) noexcept
        : func_(std::move(other.func_)), active_(other.active_) {
        other.active_ = false;
    }

    void release() noexcept { active_ = false; }

private:
    F func_;
    bool active_;
};

template <typename F>
ScopeExit<F> scope_exit(F f) {
    return ScopeExit<F>(std::move(f));
}
