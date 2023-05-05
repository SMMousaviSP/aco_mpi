# Preprocessing the Result on the HPC Cluster
The result of the parallel code is saved in different `csv` files for each process. In order to preprocess the result and have the best found path of each iteration among all processes in one file you can use this code.

## Requirements
- Python 3.x
- pandas library

## Usage
Modify the `path_pbs.sh` file and set the path to the directory containing the `csv` files. Then run the following command.
```bash
qsub path_pbs.sh
```
The result will be saved in the `result.csv` file in the same directory as the `csv` files.

It is also possible to give a list of directories containing the `csv` files. You can pass more than one address to the `smallest_path.py` in `path_pbs.sh`.
