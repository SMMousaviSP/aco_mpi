#include <iostream>
#include <vector>

#include "graph_gen/naivegen.cpp"
#include "ant.cpp"

using namespace std;

#define SIZE 5

const float alpha = 0.5;
const float beta = 0.5;

int main() {
    int graphSize = SIZE;
    T_GRAPH** graphData;
    T_PHER** pheromones;
    graphData = generateNaiveGraph(graphSize, 1.0, 6.0, 0);
    pheromones = generatePheromones(graphSize, 1.0);

    // run ants
    // @TODO
    vector<int> neighbors = getNeighbors(2, graphData, graphSize);
    vector<int> alreadyVisited;
    alreadyVisited.push_back(1);
    alreadyVisited.push_back(3);
    vector<int> neighborsCleaned = eliminateAlreadyVisitedNeighbors(neighbors, alreadyVisited);

    // print neighbors vector  
    cout << "neighbors: " << endl;
    for (int i = 0; i < neighbors.size(); i++) {
        cout << neighbors[i] << endl;
    }
    cout << endl;


    // print neighborsCleaned vector
    cout << "cleaned neighbors: " << endl;
    for (int i = 0; i < neighborsCleaned.size(); i++) {
        cout << neighborsCleaned[i] << endl;
    }
    cout << endl;


    // print graphData
    cout << "graph: " << endl;
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            cout << graphData[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    
    // print pheromones
    cout << "pheromones: " << endl;
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            cout << pheromones[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    

    return 0;
}