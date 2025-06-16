# Exploration of algorithms for extracting wireframe models from man-made urban linear object point clouds

This repository contains the implementation of various algorithms for wireframe model extraction from point clouds, developed as part of an MSc thesis in Geomatics at TU Delft.

## Project Structure

```
.
├── 3rd_party/         # Third-party dependencies
├── data/              # Data files
├── hough_3d_line/     # 3D Hough transform implementation
├── output/            # Output files
└── src/               # Source code
    ├── analysis/      # Analysis modules
    ├── energyMinimization/  # Energy minimization algorithms
    ├── graph/         # Graph-based processing
    ├── ransac/        # RANSAC implementation
    └── regionGrowing/ # Region growing algorithms
```

## Prerequisites

- CMake (version 3.15 or higher)
- C++17 compatible compiler
- Required system libraries:
  - GMP (GNU Multiple Precision Arithmetic Library)
  - MPFR (Multiple Precision Floating-point Reliable Library)
  - Eigen (C++ template library for linear algebra)
  - Boost (C++ libraries)
- Additional third-party libraries (specified in 3rd_party directory)

### Installing Dependencies

#### Windows
Using vcpkg (recommended):
```bash
vcpkg install gmp
vcpkg install mpfr
vcpkg install eigen
vcpkg install boost
```

Alternatively, you can download and install manually:
- GMP: Download from [GMP website](https://gmplib.org/)
- MPFR: Download from [MPFR website](http://www.mpfr.org/)
- Eigen: Download from [Eigen website](https://eigen.tuxfamily.org/)
- Boost: Download from [Boost website](https://www.boost.org/)

#### macOS
For mac users, please switch the branch `mac`. Mac users need to install cgal package, instead of using the one provided in the `3rd_party` directory.
```bash
brew install gmp mpfr eigen boost cgal
```

## Building the Project

1. Clone the repository:
```bash
git clone [repository-url]
cd final_thesis
```

2. Create and enter the build directory:
```bash
mkdir build
cd build
```

3. Configure and build the project:

- Windows
    
    if use `Visual Studio 17 2022` as generator (use `cmake --help` to check available generators on the platform.)

    ```bash
    cmake -G "Visual Studio 17 2022" ..
    cmake --build .
    ```

    The compiled binaries will be placed in the `bin/` directory.

- macOS
    ```bash
    cmake ..
    make
    ```
    The compiled binaries will be placed in the `bin/` directory.

## Usage

The project provides several executables:

- `main_gui`: GUI-based tool for running algorithms.
- `main_analysis`: Command-line analysis tool for analyzing results.
- `main_paramSearch`: Command-line tool for parameter search.

### Running the GUI

```bash
./bin/Thesis_GUI
```

### Running Analysis

```bash
./bin/Thesis_Analysis [input_file] [options]
```

### Parameter Search

```bash
./bin/Thesis_ParamSearch [input_file] [options]
```

## Features

- Five algorithm implementataions for wireframe model extraction:
  - 3D RANSAC
  - 3D-2D RANSAC
  - Region Growing
  - Hough Transform
  - Energy minimization for Markov Random Field

- GUI tools for running algorithms
- Parameter optimization
- Result analysis