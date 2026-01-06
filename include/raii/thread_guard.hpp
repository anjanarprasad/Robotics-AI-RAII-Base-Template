#pragma once

#include <thread>
#include <utility>

class ThreadGuard {
public:
    explicit ThreadGuard(std::thread t) noexcept : t_(std::move(t)) {}

    ~ThreadGuard() noexcept {
        if (t_.joinable()) {
            t_.join();
        }
    }

    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;

    ThreadGuard(ThreadGuard&&) noexcept = default;
    ThreadGuard& operator=(ThreadGuard&&) noexcept = default;

private:
    std::thread t_;
};
