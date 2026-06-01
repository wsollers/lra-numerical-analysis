# LRA Numerical Analysis

C++23 numerical software workbench for the Learning Real Analysis curriculum.

This repo hosts numerical methods, approximation experiments, floating-point and
interval arithmetic studies, algorithms, benchmarks, fuzzing, generated plots,
and software artifacts that support Volume VII and related coursework.

## Workspace Shape

```text
include/lra/numeric/        Shared numerical library headers
cmake/                      Shared CMake helpers
projects/<name>/            Self-contained app/test/benchmark/fuzz labs
analysis/                   Python analysis notes and scripts
docker/                     Portable Clang and analysis images
tools/                      Local build helpers
artifacts/                  Generated data, plots, and reports
```

Each `projects/<name>/` owns its local tests, benchmarks, and fuzz targets.
Shared reusable numerical code belongs under `include/lra/numeric/` and, later,
`src/numeric/`.

## Initial Projects

- `projects/hello-one`: minimal executable. It prints `hello-one`.
- `projects/hello-unit`: minimal GoogleTest project. It asserts `true`.

## MSVC Build

From PowerShell:

```powershell
.\tools\build-msvc.ps1
```

The script finds Visual Studio, enters the x64 developer environment, configures
`msvc-debug`, builds, and runs CTest.

## Docker Clang Build

```powershell
.\tools\build-docker-clang.ps1
```

The Docker path builds the portable Clang image, configures with Ninja, builds,
runs `hello-one`, and runs CTest.

## Analysis Image

```powershell
.\tools\run-analysis-docker.ps1
```

The analysis image includes Python with NumPy, SciPy, pandas, matplotlib, and
seaborn. C++ executables should write structured JSON/CSV artifacts that Python
analysis scripts can turn into plots and reports.
