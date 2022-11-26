#include <stdio.h>
#include "graph_gen/naivegen.c"

#define SIZE 5



int main() {
    int graph_size = SIZE;
    float** graph_data;
    graph_data = generate_naive_graph(graph_size, 1.0, 6.0, 0);

    // run ants
    // @TODO

    // print graph_data
    for (int i = 0; i < graph_size; i++) {
        for (int j = 0; j < graph_size; j++) {
            printf("%f ", graph_data[i][j]);
        }
        printf("\n");
    }

    return 0;
}