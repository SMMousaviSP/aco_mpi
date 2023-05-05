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


/**
 * Saves the graph data to a CSV file.
 * 
 * @param graphData Pointer to the array of graph data
 * @param graphSize The size of the graph (number of nodes)
 * @param filename  The name of the file to save the data to
 */
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


/**
 * Prints the pheromone matrix to the console.
 * 
 * @param pheromones Pointer to the array of pheromone values
 * @param graphSize  The size of the graph (number of nodes)
 */
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


/**
 * Prints the given AntPath to the console.
 *
 * @param path An AntPath consisting of a vector of node indices and its length
 */
void printPath(AntPath path) {
    cout << "path: " << endl;
    for (int i = 0; i < path.path.size(); i++) {
        cout << path.path[i] << " ";
    }
    cout << endl;
    cout << "path cost: " << path.pathLength << endl;
    cout << endl;
}


/**
 * Saves a 2D array of AntPath objects to a CSV file with the specified filename.
 *
 * @param antPathArrayIter A pointer to a 2D array of AntPath objects to be saved.
 * @param filename The name of the CSV file to be created and saved to.
 */
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


/**
 * Saves experiment metadata to a CSV file.
 *
 * @param filename     The name or path of the CSV file to be created.
 * @param graphSize    The number of nodes/vertices in the graph.
 * @param exploitation The exploitation constant used in the algorithm.
 * @param exploration  The exploration constant used in the algorithm.
 * @param elapsedTime  The time elapsed during the algorithm's execution.
 */
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


/**
 * Calculates the average length of the best paths found by a group of ants.
 *
 * @param antPathArray An array of AntPath objects, each representing a path found by an ant.
 * @param arraySize    The size of the antPathArray.
 * @return             The average length of the best paths in antPathArray.
 */
T_GRAPH getBestAntPath(AntPath* antPathArray, int arraySize) {
    T_GRAPH sum_length;
    for (int i = 0; i < arraySize; i++) {
        sum_length = antPathArray[i].pathLength;
    }
    return sum_length / arraySize;
}


/**
* @brief Creates a new directory in the user's home directory.
* 
* This function takes in the name of the folder that needs to be created and creates a new directory within the user's home directory. 
* If the folder already exists, no action is taken.
*
* @param folderName The name of the folder that needs to be created.
*/
void createDirectoryInHome(string folderName) {
    string homeDir = getenv("HOME");
    string folderPath = homeDir + "/" + folderName;

    struct stat st;
    if (stat(folderPath.c_str(), &st) == -1) {
        mkdir(folderPath.c_str(), 0700);
    }
}
