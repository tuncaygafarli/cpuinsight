# CPUINSIGHT - CPU Branch Prediction Visualizer

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-2.6.1-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-yellow.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20|%20Linux%20|%20Mac-lightgrey.svg)
[![CI Build](https://github.com/tuncaygafarli/cpuinsight/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/tuncaygafarli/cpuinsight/actions/workflows/cmake-multi-platform.yml)

**CPUINSIGHT** is a graphical user interface built on top of the **CPUINSIGHT CPU** simulation backend, providing real-time visualization of CPU instruction execution and branch prediction behavior.

This repository focuses on the **GUI frontend**, enabling interactive inspection of pipeline flow, register states, and branch predictor performance while leveraging the original CPU simulation core.

## Features

- **Interactive GUI** - Real-time visualization of CPU pipeline
- **Multiple Branch Predictors** - Support for various prediction algorithms
- **Instruction Pipeline Display** - Visualize instruction execution flow
- **Register File Visualization** - Watch register values update in real-time
- **Performance Metrics** - Track prediction accuracy and performance

## Screenshots

<img width="1920" height="1080" alt="Base Profile Screenshot 2025 12 30 - 02 05 30 37" src="https://github.com/user-attachments/assets/287480d2-d916-4438-9d7c-96f0e713c3bd" />

## 🚀 Quick Start

## Prerequisites

- **CMake 3.16+** - [Download](https://cmake.org/download/)
- **Git** - [Download](https://git-scm.com/download/)
- **C++17 Compiler**
  - Windows: Visual Studio 2019+ or MinGW
  - Linux: GCC 8+ or Clang 7+
  - Mac: Xcode Command Line Tools

### Installation & Build

#### Windows (Command Prompt/PowerShell)

```bash
# Clone the repository
git clone https://github.com/tuncaygafarli/cpuinsight.git
cd cpuinsight

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release

# Run the application
cd bin/Release
CPUINSIGHT.exe
```

#### Linux/Mac

```bash
# Clone the repository
git clone https://github.com/tuncaygafarli/cpuinsight.git
cd cpuinsight

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release -j4  # Use 4 cores for faster build

# Run the application
cd bin/Release
./CPUINSIGHT
```

### One-Click Build Scripts

#### Windows (build.bat):

```bat
@echo off
echo Building CPUINSIGHT...
mkdir build 2>nul
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
echo.
echo Running CPUINSIGHT...
cd bin/Release
CPUINSIGHT.exe
pause
```

#### Linux/Mac (build.sh):

```bash
#!/bin/bash
echo "Building CPUINSIGHT..."
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -j$(nproc)
echo -e "\nRunning CPUINSIGHT..."
cd bin
./CPUINSIGHT
```

## Project Structure

```
cpuinsight/
├── build/                                # Build directory (generated)
├── vendor/                               # Dependencies (SFML)
├── src/
│   ├── gui/                              # GUI rendering and interface
│   │   ├── gui_render.cpp                # Main GUI rendering logic
│   │   ├── gui_render.h
│   │   └── gui_command_parser.cpp
│   ├── cpu/                              # CPU simulation core
│   │   ├── cpu.cpp                       # CPU emulation
│   │   ├── instruction.h                 # Instruction definitions
│   │   └── branchpredictor.cpp           # Branch prediction algorithms
│   ├── parser/                           # Assembly parser
│   │   ├── parser.cpp                    # Assembly parsing
│   │   └── lookup.cpp                    # Instruction lookup tables
│   └── main.cpp                          # Application entry point
├── fonts/                                # Font assets
├── asm_tests/                            # Assembly test files
├── CMakeLists.txt                        # Build configuration
└── README.md                             # This file
```

## Usage

### Basic controls

- ArrowUp : Scrolls up in INSTRUCTION section
- ArrowDown : Scrolls down in INSTRUCTION section
- SpaceBar : Executes only one instruction
- V : Enables / disables automatic execution
- R : Resets the process
- LShift : Increases auto CPU execution delay
- LControl : Decreases auto CPU execution delay

## Build options

### Configure different build types:

```bash
# Debug build (with symbols)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build (optimized)
cmake .. -DCMAKE_BUILD_TYPE=Release

# RelWithDebInfo (optimized with debug info)
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

# MinSizeRel (minimum size)
cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel
```

### Build specific configurations:

```bash
# Build Debug version
cmake --build . --config Debug

# Build Release version
cmake --build . --config Release

# Clean and rebuild
cmake --build . --config Release --clean-first
```

## Supported Branch Predictors

### 1. Simple Predictor

Basic implementation using simple heuristics for branch prediction.

### 2. GAg (Global Adaptive)

Uses a global history register to track branch patterns and adapts prediction based on recent history.

### 3. PAg (Per-address Adaptive)

Maintains separate prediction history for each branch address, allowing per-branch optimization.

### 4. GShare

Hybrid approach that combines global branch history with branch address for improved accuracy.

## 🛠️ Development

### Adding New Features

1. **Add source files** to the appropriate directory (`src/gui/`, `src/cpu/`, or `src/parser/`)
2. **Update `CMakeLists.txt`** if adding new source files:
   ```cmake
   # Add to the existing file glob pattern
   file(GLOB_RECURSE SRC_FILES
       "src/gui/*.cpp"
       "src/cpu/*.cpp"
       "src/parser/*.cpp"
       # Add new patterns here if needed
   )
   ```
3. **Rebuild** the project:
   ```bash
   cd build
   cmake --build . --config Debug
   ```

## 🧾 Credits

- **CPU Simulation Backend**
  Developed by **f3rhd**
  Licensed under the MIT License

- **GUI Frontend**
  Developed by **Tuncay Gafarli**

This repository is a fork of the original [**CPUINSIGHT**](https://github.com/f3rhd/cpuinsight) project and focuses on GUI-level visualization.

---

## 📜 License

- The **CPU simulation backend** is licensed under the **MIT License**
  © 2025 **f3rhd**

- The **GUI frontend additions** in this repository are
  © 2025 **Tuncay Gafarli**

See the LICENSE files for full details.
