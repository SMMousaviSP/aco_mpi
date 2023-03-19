#include <iostream>
#include <vector>
#include <chrono>

#include "settings.h"
#include "structs.h"
#include "utility.cpp"
#include "graph_gen/naivegen.cpp"
#include "ant.cpp"
#include "pheromone.cpp"

using namespace std;


const float alpha = ALPHA;
const float beta = BETA;

int main() {

    srand(time(NULL));

	std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

    int graphSize = SIZE;
    T_GRAPH* graphData;
    T_PHER* pheromones;
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

	std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    savePath(antPathArrayIter, "result.csv");
    saveMetadata("metadata.csv",SIZE, alpha, beta, elapsed_time.count());

    printGraph(graphData, SIZE);
    printPheromone(pheromones, SIZE);

    return 0;
}
