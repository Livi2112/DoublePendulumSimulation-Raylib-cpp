# Double Pendulum Simulation (C++ / Raylib)
A high-performance physical simulation of a double pendulum, developed in C++ using the Raylib library. This project demonstrates the phenomenon of deterministic chaos, where a simple set of initial conditions leads to a complex and unpredictable trajectory.
🚀 Features

    Real-time Physics: Solving Lagrangian equations of motion every frame.

    Dynamic Trace: A circular buffer-based trajectory visualizer that tracks the path of the second mass without performance degradation.

    Parameter Customization: Easily adjustable mass, rod length, and gravity constants.

    Stability Control: Optional energy loss (damping) to compensate for numerical integration drift.

🛠️ Technologies

    Language: C++11 or higher.

    Graphics Library: Raylib.

    Physics Engine: Lagrangian mechanics (Numerical integration via Euler method).

📸 Demonstration
    ![Example](example.png)

📋 How to Build and Run
Prerequisites

    A C++ compiler (GCC, Clang, or MSVC).

    Raylib installed on your system.

Compilation (Linux/MacOS example)
Bash

g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o double_pendulum
./double_pendulum

🧠 The Physics / Source

The formulas used in this project are based on the [MIT Double Pendulum Analysis](https://web.mit.edu/jorloff/www/chaosTalk/double-pendulum/double-pendulum-en.html).
    
