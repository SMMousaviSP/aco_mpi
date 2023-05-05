#include "settings.h"
#include "structs.h"


/**
 * @brief Update pheromones matrix
 * 
 * @param antPath       path covered by an ant
 * @param pheromones    pheromone matrix
 * @param graphSize     size of 'pheromones' matrix
 */
void depositAntPheromone(AntPath antPath, T_PHER* pheromones, int graphSize) {
    // if the ant didn't visit all the nodes, return (eliminating the ant)
    if (antPath.path.size() != graphSize + 1) {
        return;
    }

    // iterate over path in antpath
    for (int i = 0; i < antPath.path.size() - 1; i++) {
        // get current node and next node
        int currentNode = antPath.path[i];
        int nextNode = antPath.path[i + 1];
        // calculate delta
        T_PHER pheromoneTrace = PHER_CONST / antPath.pathLength;
        // update pheromones
        pheromones[INDEX(currentNode, nextNode)] += pheromoneTrace;
    }
}


/**
 * @brief evaporate the cells in the pheromone matrix
 *
 * evaporation rate is controlled by macro 'EVAP_RATE'
 * 
 * @param pheromones    pheromone matrix
 * @param graphSize     size of 'pheromones' matrix
 */
void evaporatePheromones(T_PHER* pheromones, int graphSize) {
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            pheromones[INDEX(i, j)] = (1 - EVAP_RATE) * pheromones[INDEX(i, j)];
        }
    }
}
