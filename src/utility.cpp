#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <filesystem>

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


void saveMetadata(string filename, int graphSize, float exploitation, float exploration) {
    ofstream csvFile;
    csvFile.open(filename);
    // Printing the header
    csvFile << "graphSize,antsNumber,antsIterations,evaporationRate,kConstant,pheromoneConstant,exploitation,exploration,seed" << endl;
    // Printing the metadata
    csvFile << graphSize << ",";
    csvFile << ANTS_N << ",";
    csvFile << ANTS_ITER << ",";
    csvFile << EVAP_RATE << ",";
    csvFile << K_CONST << ",";
    csvFile << PHER_CONST << ",";
    csvFile << exploitation << ",";
    csvFile << exploration << ",";
    csvFile << SEED << endl;
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


void createDirectoryInHome(string dir_name) {
    const char* home_dir = getenv("HOME");
    string output_dir = string(home_dir) + "/" + dir_name;
    filesystem::path dir(output_dir);
    if (!filesystem::exists(dir)) {
        if (filesystem::create_directory(dir)) {
            cout << "Output directory created: " << output_dir << endl;
        } else {
            cerr << "Failed to create output directory: " << output_dir << endl;
        }
    }
}
