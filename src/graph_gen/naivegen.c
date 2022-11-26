#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Generate weigthed graph where each tie is randomly generated.
 * 
 * Graph is generated with random weigthed ties but no self-ties to nodes.
 * The weigth is within an interval
 *
 * @param size      size of the adjacency matrix
 * @param min       minimum generated number
 * @param max       maximum generated number
 * @param seed      random generation
 * @return float**  adjacency matrix
 */
float ** generate_naive_graph(int size, float min, float max, int seed) {

    if(max < min) {
        perror("Variable min is greater than max!");
        exit(1);
    }

    float ** graph = (float **) malloc(size * sizeof(float *) * size);
    srand(seed);
    for (int i = 0; i < size; i++) {
        graph[i] = (float *) malloc(size * sizeof(float) * size);
        for (int j = 0; j < size; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = (((float)rand() / (float)(RAND_MAX)) * (max - min)) + min;
            }
        }
    }
    
    return graph;
}
