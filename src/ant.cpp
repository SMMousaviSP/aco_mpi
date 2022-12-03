#include <vector>
#include <unordered_map>

#include "settings.h"

using namespace std;


/**
 * @brief calculates static probability of choosing a tie
 * 
 * @param tie       tie of which calculate probability
 * @return double   probability of tie
 */
double staticProb(T_GRAPH tie) {
    return K_CONST / tie;
}


/**
 * @brief Get the neighbors of a node
 *
 * assume square matrix for "graphData"
 * 
 * @param node          node of which you want to obtain neighbors
 * @param graphData     graph of refence
 * @param graphSize     size of the graph
 * @return vector<int>  the neighbors of "node"
 */
vector<int> getNeighbors(int node, T_GRAPH** graphData, int graphSize) {
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


/**
 * @brief calculate the probability of choosing each neighbour of node
 * 
 * @param node                          node of which you want to calculate the possibility of next neighbor
 * @param graph                         graph of reference
 * @param pheromones                    pheromones of reference
 * @param exploitation                  exploitation factor
 * @param exploration                   exploration factor
 * @return unordered_map<int, double>   probabilities of choosing a neighbor node as next node
 */
unordered_map<int, double> calculateProbability(int node, T_GRAPH** graph, int graphSize, T_PHER** pheromones, float exploitation, float exploration) {
    vector<int> validNeighbors = getNeighbors(node, graph, graphSize);
    double denominator = 0;
    // for element in validNeighbors calculate probability and store in map
    unordered_map<int, double> probabilities;
    for (int i = 0; i < validNeighbors.size(); i++) {
        double probability = pow(pheromones[node][validNeighbors[i]], exploitation) * pow(staticProb(graph[node][validNeighbors[i]]), exploration);
        denominator += probability;
        probabilities[validNeighbors[i]] = probability;
    }

    // normalize probabilities
    for (int i = 0; i < validNeighbors.size(); i++) {
        probabilities[validNeighbors[i]] /= denominator;
    }
    return probabilities;
}


/**
 * @brief choose a neighbor based on probability
 * 
 * @param probabilities  probabilities of choosing a neighbor node as next node
 * @param seed           seed for random number generator
 * @return int           chosen neighbor
 */
int chooseNextNode(unordered_map<int, double> probabilities, int seed) {
    srand(seed);
    double randomDouble = (double) rand() / (double) RAND_MAX;
    for (auto const& x : probabilities) {
        if (randomDouble < x.second) {
            return x.first;
        }
        randomDouble -= x.second;
    }
    // throw exception that the probabilities do not sum up to 1
    throw std::invalid_argument("probabilities do not sum up to 1");
}
