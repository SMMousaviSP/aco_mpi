# Ant Colony Optimization for TSP
This readme serves no purpose for now, but it will be completed later. In the
meanwhile if you want to run the code, compile the `acompi.cpp` in the `src`
directory with `g++` and then run it.
```bash
g++ aco.cpp
./a.out
```

# Running on the HPC Cluster
Create the `env` directory in `src`. Move pbs script from `pbs` directory to
the `env` directory. On the last line of the pbs script give the absolute path
of your compiled code.
Be careful as the `env` directory will be ignored by git.
Run the following command to compile the code.
```bash
mpic++ -std=c++0x -g -Wall -o env/acompi.out acompi.cpp
```
Then go to the `env` directory and run the following command to submit the job
to the cluster.
```bash
qsub acompi_pbs.sh
```

# Todo
- [ ] get time for each iteration, save it in a new CSV
- [ ] make parallel for main.cpp and ant.cpp
- [ ] save data in UNIX timestamp folder so to compare later
- [ ] Julia
-- iterate set of folder, check if plots are missing and if so make them
- [ ] Benchmark
-- 5 iterations for 5 different graphs

