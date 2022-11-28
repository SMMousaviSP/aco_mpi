#include <cstdlib>
#include <stdexcept>


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

    // Throw an exception if min is greater than max
    if (min > max) {
        throw std::invalid_argument("min is greater than max");
    }

    float ** graph = new float*[size];
    srand(seed);
    for (int i = 0; i < size; i++) {
        graph[i] = new float[size];
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
