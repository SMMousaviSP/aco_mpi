#include <iostream>
#include <vector>
#include <mpi.h>
#include <limits>
#include <string>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "settings.h"
#include "structs.h"
#include "utility.cpp"
#include "graph_gen/naivegen.cpp"
#include "ant.cpp"
#include "pheromone.cpp"

using namespace std;


const float alpha = ALPHA;
const float beta = BETA;

int main(int argc, char* argv[]) {
    int comm_sz;
    int my_rank;
    int comm_num = COMM_NUM;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Get thread count from command line
    int thread_count;
    string out_dir;
	try {
		if (argc != 3) {
			throw runtime_error("Invalid number of arguments. Enter exactly one integer\
					             for indicating the number of threads for OpenMP and one string\
                                 for indicating the folder name for output.");
		}
		thread_count = atoi(argv[1]);
        out_dir = argv[2];
    } catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    // Check if ANTS_ITER is divisible by comm_num
    if (ANTS_ITER % comm_num != 0) {
        cerr << "ANTS_ITER must be divisible by comm_num" << endl;
        return 1;
    }

    int comm_per_iter = ANTS_ITER / comm_num;

    int graphSize = SIZE;
    srand(time(NULL) + my_rank);

    T_GRAPH* graphData;
    T_PHER* pheromones;
    pheromones = generatePheromones(graphSize, 1.0);

    T_GRAPH antLength;
    if (my_rank == 0) {
        // Create the output directory
        createDirectoryInHome(out_dir);

        double start_time = MPI_Wtime();

        // Distributing the graph
        graphData = generateNaiveGraph(graphSize, GRAPH_MIN, GRAPH_MAX);
        for (int i = 1; i < comm_sz; i++) {
            MPI_Send(&graphData[0], graphSize * graphSize, MPI_T_GRAPH, i, 0, MPI_COMM_WORLD);
        }

        MPI_Status status;
        int source;
        for (int j = 0; j < comm_num; j++) {
            // Receiving path lengths from all colonies and finding the best and worst
            T_GRAPH bestLength = numeric_limits<T_GRAPH>::max();
            T_GRAPH worstLength = -1;
            int bestColony = -1;
            int worstColony = -1;
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
            MPI_Send(&mode, 1, MPI_CHAR, bestColony, 0, MPI_COMM_WORLD);
            MPI_Recv(&pheromones[0], graphSize * graphSize, MPI_T_PHER, bestColony, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Distributing the pheromone of the best colony based on the update strategy
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
            mode = M_CONTINUE;
            for (int i = 1; i < comm_sz; i++) {
                MPI_Send(&mode, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }

            // Synchronizing all of the colonies after each communication
            MPI_Barrier(MPI_COMM_WORLD);
        
        }

        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;

        saveMetadata(out_dir + "/metadata.csv", SIZE, alpha, beta, elapsed_time);
        MPI_Barrier(MPI_COMM_WORLD);
        return 0;
    }


    // Receiving the broadcasted graph from rank 0
    graphData = generateEmptyGraph(graphSize);
    // MPI_Bcast(&graphData[0][0], graphSize * graphSize, MPI_T_GRAPH, 0, MPI_COMM_WORLD);
    MPI_Recv(&graphData[0], graphSize * graphSize, MPI_T_GRAPH, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Running the Ants
    AntPath ** antPathArrayIter = new AntPath*[ANTS_ITER];
    T_GRAPH bestLength = numeric_limits<T_GRAPH>::max();
    for (int i = 0; i < ANTS_ITER; i++) {
        AntPath * antPathArray = new AntPath[ANTS_N];
        #pragma omp parallel for num_threads(thread_count)
        for (int j = 0; j < ANTS_N; j++) {
            // Choose a random starting node from 0 to SIZE
            int startingNode = rand() % SIZE;
            antPathArray[j] = antRun(startingNode, graphData, SIZE, pheromones, alpha, beta);
        }
        for (int j = 0; j < ANTS_N; j++) {
            depositAntPheromone(antPathArray[j], pheromones, SIZE);
        }
        evaporatePheromones(pheromones, SIZE);

        // Saving the result of each iteration
        antPathArrayIter[i] = antPathArray;

        // Sending path length and waiting for further instructions from the master
        if ((i + 1) % comm_per_iter == 0) {
            // Send the best ant path to the master
            char mode = -1; // 0: continue, 1: send pheromones, 2: receive and update pheromones
            T_GRAPH bestLength = getBestAntPath(antPathArray, ANTS_N);
            MPI_Send(&bestLength, 1, MPI_T_PHER, 0, 0, MPI_COMM_WORLD);
            while (mode != M_CONTINUE) {
                MPI_Recv(&mode, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
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

    string file_name = out_dir + "/result_" + to_string(my_rank) + ".csv";
    savePath(antPathArrayIter, file_name);

    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}
