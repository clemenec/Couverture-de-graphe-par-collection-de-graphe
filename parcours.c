#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "lecture_graph.h"
#include "parcours.h"

int BFS(Graph graph, int start) {
    int *visited = (int *) calloc(graph.V, sizeof(int)); 
    int distance[graph.V];
    memset(distance, MIN, graph.V);

    int queue[graph.V];
    int front = 0, rear = -1;
    int eccentricity = MIN;

    visited[start] = true; 
    distance[start] = 0;
    queue[++rear] = start;

    while (front <= rear) {
        int current = queue[front++];

        if (distance[current] > eccentricity)
            eccentricity = distance[current];

        AdjencyList *adj = graph.array[current];
        while (adj != NULL) {
            int adj_vertex = adj->neighbour;
            if (!visited[adj_vertex]) {
                visited[adj_vertex] = true;
                distance[adj_vertex] = distance[current] + 1;
                queue[++rear] = adj_vertex;
            }
            adj = adj->next;
        }
    }
    free(visited);

    return eccentricity;
}

int min_eccentricity(Graph graph_G) {
    int min_eccentricity = MIN;
    for (int i = 0; i < graph_G.V; i++) {
        if (graph_G.isLeaf[i]) {
            int ecc_vertex = BFS(graph_G, i);
            if (!ecc_vertex)
                continue;
                
            if (min_eccentricity == MIN || min_eccentricity > ecc_vertex) {
                min_eccentricity = ecc_vertex;
            }
        }
    }

    return min_eccentricity;
}

int isType1(Graph graph, Graph *chain, int longueur) {
    int *visited = (int *)calloc(graph.V, sizeof(int));
    int *parent = (int *)malloc(graph.V * sizeof(int));
    int *distance = (int *)malloc(graph.V * sizeof(int));
    int *queue = (int *)malloc(graph.V * sizeof(int));

    for (int start = 0; start < graph.V; ++start) {
        if (!graph.isLeaf[start]) continue;

        for (int i = 0; i < graph.V; ++i) {
            parent[i] = -1;
            distance[i] = -1;
            visited[i] = false;
        }

        distance[start] = 0;
        visited[start] = true;
        int front = 0, rear = 0;
        queue[rear++] = start;

        while (front < rear) {
            int current = queue[front++];
            AdjencyList *adj = graph.array[current];

            while (adj != NULL) {
                int adj_vertex = adj->neighbour;

                if (!visited[adj_vertex]) {
                    visited[adj_vertex] = true;
                    parent[adj_vertex] = current;
                    distance[adj_vertex] = distance[current] + 1;
                    queue[rear++] = adj_vertex;

                    if (graph.isLeaf[adj_vertex] && distance[adj_vertex] == longueur) {
                        int path_length = longueur + 1;
                        int path[path_length];
                        int path_index = path_length - 1;
                        int temp = adj_vertex;
                        int high_degree_count = 0;

                        while (temp != -1) {
                            if (getDegree(&graph, temp) > 2) {
                                high_degree_count++;
                            }
                            if (high_degree_count > 1) {
                                free(visited);
                                free(parent);
                                free(distance);
                                free(queue);
                                supprAllEdges(chain);
                                return 0;
                            }

                            path[path_index--] = temp;
                            temp = parent[temp];
                        }

                        for (int i = 0; i < path_length - 1; ++i) {
                            addEdge(chain, path[i], path[i + 1]);
                        }

                        free(visited);
                        free(parent);
                        free(distance);
                        free(queue);
                        return 1;
                    }
                }

                adj = adj->next;
            }
        }
    }

    free(visited);
    free(parent);
    free(distance);
    free(queue);
    return 0;
}

