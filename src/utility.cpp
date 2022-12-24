#include <iostream>

#include "settings.h"
#include "structs.h"

using namespace std;


void printGraph(T_GRAPH** graphData, int graphSize) {
    cout << "graph: " << endl;
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            cout << graphData[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


void printPheromone(T_PHER** pheromones, int graphSize) {
    cout << "pheromones: " << endl;
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            cout << pheromones[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


void printPath(AntPath path) {
    cout << "path: " << endl;
    for (int i = 0; i < path.path.size(); i++) {
        cout << path.path[i] << " ";
    }
    cout << endl;
    cout << "path cost: " << path.pathLength << endl;
    cout << endl;
}
