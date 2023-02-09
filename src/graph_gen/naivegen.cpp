#include <cstdlib>
#include <stdexcept>

#include "../settings.h"


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
 * @return T_GRAPH**  adjacency matrix
 */
T_GRAPH * generateNaiveGraph(int size, T_GRAPH min, T_GRAPH max) {

    // Throw an exception if min is greater than max
    if (min > max) {
        throw std::invalid_argument("min is greater than max");
    }

    T_GRAPH * graph = new T_GRAPH[size * size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                graph[INDEX(i, j)] = NULL_TIE;
            } else {
                graph[INDEX(i, j)] = (((T_GRAPH)rand() / (T_GRAPH)(RAND_MAX)) * (max - min)) + min;
            }
        }
    }

    return graph;
}


T_GRAPH * generateEmptyGraph(int size) {
    T_GRAPH * graph = new T_GRAPH[size * size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            graph[INDEX(i, j)] = 0.0;
        }
    }
    return graph;
}


/**
 * @brief Generate pheromones matrix
 * 
 * @param size       size of the adjacency matrix
 * @param initValue  initial value of the pheromones
 * @return T_PHER**  adjacency matrix
 */
T_PHER * generatePheromones(int size, T_PHER initValue) {
    T_PHER * pheromones = new T_PHER[size * size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            pheromones[INDEX(i, j)] = initValue;
        }
    }
    return pheromones;
}
