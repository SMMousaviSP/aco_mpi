#include <iostream>
#include <vector>
#include <mpi.h>
#include <limits>
#include <string>

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

    int comm_per_iter = ANTS_ITER / comm_num;

    string o;

    int graphSize = SIZE;
    srand(time(NULL));

    T_PHER** pheromones;
    pheromones = generatePheromones(graphSize, 1.0);

    int bestLength = numeric_limits<T_GRAPH>::max();
    int worstLength = -1;
    int bestColony = -1;
    int worstColony = -1;

    T_GRAPH antLength;
    if (my_rank == 0) {
        MPI_Status status;
        int source;
        for (int j = 0; j < comm_num; j++) {
            o = "-------------- CN " + to_string(j) + "\n";
            cout << o;
            for (int i = 1; i < comm_sz; i++) {
                MPI_Recv(&antLength, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                source = status.MPI_SOURCE;
                if (antLength < bestLength) {
                    bestLength = antLength;
                    bestColony = source;
                }
                if (antLength > worstLength) {
                    worstLength = antLength;
                    worstColony = source;
                }
            }

            // Get pheromones from the best colony
            short mode = 1;
            o = "Rank 0 sending mode 1 to best colony CN " + to_string(j) + "\n";
            cout << o;
            MPI_Send(&mode, 1, MPI_SHORT, bestColony, 0, MPI_COMM_WORLD);
            o = "Rank 0 getting best pheromone from best colony CN " + to_string(j) + "\n";
            cout << o;
            MPI_Recv(&pheromones[0][0], graphSize * graphSize, MPI_FLOAT, bestColony, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            if (UPDATE_STRATEGY == UPDATE_WORST) {
                // Send pheromones only to the worst colony
                mode = 2;
                MPI_Send(&mode, 1, MPI_SHORT, worstColony, 0, MPI_COMM_WORLD);
                MPI_Send(&pheromones[0][0], graphSize * graphSize, MPI_FLOAT, worstColony, 0, MPI_COMM_WORLD);
            } else if (UPDATE_STRATEGY == UPDATE_ALL) {
                // Send pheromones to all colonies except the bestColony
                mode = 2;
                for (int i = 1; i < comm_sz; i++) {
                    if (i != bestColony) {
                        MPI_Send(&mode, 1, MPI_SHORT, i, 0, MPI_COMM_WORLD);
                        MPI_Send(&pheromones[0][0], graphSize * graphSize, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
                    }
                }
            }

            // Broadcast mode 0 to all colonies so they can continue
            o = "Broadcasting mode 0 to all colonies CN " + to_string(j) + "\n";
            cout << o;
            mode = 0;
            for (int i = 1; i < comm_sz; i++) {
                MPI_Send(&mode, 1, MPI_SHORT, i, 0, MPI_COMM_WORLD);
            }

            // Synchronizing all of the colonies after each communication
            MPI_Barrier(MPI_COMM_WORLD);
        
        }
        return 0;
    }

    T_GRAPH** graphData;
    graphData = generateNaiveGraph(graphSize, 1.0, 6.0);

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

        if ((i + 1) % comm_per_iter == 0) {
            // Send the best ant path to the master
            short mode = -1; // 0: continue, 1: send pheromones, 2: receive and update pheromones
            AntPath bestAntPath = getBestAntPath(antPathArray, ANTS_N);
            bestLength = bestAntPath.pathLength;
            o = "Sending best length from iteration " + to_string(i + 1) + "\n";
            cout << o;
            MPI_Send(&bestLength, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            while (mode != 0) {
                o = "Getting mode in rank " + to_string(my_rank) + " and iteration " + to_string(i + 1) + "\n";
                cout << o;
                MPI_Recv(&mode, 1, MPI_SHORT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                o = "Received mode " + to_string(mode) + " in rank " + to_string(my_rank) + " and iteration " + to_string(i + 1) + "\n";
                cout << o;
                if (mode == 1) {
                    // Send the pheromones to the master
                    MPI_Send(&pheromones[0][0], graphSize * graphSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                } else if (mode == 2) {
                    // Receive the pheromones from the master
                    MPI_Recv(&pheromones[0][0], graphSize * graphSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
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
