//Chained List to define vertex and their neighbours
typedef struct AdjencyList {
    int neighbour;
    int marked;
    struct AdjencyList *next;
} AdjencyList;

//Structure to define a graph
typedef struct {
    int V;
    int *isLeaf;
    AdjencyList **array;
} Graph;

//Structure to define a family
typedef struct {
    int nb_graph;
    Graph *graph;
} Family;

Graph create_graph(int V);

void addEdge(Graph *graph, int src, int dest);

void supprEdge(Graph *graph, int src, int dest);

void supprAllEdges(Graph* graph);

int getDegree(Graph* graph, int vertex);

int AllEdgesMarked(Graph graph);

Family create_family(int nb_graph, int *size_graph);

Graph G_example();

Family F_example();

void delete_graph(Graph *graph);

void delete_family(Family *family);

void display_graph(Graph *graph);

void display_family(Family *family);
