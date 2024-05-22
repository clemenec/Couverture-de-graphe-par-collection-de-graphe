#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structures.h"
#include "lecture_graph.h"

Graph create_G(char *file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int V = 0;
    char line[128];
    fgets(line, sizeof(line), file);
    sscanf(line, "sommets : %d", &V);
    Graph G = create_graph(V);

    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);
    int src = -1;
    int dest = -1;
    char *ptr = strtok(line, ";");
    while (ptr != NULL) {
        sscanf(ptr, "[%d,%d]", &src, &dest);
        addEdge(&G, src, dest);
        ptr = strtok(NULL, ";");
    }

    fclose(file);
    return G;
}

Family create_F(char *file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int nb_graph = 0;
    char line[128];
    fgets(line, sizeof(line), file);
    sscanf(line, "graphes : %d", &nb_graph);
    int *size_graph = (int *) calloc(nb_graph, sizeof(int));
    int current_graph = 0;

    while (fgets(line, sizeof(line), file) != NULL)
        if (sscanf(line, "sommets : %d", &size_graph[current_graph]) == 1)
            current_graph++;

    rewind(file);
    Family F = create_family(nb_graph, size_graph);
    free(size_graph);

    current_graph = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '[') {
            int src = -1;
            int dest = -1;
            char *ptr = strtok(line, ";");
            while (ptr != NULL) {
                sscanf(ptr, "[%d,%d]", &src, &dest);
                addEdge(&F.graph[current_graph], src, dest);
                ptr = strtok(NULL, ";");
            }
            current_graph++;
        }
    }

    fclose(file);
    return F;
}