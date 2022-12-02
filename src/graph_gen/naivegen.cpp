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
T_GRAPH ** generateNaiveGraph(int size, T_GRAPH min, T_GRAPH max, int seed) {

    // Throw an exception if min is greater than max
    if (min > max) {
        throw std::invalid_argument("min is greater than max");
    }

    T_GRAPH ** graph = new T_GRAPH*[size];
    srand(seed);
    for (int i = 0; i < size; i++) {
        graph[i] = new T_GRAPH[size];
        for (int j = 0; j < size; j++) {
            if (i == j) {
                graph[i][j] = NULL_TIE;
            } else {
                graph[i][j] = (((T_GRAPH)rand() / (T_GRAPH)(RAND_MAX)) * (max - min)) + min;
            }
        }
    }

    return graph;
}
