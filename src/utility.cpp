#include <iostream>
#include <fstream>

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


void savePath(AntPath** antPathArrayIter, string filename) {
    ofstream csvFile;
    csvFile.open(filename);
    // Printing the header
    csvFile << "iteration,antNumber,pathLength,path" << endl;
    for (int iter = 0; iter < ANTS_ITER; iter++) {
        for (int i = 0; i < ANTS_N; i++) {
            // Printing the number of the iteration
            csvFile << iter << ",";
            // Printing the number of the ant
            csvFile << i << ",";
            // printing the path length
            csvFile << antPathArrayIter[iter][i].pathLength << ",";
            for (int j = 0; j < antPathArrayIter[iter][i].path.size(); j++) {
                // Printing the path
                // If it's the last element, don't print the dash
                if (j == antPathArrayIter[iter][i].path.size() - 1)
                    csvFile << antPathArrayIter[iter][i].path[j];
                else
                    csvFile << antPathArrayIter[iter][i].path[j] << "-";
            }
            csvFile << endl;
        }
    }
    csvFile.close();
}
