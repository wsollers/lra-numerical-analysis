cmake_minimum_required(VERSION 3.27)

project(lra_numerical_analysis
    VERSION 0.1.0
    LANGUAGES CXX
)

enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(LRA_NUM_ENABLE_SANITIZERS "Enable ASan/UBSan for supported Debug builds" ON)
option(LRA_NUM_WARNINGS_AS_ERRORS "Treat warnings as errors for project targets" OFF)
option(LRA_NUM_ENABLE_BENCHMARKS "Enable project benchmark targets" OFF)
option(LRA_NUM_ENABLE_FUZZING "Enable project fuzz targets" OFF)

include(CompilerOptions)
include(Sanitizers)
include(Dependencies)

add_library(lra_numeric INTERFACE)
add_library(lra::numeric ALIAS lra_numeric)
target_include_directories(lra_numeric INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_subdirectory(projects/hello-one)
add_subdirectory(projects/hello-unit)
add_subdirectory(projects/real-intervals)
add_subdirectory(projects/convex-hull)
