#include <iostream>
#include <vector>
#include <mpi.h>

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
    int comm_sz;
	int my_rank;
    int comm_num = COMM_NUM;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Check if ANTS_ITER is divisible by comm_num
    if (ANTS_ITER % comm_num != 0) {
        cout << "ANTS_ITER must be divisible by comm_num" << endl;
        return 1;
    }

    int graphSize = SIZE;
    srand(time(NULL));

    int best_length = 0;
    T_GRAPH best_ant_length;
    if (my_rank == 0) {
        for (int j = 0; j < comm_num; j++) {
            T_GRAPH * bestLengthArray = new T_GRAPH[comm_sz - 1];
            for (int i = 1; i < comm_sz; i++) {
                MPI_Recv(&best_ant_length, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                bestLengthArray[i - 1] = best_ant_length;
            }
			for (int i = 0; i < comm_sz - 1; i++) {
				cout << bestLengthArray[i] << endl;
			}
            MPI_Barrier(MPI_COMM_WORLD);
        }
        return 0;
    }

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

        if ((i + 1) % comm_num == 0) {
            // Send the best ant path to the master
            AntPath best_ant_path = getBestAntPath(antPathArray, ANTS_N);
            best_length = best_ant_path.pathLength;
            MPI_Send(&best_length, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }
    // savePath(antPathArrayIter, "result.csv");
    // saveMetadata("metadata.csv",SIZE, alpha, beta);

    // printGraph(graphData, SIZE);
    // printPheromone(pheromones, SIZE);


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


// MPI_Datatype create_mpi_type(const AntPath& s) {
//   int count = 2;
//   int blocklengths[2] = {1, s.path.size()};
//   MPI_Datatype types[2] = {MPI_FLOAT, MPI_FLOAT};
//   MPI_Aint offsets[2];
//   offsets[0] = offsetof(AntPath, pathLength);
//   offsets[1] = offsetof(AntPath, path);
//   MPI_Datatype mpi_type;
//   MPI_Type_create_struct(count, blocklengths, offsets, types, &mpi_type);
//   MPI_Type_commit(&mpi_type);
//   return mpi_type;
// }
