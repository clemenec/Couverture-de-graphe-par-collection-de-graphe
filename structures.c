#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

#define true 1
#define false 0

Graph create_graph(int V) {
    Graph graph;
    graph.V = V;
    graph.isLeaf = (int *) malloc(V * sizeof(int));
    graph.array = (AdjencyList **) malloc(V * sizeof(AdjencyList *));

    for (int i = 0; i < V; ++i) {
        graph.isLeaf[i] = true;
        graph.array[i] = NULL;
    }

    return graph;
}

// Vérification si une arête existe déjà
int edgeExists(Graph* graph, int src, int dest) {
    if (!graph)
        return false;

    AdjencyList* current = graph->array[src];
    while (current != NULL) {
        if (current->neighbour == dest) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Ajout d'un arc dirigé dans le graphe (si l'arc n'existe pas)
void addArc(Graph* graph, int src, int dest) {
    if (!edgeExists(graph, src, dest)) {
        AdjencyList *newNeighbour = (AdjencyList *) malloc(sizeof(AdjencyList));
        newNeighbour->neighbour = dest;
        newNeighbour->next = graph->array[src];
        newNeighbour->marked = false;
        graph->array[src] = newNeighbour;
        if (graph->array[src]->next != NULL)
            graph->isLeaf[src] = false;
        else
            graph->isLeaf[src] = true;
        }
}

// Ajout d'une arête non dirigée dans le graphe (si l'arête n'existe pas)
void addEdge(Graph* graph, int src, int dest) {
    addArc(graph, src, dest);
    addArc(graph, dest, src);
}

void supprArc(Graph* graph, int src, int dest) {
    AdjencyList *current = graph->array[src];
    AdjencyList *prev = NULL;

    while (current != NULL && current->neighbour != dest) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev == NULL) {
            graph->array[src] = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }

    // Mettre à jour isLeaf
    if (graph->array[src] == NULL || graph->array[src]->next == NULL) {
        graph->isLeaf[src] = true;
    } else {
        graph->isLeaf[src] = false;
    }
}

void supprEdge(Graph* graph, int src, int dest) {
    supprArc(graph, src, dest);
    supprArc(graph, dest, src);
}

void supprAllEdges(Graph* graph) {
    for (int i = 0; i < graph->V; ++i) {
        AdjencyList* current = graph->array[i];
        while (current != NULL) {
            // Stocke le voisin actuel avant de le supprimer
            int neighbor = current->neighbour;
            AdjencyList* next = current->next;

            // Supprime l'arc entre i et neighbor
            supprEdge(graph, i, neighbor);

            // Passe à l'arc suivant
            current = next;
        }
    }
}

int getDegree(Graph* graph, int vertex) {
    int degree = 0;
    AdjencyList* adj = graph->array[vertex];
    while (adj != NULL) {
        degree++;
        adj = adj->next;
    }
    return degree;
}

int AllEdgesMarked(Graph graph) {
    for (int i = 0; i < graph.V; ++i) {
        AdjencyList *adj = graph.array[i];
        while (adj != NULL) {
            if (!adj->marked) {
                return 0;
            }
            adj = adj->next;
        }
    }
    return 1;
}

Family create_family(int nb_graph, int *size_graph) {
    Family family_F;
    family_F.graph = (Graph *) calloc(nb_graph, sizeof(Graph));
    family_F.nb_graph = nb_graph;
    for (int i = 0; i < family_F.nb_graph; i++) {
        family_F.graph[i] = create_graph(size_graph[i]);
    }
    return family_F;
}

Graph G_example() {
    Graph graph_G = create_graph(9);
    addEdge(&graph_G, 0, 2); 
    addEdge(&graph_G, 1, 2); 
    addEdge(&graph_G, 2, 3); 
    addEdge(&graph_G, 4, 3); 
    addEdge(&graph_G, 3, 5); 
    addEdge(&graph_G, 5, 6); 
    addEdge(&graph_G, 5, 7); 
    addEdge(&graph_G, 7, 8); 

    return graph_G;
}

Family F_example() {
    int size_graph[3] = {4, 3, 7};
    Family family_f = create_family(3, size_graph);
    //graphe 1 :
    addEdge(&family_f.graph[0], 0, 1);
    addEdge(&family_f.graph[0], 1, 2);
    addEdge(&family_f.graph[0], 2, 3);
    //graphe 2 :
    addEdge(&family_f.graph[1], 0, 1);
    addEdge(&family_f.graph[1], 1, 2);
    //graphe 3 :
    addEdge(&family_f.graph[2], 0, 1);
    addEdge(&family_f.graph[2], 1, 2);
    addEdge(&family_f.graph[2], 2, 3);
    addEdge(&family_f.graph[2], 3, 4);
    addEdge(&family_f.graph[2], 4, 5);
    addEdge(&family_f.graph[2], 5, 6);

    return family_f;
}

void display_neighbour(AdjencyList *current) {
    if (!current)
        return;
    display_neighbour(current->next);
    printf("%d ", current->neighbour);
}

void display_graph(Graph *graph) {
    if (graph) {
        if (graph->array) {
            printf("%d sommets\n", graph->V);
            for (int i = 0; i < graph->V; ++i) {
                printf("%d adjency list : ", i);
                display_neighbour(graph->array[i]);
                printf("\n");
            }
        }
    }
}

void display_family(Family *family) {
    for(int i = 0; i < family->nb_graph; i++) {
        printf("graphe %d : \n", i);
        display_graph(&(family->graph[i]));
    }
}

void delete_neighbour(AdjencyList *current) {
    if (!current)
        return;
    delete_neighbour(current->next);
    free(current);
}

void delete_graph(Graph* graph) {
    if (graph) {
        if (graph->array) {
            for (int i = 0; i < graph->V; ++i) {
                delete_neighbour(graph->array[i]);
            }
            free(graph->array);
            free(graph->isLeaf);
        }
    }
}

void delete_family(Family *family) {
    for(int i = 0; i < family->nb_graph; i++) {
        delete_graph(&(family->graph[i]));
    }
    free(family->graph);
}
