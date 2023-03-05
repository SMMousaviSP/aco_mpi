#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "settings.h"
#include "structs.h"

using namespace std;


void printGraph(T_GRAPH* graphData, int graphSize) {
    cout << "graph: " << endl;
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            cout << graphData[INDEX(i, j)] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// save the graph in a file
void saveGraph(T_GRAPH* graphData, int graphSize, string filename) {
    ofstream csvFile;
    csvFile.open(filename);
    // Printing the header
    csvFile << "i,j,value" << endl;
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            // Printing the number of the iteration
            csvFile << i << ",";
            // Printing the number of the ant
            csvFile << j << ",";
            // printing the path length
            csvFile << graphData[INDEX(i, j)] << endl;
        }
    }
    csvFile.close();
}


void printPheromone(T_PHER* pheromones, int graphSize) {
    cout << "pheromones: " << endl;
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            cout << pheromones[INDEX(i, j)] << " ";
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


void saveMetadata(string filename, int graphSize, float exploitation, float exploration, double elapsedTime) {
    ofstream csvFile;
    csvFile.open(filename);
    // Printing the header
    csvFile << "graphSize,antsNumber,antsIterations,evaporationRate,kConstant,pheromoneConstant,exploitation,exploration,seed,elapsedTime" << endl;
    // Printing the metadata
    csvFile << graphSize << ",";
    csvFile << ANTS_N << ",";
    csvFile << ANTS_ITER << ",";
    csvFile << EVAP_RATE << ",";
    csvFile << K_CONST << ",";
    csvFile << PHER_CONST << ",";
    csvFile << exploitation << ",";
    csvFile << exploration << ",";
    csvFile << SEED << ",";
    csvFile << elapsedTime << endl;
    csvFile.close();
}


AntPath getBestAntPath(AntPath* antPathArray, int arraySize) {
    AntPath bestPath;
    bestPath.pathLength = numeric_limits<T_GRAPH>::max();
    for (int i = 0; i < arraySize; i++) {
        if (antPathArray[i].pathLength < bestPath.pathLength) {
            bestPath = antPathArray[i];
        }
    }
    return bestPath;
}


void createDirectoryInHome(string folderName) {
    string homeDir = getenv("HOME");
    string folderPath = homeDir + "/" + folderName;

    struct stat st;
    if (stat(folderPath.c_str(), &st) == -1) {
        mkdir(folderPath.c_str(), 0700);
        cout << "Folder created successfully!" << endl;
    } else {
        cout << "Folder already exists." << endl;
    }
}
