#!/bin/bash

#PBS -l select=5:ncpus=2:mem=2gb

#PBS -l walltime=0:02:00

#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 10 /absolute/path/to/binary.out