int isType2(Graph graph, Graph *chain, int longueur) {
    int *visited = (int *)calloc(graph.V, sizeof(int));
    int *parent = (int *)malloc(graph.V * sizeof(int));
    int *distance = (int *)malloc(graph.V * sizeof(int));
    int *queue = (int *)malloc(graph.V * sizeof(int));

    for (int start = 0; start < graph.V; ++start) {
        if (!graph.isLeaf[start]) continue;

        for (int i = 0; i < graph.V; ++i) {
            parent[i] = -1;
            distance[i] = -1;
            visited[i] = false;
        }

        distance[start] = 0;
        visited[start] = true;
        int front = 0, rear = 0;
        queue[rear++] = start;

        while (front < rear) {
            int current = queue[front++];
            AdjencyList *adj = graph.array[current];

            while (adj != NULL) {
                int adj_vertex = adj->neighbour;

                if (!visited[adj_vertex]) {
                    visited[adj_vertex] = true;
                    parent[adj_vertex] = current;
                    distance[adj_vertex] = distance[current] + 1;
                    queue[rear++] = adj_vertex;

                    if (distance[adj_vertex] == longueur && getDegree(&graph, adj_vertex) >= 2) {
                        int valid_chain = true;
                        int temp = adj_vertex;

                        for (int i = 0; i < longueur - 1; ++i) {
                            temp = parent[temp];
                            if (getDegree(&graph, temp) != 2) {
                                valid_chain = false;
                                break;
                            }
                        }

                        if (valid_chain) {
                            int path_length = longueur + 1;
                            int path[path_length];
                            int path_index = path_length - 1;
                            temp = adj_vertex;

                            while (temp != -1) {
                                path[path_index--] = temp;
                                temp = parent[temp];
                            }

                            for (int i = 0; i < path_length - 1; ++i) {
                                addEdge(chain, path[i], path[i + 1]);
                            }

                            free(visited);
                            free(parent);
                            free(distance);
                            free(queue);
                            return 1;
                        }
                    }
                }

                adj = adj->next;
            }
        }
    }

    free(visited);
    free(parent);
    free(distance);
    free(queue);
    return 0;
}

int findLightestChain(Graph graph, Graph *chain, int longueur) {
    int *visited = (int *)calloc(graph.V, sizeof(int));
    int *parent = (int *)malloc(graph.V * sizeof(int));
    int *distance = (int *)malloc(graph.V * sizeof(int));
    int *queue = (int *)malloc(graph.V * sizeof(int));

    int lightestWeight = MAX;

    for (int start = 0; start < graph.V; ++start) {
        if (!graph.isLeaf[start]) continue;

        for (int i = 0; i < graph.V; ++i) {
            parent[i] = -1;
            distance[i] = -1;
            visited[i] = false;
        }

        distance[start] = 0;
        visited[start] = true;
        int front = 0, rear = 0;
        queue[rear++] = start;

        while (front < rear) {
            int current = queue[front++];
            AdjencyList *adj = graph.array[current];

            while (adj != NULL) {
                int adj_vertex = adj->neighbour;

                if (!visited[adj_vertex]) {
                    visited[adj_vertex] = true;
                    parent[adj_vertex] = current;
                    distance[adj_vertex] = distance[current] + 1;
                    queue[rear++] = adj_vertex;

                    if (distance[adj_vertex] == longueur) {
                        int temp = adj_vertex;
                        int path_length = longueur + 1;
                        int path[path_length];
                        int path_index = path_length - 1;
                        int totalWeight = 0;

                        while (temp != -1) {
                            path[path_index--] = temp;
                            temp = parent[temp];
                        }

                        for (int i = 0; i < path_length - 1; ++i) {
                            AdjencyList *edge = graph.array[path[i]];
                            while (edge != NULL && edge->neighbour != path[i + 1]) {
                                edge = edge->next;
                            }
                            if (edge != NULL) {
                                totalWeight += (1 + edge->marked);
                            }
                        }

                        if (totalWeight < lightestWeight) {
                            lightestWeight = totalWeight;
                            // Supprimer l'ancienne meilleure chaîne
                            supprAllEdges(chain);
                            // Ajouter les arcs de la nouvelle meilleure chaine
                            for (int i = 0; i < path_length - 1; ++i) {
                                addEdge(chain, path[i], path[i + 1]);
                            }
                        }
                    }
                }

                adj = adj->next;
            }
        }
    }

    if (lightestWeight == MAX) {
        free(visited);
        free(parent);
        free(distance);
        free(queue);
        return 0;
    }

    free(visited);
    free(parent);
    free(distance);
    free(queue);
    return 1;
}
