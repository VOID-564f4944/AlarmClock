# Alarm Clock

A command-line alarm clock written in C++.

## Features
- Set an alarm in hh:mm format
- Logs remaining time every minute
- Rings when the alarm time is reached

## Requirements
- C++20 (for `std::chrono` timezone support)
- A compiler that supports `std::chrono::current_zone()` (MSVC, GCC 13+, Clang 16+)

## How to Run

### Visual Studio 2026
1. Open the `.sln` file in Visual Studio
2. Set build configuration to **Release** (top toolbar)
3. Press `Ctrl+F5` to build and run

### Command Line (g++)
1. Compile: `g++ -std=c++20 main.cpp -o alarm`
2. Run: `./alarm`
