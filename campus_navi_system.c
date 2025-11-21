#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define NUM_LOCATIONS 5
#define MAX_NAME_LEN 20

// Node for adjacency list with distance
typedef struct Node {
    int vertex;
    int distance;
    struct Node* next;
} Node;

typedef struct Graph {
    char names[NUM_LOCATIONS][MAX_NAME_LEN];
    Node* adjLists[NUM_LOCATIONS];
} Graph;

Graph* createGraph();
void addEdge(Graph* graph, int src, int dest, int distance);
int getIndex(Graph* graph, const char* name);
void displayLocations(Graph* graph);
void displayAsciiConnections();
void dijkstraDirections(Graph* graph, int src, int dest);

int main() {
    // Hardcoded campus map setup
    Graph* graph = createGraph();

    // Building names
    strcpy(graph->names[0], "Gate");
    strcpy(graph->names[1], "Library");
    strcpy(graph->names[2], "Admin");
    strcpy(graph->names[3], "Lab");
    strcpy(graph->names[4], "Cafeteria");

    // Paths and distances
    addEdge(graph, 0, 1, 100);  // Gate-Library
    addEdge(graph, 1, 0, 100);

    addEdge(graph, 1, 3, 30);   // Library-Lab
    addEdge(graph, 3, 1, 30);

    addEdge(graph, 3, 4, 80);   // Lab-Cafeteria
    addEdge(graph, 4, 3, 80);

    addEdge(graph, 0, 2, 200);  // Gate-Admin
    addEdge(graph, 2, 0, 200);

    addEdge(graph, 1, 2, 50);   // Library-Admin
    addEdge(graph, 2, 1, 50);

    addEdge(graph, 2, 4, 120);  // Admin-Cafeteria
    addEdge(graph, 4, 2, 120);

    // Show buildings and connections (ASCII Art)
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n\n======Welcome to the Campus Navigation System======\n\n");
    displayLocations(graph);
    displayAsciiConnections();

    // User input (source and destination)
    char srcName[MAX_NAME_LEN], destName[MAX_NAME_LEN];
    printf("Please enter your start location (choose name from above): ");
    scanf("%s", srcName);
    printf("Please enter your end location (choose name from above): ");
    scanf("%s", destName);

    int srcIdx = getIndex(graph, srcName);
    int destIdx = getIndex(graph, destName);
    if (srcIdx == -1 || destIdx == -1) {
        printf("Invalid locations.\n");
        return 1;
    }

    dijkstraDirections(graph, srcIdx, destIdx);

    return 0;
}

// Utility functions
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i < NUM_LOCATIONS; ++i)
        graph->adjLists[i] = NULL;
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int distance) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->vertex = dest;
    node->distance = distance;
    node->next = graph->adjLists[src];
    graph->adjLists[src] = node;
}

int getIndex(Graph* graph, const char* name) {
    for (int i = 0; i < NUM_LOCATIONS; ++i)
        if (strcmp(graph->names[i], name) == 0)
            return i;
    return -1;
}

void displayLocations(Graph* graph) {
    printf("Campus Locations (Buildings):\n");
    for (int i = 0; i < NUM_LOCATIONS; ++i) {
        printf("%d. %s\n", i + 1, graph->names[i]);
    }
    printf("\n");
}

void displayAsciiConnections() {
    printf("Connections (distance in meters):\n");
    printf("Gate ----100----> Library --30--> Lab --80--> Cafeteria\n");
    printf("  \\                 |                          \n");
    printf("  200               50                          \n");
    printf("   \\                |                          \n");
    printf("   Admin ---120---> Cafeteria                  \n\n");
}

// Dijkstra's algorithm, with step-by-step directions
void dijkstraDirections(Graph* graph, int src, int dest) {
    int n = NUM_LOCATIONS;
    int dist[NUM_LOCATIONS], prev[NUM_LOCATIONS], visited[NUM_LOCATIONS] = {0};
    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1, minDist = INT_MAX;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = 1;
        Node* adj = graph->adjLists[u];
        while (adj != NULL) {
            int v = adj->vertex;
            int d = adj->distance;
            if (!visited[v] && dist[u] + d < dist[v]) {
                dist[v] = dist[u] + d;
                prev[v] = u;
            }
            adj = adj->next;
        }
    }

    // Path reconstruction
    int path[NUM_LOCATIONS], count = 0;
    for (int at = dest; at != -1; at = prev[at])
        path[count++] = at;
    if (dist[dest] == INT_MAX) {
        printf("No path found.\n");
        return;
    }

    printf("Directions to reach %s from %s:\n", graph->names[dest], graph->names[src]);
    for (int i = count - 1; i > 0; --i) {
        // Find the distance between path[i] and path[i-1]
        Node* tmp = graph->adjLists[path[i]];
        while (tmp && tmp->vertex != path[i-1])
            tmp = tmp->next;
        int stepDist = tmp ? tmp->distance : 0;
        printf("%d. Move from %s to %s (%d meters)\n", (count-i), graph->names[path[i]], graph->names[path[i-1]], stepDist);
    }
    printf("\nTotal distance: %d meters\n", dist[dest]);
}
