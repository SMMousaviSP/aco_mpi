#include "settings.h"

// formula: delta(dep, arr) = (1-EVAP_RATE) + sum(if_and_went_there, PHER_BUDGET/length)

// void sumPheromones(T_PHER** pheromones, int graphSize, vector<antPath> antsPaths) {
//     for (int i = 0; i < graphSize; i++) {
//         for (int j = 0; j < graphSize; j++) {
//             pheromones[i][j] = (1 - EVAP_RATE) * pheromones[i][j];
//         }
//     }
//     for (int i = 0; i < antsPaths.size(); i++) {
//         for (int j = 0; j < antsPaths[i].path.size() - 1; j++) {
//             pheromones[antsPaths[i].path[j]][antsPaths[i].path[j+1]] += PHER_BUDGET / antsPaths[i].pathLength;
//         }
//     }
// }

/**
 * @brief Update pheromones matrix
 * 
 * @param antPath       path covered by an ant
 * @param pheromones    pheromone metrices
 */
void depositAntPheromone(antPath antPath, T_PHER** pheromones) {
    // iterate over path in antpath
    for (int i = 0; i < antPath.path.size() - 1; i++) {
        // get current node and next node
        int currentNode = antPath.path[i];
        int nextNode = antPath.path[i + 1];
        // calculate delta
        T_PHER pheromoneTrace = PHER_CONST / antPath.pathLength;
        // update pheromones
        pheromones[currentNode][nextNode] += pheromoneTrace;
    }
}


/**
 * @brief evaporate the cells in the pheromone matrix
 *
 * evaporation rate is controlled by macro 'EVAP_RATE'
 * 
 * @param pheromones    matrix of pheromones
 * @param graphSize     size of 'pheromones' matrix
 */
void evaporatePheromones(T_PHER** pheromones, int graphSize) {
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            pheromones[i][j] = (1 - EVAP_RATE) * pheromones[i][j];
        }
    }
}
