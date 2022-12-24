#include <vector>
#include <unordered_map>

#include "settings.h"

using namespace std;

struct antPath {
    vector<int> path;
    T_GRAPH pathLength;
};


antPath antRun(int, T_GRAPH**, int, T_PHER**, float, float);
double staticProb(T_GRAPH);
vector<int> getNeighbors(int, T_GRAPH**, int);
vector<int> eliminateAlreadyVisitedNeighbors(vector<int>, vector<int>);
unordered_map<int, double> calculateProbability(int, T_GRAPH**, int, T_PHER**, float, float);
int chooseNextNode(unordered_map<int, double>, int);


/**
 * @brief                Compute the path that a single ant will take
 *
 * pseudo-random generator is controlled by macro 'SEED'
 *
 * @param node           node of which you want to calculate the possibility of next neighbor
 * @param graph          graph of reference
 * @param graphSize      size of the graph
 * @param pheromones     pheromones of reference
 * @param exploitation   exploitation factor
 * @param exploration    exploration factor
 * @return antPath       path and path length that the ant traveled
 */
antPath antRun(int node, T_GRAPH** graph, int graphSize, T_PHER** pheromones, float exploitation, float exploration) {
    vector<int> alreadyVisited;
    alreadyVisited.push_back(node);
    antPath antPath;
    antPath.path.push_back(node);
    antPath.pathLength = 0;
    unordered_map<int, double> probabilities;
    int currentNode = node;
    int nextNode = -1;
    while (alreadyVisited.size() < graphSize) {
        probabilities = calculateProbability(currentNode, graph, graphSize, pheromones, exploitation, exploration);
        nextNode = chooseNextNode(probabilities, SEED);
        antPath.path.push_back(nextNode);
        alreadyVisited.push_back(nextNode);
        antPath.pathLength += graph[currentNode][nextNode];
        currentNode = nextNode;
    }
    // Check if the last edge to return back to the initial node exists (for not fully connected graphs)
    if (graph[currentNode][node] != NULL_TIE) {
        antPath.pathLength += graph[currentNode][node];
        antPath.path.push_back(node);
    }
    return antPath;
}


/**
 * @brief calculates static probability of choosing a tie
 * 
 * constant is controlled by macro 'K_CONST'
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
 * @param graphSize                     size of the graph
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
