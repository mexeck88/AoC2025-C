# Advent of Code 2025 — Solutions in Barr C

This repository contains solutions for **Advent of Code 2025**, implemented entirely in Barr C.

## Overview

- Each challenge is organized under a separate `ChalX` directory.  
- All solutions are written in standard C (C99) using only the standard library.  
- The focus is on correctness, simplicity, and performance.

## Directory Structure

```
advent-of-code-2025/
├── Chal01/
│ ├── Makefile
│ ├── bin/
│ ├── include/
│ └── src/
│ └── chal01.c
├── Chal02/
│ ├── Makefile
│ ├── bin/
│ ├── include/
│ └── src/
│ └── chal02.c
├── Chal03/
│ ├── ...
└── README.md
```

Each `ChalX` directory contains:
- `Makefile`: Build configuration for that day’s challenge  
- `bin/`: Compiled binaries  
- `include/`: Header files  
- `src/`: Source code (`chalX.c`)

## Building and Running

To build and run a specific challenge:

```bash
cd Chal01
make
./bin/chal01

To clean build artifacts:
make clean

All code is verified to compile with:
gcc -std=c99 -Wall -Wextra -O2
```

## Implementation Notes

- Each challenge reads from standard input or a local input.txt file.
- No external libraries are used.
- Shared utility headers and source files, if any, are located in each challenge’s include/ and src/ directories.

## Requirements

- GCC or Clang
- Make
- Standard C library
