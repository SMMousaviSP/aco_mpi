#include <iostream>
#include <vector>

#include "settings.h"
#include "structs.h"
#include "utility.cpp"
#include "graph_gen/naivegen.cpp"
#include "ant.cpp"
#include "pheromone.cpp"

using namespace std;

#define SIZE 5

const float alpha = 0.5;
const float beta = 0.5;

int main() {

    srand(time(NULL));

    int graphSize = SIZE;
    T_GRAPH** graphData;
    T_PHER** pheromones;
    graphData = generateNaiveGraph(graphSize, 1.0, 6.0);
    pheromones = generatePheromones(graphSize, 1.0);

    // Running the Ants
    AntPath ** antPathArrayIter = new AntPath*[ANTS_ITER];
    for (int i = 0; i < ANTS_ITER; i++) {
        AntPath * antPathArray = new AntPath[ANTS_N];
        for (int j = 0; j < ANTS_N; j++) {
            // Choose a random starting node from 0 to SIZE
            int startingNode = rand() % SIZE;
            antPathArray[j] = antRun(startingNode, graphData, SIZE, pheromones, alpha, beta);
        }
        for (int j = 0; j < ANTS_N; j++) {
            depositAntPheromone(antPathArray[j], pheromones, SIZE);
        }
        evaporatePheromones(pheromones, SIZE);

        // Saving the result of each iteration to a file
        antPathArrayIter[i] = antPathArray;
    }
    savePath(antPathArrayIter, "result.csv");
    saveMetadata("metadata.csv",SIZE, alpha, beta);

    printGraph(graphData, SIZE);
    printPheromone(pheromones, SIZE);


    // vector<int> neighbors = getNeighbors(2, graphData, graphSize);
    // vector<int> alreadyVisited;
    // alreadyVisited.push_back(1);
    // alreadyVisited.push_back(3);
    // vector<int> neighborsCleaned = eliminateAlreadyVisitedNeighbors(neighbors, alreadyVisited);

    // // print neighbors vector  
    // cout << "neighbors: " << endl;
    // for (int i = 0; i < neighbors.size(); i++) {
    //     cout << neighbors[i] << endl;
    // }
    // cout << endl;


    // // print neighborsCleaned vector
    // cout << "cleaned neighbors: " << endl;
    // for (int i = 0; i < neighborsCleaned.size(); i++) {
    //     cout << neighborsCleaned[i] << endl;
    // }
    // cout << endl;


    // calculate probability and print it
    // unordered_map<int, double> probability = calculateProbability(2, graphData, SIZE, pheromones, alpha, beta);
    // cout << "probability: " << endl;
    // for (auto const& x : probability) {
    //     cout << x.first << " " << x.second << endl;
    // }
    // cout << endl;

    // Evaporate pheromones and print it
    // evaporatePheromones(pheromones, graphSize);
    // cout << "evaporated pheromones: " << endl;
    // for (int i = 0; i < graphSize; i++) {
    //     for (int j = 0; j < graphSize; j++) {
    //         cout << pheromones[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // Create a sample antPath
    // AntPath antPath;
    // antPath.path.push_back(1);
    // antPath.path.push_back(2);
    // antPath.path.push_back(3);
    // antPath.path.push_back(4);
    // antPath.path.push_back(1);
    // antPath.pathLength = 10;
    // // Deposit pheromones and print it
    // depositAntPheromone(antPath, pheromones, SIZE);
    // cout << "deposited pheromones: " << endl;
    // for (int i = 0; i < graphSize; i++) {
    //     for (int j = 0; j < graphSize; j++) {
    //         cout << pheromones[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    return 0;
}
