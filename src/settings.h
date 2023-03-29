#ifndef SETTINGS_H
#define SETTINGS_H

#define SIZE 200
#define INDEX(i, j) (i * SIZE + j)
#define NULL_TIE -1
#define T_PHER float
#define T_GRAPH float
#define GRAPH_MIN 1.0
#define GRAPH_MAX 20.0

// parameters for ant colony tuning
// default values if others are not specified
// @TODO check if these value are right
#define EVAP_RATE 0.1
#define K_CONST 0.4
#define PHER_CONST 0.4
#define ALPHA 0.9
#define BETA 0.1
#define SEED 0

#define ANTS_N 8
#define ANTS_ITER 100

// Only used in the parallel version
#define MPI_T_PHER MPI_FLOAT
#define MPI_T_GRAPH MPI_FLOAT
#define COMM_NUM 10
// 'W' for updating the worst colony only and 'A' for updating all.
#define UPDATE_ALL 'A'
#define UPDATE_WORST 'W'
#define UPDATE_STRATEGY UPDATE_ALL
// mode, 0: continue, 1: send pheromones, 2: receive and update pheromones
#define M_CONTINUE 0
#define M_SEND_PHER 1
#define M_RECV_PHER 2

#define OUT_DIR string("out")

#endif
