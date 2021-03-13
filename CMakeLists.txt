cmake_minimum_required(VERSION 3.17)
project(mpi_parallel_tasks)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/include/mpi/)
set(CMAKE_C_COMPILER "/usr/bin/mpicc")
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")

add_executable(matrix matrix.cpp)