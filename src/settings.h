#define NULL_TIE -1
#define T_PHER float
#define T_GRAPH float

// parameters for ant colony tuning
// default values if others are not specified
// @TODO check if these value are right
#define EVAP_RATE 0.1
#define K_CONST 0.4
#define PHER_BUDGET 0.4
#define SEED 0

// Structs
struct antPath {
    vector<int> path;
    T_GRAPH pathLength;
};
