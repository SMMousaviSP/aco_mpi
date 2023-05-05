#!/bin/bash

#PBS -l select=1:ncpus=1:mem=8gb

#PBS -l walltime=0:08:00

#PBS -q short_cpuQ

module load python-3.7.2
python3 /path/to/smallest_path.py /path/to/results/out*

