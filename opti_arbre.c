#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "lecture_graph.h"
#include "parcours.h"

typedef struct GraphNode {
    Graph graph;
    struct GraphNode* next;
} GraphNode;

typedef struct {
    GraphNode* head;
} GraphList;

GraphList* createGraphList() {
    GraphList* list = (GraphList*) malloc(sizeof(GraphList));
    list->head = NULL;
    return list;
}

Graph copy_graph(Graph* source) {
    Graph copy = create_graph(source->V);
    for (int i = 0; i < source->V; ++i) {
        AdjencyList* adj = source->array[i];
        while (adj != NULL) {
            addEdge(&copy, i, adj->neighbour);
            adj = adj->next;
        }
    }
    return copy;
}

// Ajoute un graphe à la liste
void addGraphToList(GraphList* list, Graph* graph) {
    GraphNode* newNode = (GraphNode*) malloc(sizeof(GraphNode));
    newNode->graph = *graph;
    newNode->next = list->head;
    list->head = newNode;
}

// Affiche tous les graphes dans la liste
void displayGraphList(GraphList* list) {
    GraphNode* current = list->head;
    printf("F-couverture :\n");
    while (current != NULL) {
        printf("\n\n");
        display_graph(&current->graph);
        current = current->next;
    }
}

// Libère la mémoire de tous les graphes dans la liste
void deleteGraphList(GraphList* list) {
    GraphNode* current = list->head;
    while (current != NULL) {
        GraphNode* next = current->next;
        delete_graph(&current->graph);
        free(current);
        current = next;
    }
    free(list);
}

void markChainEdges(Graph* graph, Graph* chain) {
    for (int i = 0; i < chain->V; ++i) {
        AdjencyList *adj = chain->array[i];
        while (adj != NULL) {
            int src = i;
            int dest = adj->neighbour;
            AdjencyList *current = graph->array[src];
            while (current != NULL) {
                if (current->neighbour == dest) {
                    current->marked += 1;
                    break;
                }
                current = current->next;
            }
            adj = adj->next;
        }
    }
}

void deleteChain(Graph* graph, Graph* chain) {
    for (int i = 0; i < chain->V - 1; ++i) {
        AdjencyList *adj = chain->array[i];
        while (adj != NULL) {
            supprEdge(graph, i, adj->neighbour);
            adj = adj->next;
        }
    }
}

void addChain(Graph* graph, Graph* chain) {
    for (int i = 0; i < chain->V; ++i) {
        AdjencyList *adj = chain->array[i];
        while (adj != NULL) {
            addEdge(graph, i, adj->neighbour);
            adj = adj->next;
        }
    }
}

int main() {
    Graph graph_G = create_G("InputG.txt");
    Family family_F = create_F("InputF.txt");
    Graph g_couvrant;
    int isCouvrable = 0;
    display_graph(&graph_G);

    for (int i = 0; i < family_F.nb_graph; i++) {
        if (family_F.graph[i].V - 1 <= min_eccentricity(graph_G)) {
            printf("G est F-couvrable\n");
            g_couvrant = family_F.graph[i];
            isCouvrable = true;
            break;
        }
    }

    if (isCouvrable)
    {
        int chain_exist = 0;
        int chargeTotale = 0;
        int chainReadd = false;
        GraphList* markedChains = createGraphList();
        Graph chain = create_graph(graph_G.V);
        while (!AllEdgesMarked(graph_G))
        {
            if (isType1(graph_G, &chain, g_couvrant.V - 1) && !chainReadd) {
                deleteChain(&graph_G, &chain);
                chargeTotale += g_couvrant.V - 1;
                chain_exist = 1;
            } else if (isType2(graph_G, &chain, g_couvrant.V - 1) && !chainReadd) {
                deleteChain(&graph_G, &chain);
                chargeTotale += g_couvrant.V - 1;
                chain_exist = 1;
            } else if (findLightestChain(graph_G, &chain, g_couvrant.V - 1)) {
                deleteChain(&graph_G, &chain);
                chargeTotale += g_couvrant.V - 1;
                chain_exist = 1;
            }

            int min_ecc = min_eccentricity(graph_G);
            if(chain_exist && min_ecc != MIN && ((g_couvrant.V - 1) > min_ecc)) {
                addChain(&graph_G, &chain);
                markChainEdges(&graph_G, &chain);
                chainReadd = true;
            }

            if(chain_exist) {
                Graph chain_copy = copy_graph(&chain);
                addGraphToList(markedChains, &chain_copy);
                supprAllEdges(&chain);
            }

        }

        printf("Charge Totale : %d\n", chargeTotale);
        displayGraphList(markedChains);
        deleteGraphList(markedChains);
        delete_graph(&chain);
    } else {
        printf("G n'est pas F-couvrable\n");
    }

    delete_graph(&graph_G);
    delete_family(&family_F);

    return 0;
}