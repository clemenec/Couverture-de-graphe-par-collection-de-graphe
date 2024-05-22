#define true 1
#define false 0
#define MIN -1
#define MAX 256

int BFS(Graph graph, int start);
int min_eccentricity(Graph graph_G);
int isType1(Graph graph, Graph *chain, int longueur);
int isType2(Graph graph, Graph *chain, int longueur);
int findLightestChain(Graph graph, Graph *chain, int longueur);