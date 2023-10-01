#!/bin/bash
#SBATCH -ptest
#SBATCH -N3
#SBATCH -n4
mpicc -O3 hello.c -o hello
export MPIP=“-y -l”
export LD_PRELOAD=/opt/mpiP/lib/libmpiP.so
mpirun ./hello