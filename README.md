# Robotics-AI
Robotics AI starter repository with reusable templates and and buildable examples.
# C++ RAII Template (Basic)

This folder contains a small, beginner-friendly RAII base in C++.
The goal is to make cleanup automatic and safe using constructors/destructors.

## What is included
- `file_handle.hpp`  
  RAII wrapper for a file: opens in the constructor and closes in the destructor.

- `scope_exit.hpp`  
  Runs a cleanup action automatically when leaving a scope.

- `thread_guard.hpp`  
  Owns a `std::thread` and joins it automatically at scope exit.

## Folder structure
- `include/raii/` : header files (reusable utilities)
- `src/main.cpp`  : a small example program that uses the utilities

## Build and run (Mac / Linux)
From this folder:

```bash
g++ -Iinclude src/main.cpp -o demo
./demo
