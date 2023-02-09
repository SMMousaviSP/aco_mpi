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
    srand(time(NULL) + my_rank);

    T_GRAPH* graphData;
    T_PHER* pheromones;
    pheromones = generatePheromones(graphSize, 1.0);

    int bestLength = numeric_limits<T_GRAPH>::max();
    int worstLength = -1;
    int bestColony = -1;
    int worstColony = -1;

    T_GRAPH antLength;
    if (my_rank == 0) {
        // Distributing the graph
        graphData = generateNaiveGraph(graphSize, 1.0, 6.0);
        string graph_file_name = "out/graph_" + to_string(my_rank) + ".csv";
        saveGraph(graphData, graphSize, graph_file_name);
        // Broadcast the graph with MPI_Bcast
        o = "Rank 0 broadcasting graph to all colonies\n";
        cout << o;
        // MPI_Bcast(&graphData[0][0], graphSize * graphSize, MPI_T_GRAPH, 0, MPI_COMM_WORLD);
        for (int i = 1; i < comm_sz; i++) {
            o = "Rank 0 sending graph to colony CN " + to_string(i) + "\n";
            cout << o;
            MPI_Send(&graphData[0], graphSize * graphSize, MPI_T_GRAPH, i, 0, MPI_COMM_WORLD);
        }

        MPI_Status status;
        int source;
        for (int j = 0; j < comm_num; j++) {
            o = "-------------- CN " + to_string(j) + "\n";
            cout << o;
            for (int i = 1; i < comm_sz; i++) {
                MPI_Recv(&antLength, 1, MPI_T_PHER, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
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
            char mode = M_SEND_PHER;
            o = "Rank 0 sending mode 1 to best colony CN " + to_string(j) + "\n";
            cout << o;
            MPI_Send(&mode, 1, MPI_CHAR, bestColony, 0, MPI_COMM_WORLD);
            o = "Rank 0 getting best pheromone from best colony CN " + to_string(j) + "\n";
            cout << o;
            MPI_Recv(&pheromones[0], graphSize * graphSize, MPI_T_PHER, bestColony, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            if (UPDATE_STRATEGY == UPDATE_WORST) {
                // Send pheromones only to the worst colony
                mode = M_RECV_PHER;
                MPI_Send(&mode, 1, MPI_CHAR, worstColony, 0, MPI_COMM_WORLD);
                MPI_Send(&pheromones[0], graphSize * graphSize, MPI_T_PHER, worstColony, 0, MPI_COMM_WORLD);
            } else if (UPDATE_STRATEGY == UPDATE_ALL) {
                // Send pheromones to all colonies except the bestColony
                mode = M_RECV_PHER;
                for (int i = 1; i < comm_sz; i++) {
                    if (i != bestColony) {
                        MPI_Send(&mode, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
                        MPI_Send(&pheromones[0], graphSize * graphSize, MPI_T_PHER, i, 0, MPI_COMM_WORLD);
                    }
                }
            }

            // Broadcast mode 0 to all colonies so they can continue
            o = "Broadcasting mode 0 to all colonies CN " + to_string(j) + "\n";
            cout << o;
            mode = M_CONTINUE;
            for (int i = 1; i < comm_sz; i++) {
                MPI_Send(&mode, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }

            // Synchronizing all of the colonies after each communication
            MPI_Barrier(MPI_COMM_WORLD);
        
        }
        saveMetadata("out/metadata.csv",SIZE, alpha, beta);
        MPI_Barrier(MPI_COMM_WORLD);
        return 0;
    }


    // Receiving the broadcasted graph from rank 0
    graphData = generateEmptyGraph(graphSize);
    // MPI_Bcast(&graphData[0][0], graphSize * graphSize, MPI_T_GRAPH, 0, MPI_COMM_WORLD);
    MPI_Recv(&graphData[0], graphSize * graphSize, MPI_T_GRAPH, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    string graph_file_name = "out/graph_" + to_string(my_rank) + ".csv";
    saveGraph(graphData, graphSize, graph_file_name);

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
            char mode = -1; // 0: continue, 1: send pheromones, 2: receive and update pheromones
            AntPath bestAntPath = getBestAntPath(antPathArray, ANTS_N);
            bestLength = bestAntPath.pathLength;
            o = "Sending best length from iteration " + to_string(i + 1) + "\n";
            cout << o;
            MPI_Send(&bestLength, 1, MPI_T_PHER, 0, 0, MPI_COMM_WORLD);
            while (mode != M_CONTINUE) {
                o = "Getting mode in rank " + to_string(my_rank) + " and iteration " + to_string(i + 1) + "\n";
                cout << o;
                MPI_Recv(&mode, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                o = "Received mode " + to_string(mode) + " in rank " + to_string(my_rank) + " and iteration " + to_string(i + 1) + "\n";
                cout << o;
                if (mode == M_SEND_PHER) {
                    // Send the pheromones to the master
                    MPI_Send(&pheromones[0], graphSize * graphSize, MPI_T_PHER, 0, 0, MPI_COMM_WORLD);
                } else if (mode == M_RECV_PHER) {
                    // Receive the pheromones from the master
                    MPI_Recv(&pheromones[0], graphSize * graphSize, MPI_T_PHER, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
            MPI_Barrier(MPI_COMM_WORLD);
            
        }
    }
    string file_name = "out/result_" + to_string(my_rank) + ".csv";
    savePath(antPathArrayIter, file_name);

    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}
