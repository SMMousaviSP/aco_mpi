#include <iostream>
#include <vector>

#include "graph_gen/naivegen.cpp"
#include "ant.cpp"

#define SIZE 5

const float alpha = 0.5;
const float beta = 0.5;

int main() {
    int graphSize = SIZE;
    float** graphData;
    graphData = generateNaiveGraph(graphSize, 1.0, 6.0, 0);

    // run ants
    // @TODO
    vector<int> neighbors = getNeighbors(2, graphData, graphSize);
    vector<int> alreadyVisited;
    alreadyVisited.push_back(1);
    alreadyVisited.push_back(3);
    vector<int> neighborsCleaned = eliminateAlreadyVisitedNeighbors(neighbors, alreadyVisited);

    // print neighbors vector  
    std::cout << "neighbors: " << std::endl;
    for (int i = 0; i < neighbors.size(); i++) {
        std::cout << neighbors[i] << std::endl;
    }

    // print neighborsCleaned vector
    std::cout << "cleaned neighbors: " << std::endl;
    for (int i = 0; i < neighborsCleaned.size(); i++) {
        std::cout << neighborsCleaned[i] << std::endl;
    }


    // print graphData
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            std::cout << graphData[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}