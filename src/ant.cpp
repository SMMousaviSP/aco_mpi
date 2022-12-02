#include <vector>

#include "settings.h"

using namespace std;


/**
 * @brief Get the neighbors of a node
 *
 * assume square matrix for "graphData"
 * 
 * @param node          Node of which you want to obtain neighbors
 * @param graphData     graph of refence
 * @param graphSize    size of the graph
 * @return vector<int>  the neighbors of "node"
 */
vector<int> getNeighbors(int node, float** graphData, int graphSize) {
    vector<int> neighbors;
    for (int i = 0; i < graphSize; i++) {
        if (graphData[node][i] != NULL_TIE) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}


/**
 * @brief removes nodes that have already been visited from a list
 * 
 * @param neighbors         collection of neighbors
 * @param alreadyVisited    already visited neighbors 
 * @return vector<int>      neightbors that have not been visited
 */
vector<int> eliminateAlreadyVisitedNeighbors(vector<int> neighbors, vector<int> alreadyVisited) {
    vector<int> result;
    for (int i = 0; i < neighbors.size(); i++) {
        if (find(alreadyVisited.begin(), alreadyVisited.end(), neighbors[i]) == alreadyVisited.end()) {
            result.push_back(neighbors[i]);
        }
    }
    return result;
}
