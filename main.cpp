#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

#include "raii/file_handle.hpp"
#include "raii/scope_exit.hpp"
#include "raii/thread_guard.hpp"

int main() {
    std::cout << "Start main\n";

    // This will always run at the end of main (even on early return).
    auto end_note = scope_exit([] {
        std::cout << "End main (scope_exit)\n";
    });

    // File RAII test: open log.txt and write a line.
    try {
        FileHandle f("log.txt", "w");
        std::fprintf(f.get(), "RAII: file closed automatically\n");
        std::cout << "Wrote to log.txt\n";
    } catch (const std::exception& e) {
        std::cout << "File error: " << e.what() << "\n";
        return 1;
    }

    // Thread RAII test: start a thread and auto-join it.
    ThreadGuard tg(std::thread([] {
        std::cout << "Thread started\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Thread finished\n";
    }));

    std::cout << "Main returning\n";
    return 0; // destructors run automatically here
}
