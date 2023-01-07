# Ant Colony Optimization for TSP
This readme serves no purpose for now, but it will be completed later. In the
meanwhile if you want to run the code, compile the `acompi.cpp` in the `src`
directory with `g++` and then run it.
```bash
g++ acompi.cpp
./a.out
```

# Running on the HPC Cluster
```bash
mpic++ -std=c++0x -g -Wall -o env/acompi.out acompi.cpp
```

# Todo
- [ ] get time for each iteration, save it in a new CSV
- [ ] make parallel for main.cpp and ant.cpp
- [ ] save data in UNIX timestamp folder so to compare later
- [ ] Julia
-- iterate set of folder, check if plots are missing and if so make them
- [ ] Benchmark
-- 5 iterations for 5 different graphs

