#ifndef SETTINGS_H
#define SETTINGS_H

#define NULL_TIE -1
#define T_PHER float
#define T_GRAPH float

// parameters for ant colony tuning
// default values if others are not specified
// @TODO check if these value are right
#define EVAP_RATE 0.1
#define K_CONST 0.4
#define PHER_CONST 0.4
#define SEED 0

#define ANTS_N 5
#define ANTS_ITER 20 

// Only used in the parallel version
#define MPI_T_PHER MPI_FLOAT
#define COMM_NUM 4
// 'W' for updating the worst colony only and 'A' for updating all.
#define UPDATE_ALL 'A'
#define UPDATE_WORST 'W'
#define UPDATE_STRATEGY UPDATE_ALL
// mode, 0: continue, 1: send pheromones, 2: receive and update pheromones
#define M_CONTINUE 0
#define M_SEND_PHER 1
#define M_RECV_PHER 2

#endif
