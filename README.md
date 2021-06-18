# Description
Parallel programming tasks for FEFU lessons using C++ with MPI

# Installing
MPI compiler: `sudo apt-get install mpich`

# Usage
1. Compiling:  `mpic++ -o binary_file_name file_name.cpp`
2. Running: `mpiexec -n number_of_threads ./binary_file_name`