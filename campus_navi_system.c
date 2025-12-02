#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
//including std files
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

// Function prototypes
Graph* createGraph();
void addEdge(Graph* graph, int src, int dest, int distance);
void initCampus(Graph* graph);

void displayLocations(Graph* graph);
void displayMapAndConnections(Graph* graph);
void dijkstraDirections(Graph* graph, int src, int dest);

int main() {
    Graph* graph = createGraph();
    initCampus(graph);

    int choice;
    while (1) {
        printf("=========================================\n");
        printf("      Campus Navigation System\n");
        printf("=========================================\n");
        printf("1. Display campus buildings\n");
        printf("2. Display campus map and connections\n");
        printf("3. Find shortest path between two locations\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        if (choice == 1) {
            displayLocations(graph);
        } else if (choice == 2) {
            displayMapAndConnections(graph);
        } else if (choice == 3) {
            int srcIdx, destIdx;
            displayLocations(graph);
            printf("Enter the number for your start location: ");
            scanf("%d", &srcIdx);
            printf("Enter the number for your end location: ");
            scanf("%d", &destIdx);
            srcIdx -= 1;
            destIdx -= 1;
            if (srcIdx < 0 || srcIdx >= NUM_LOCATIONS ||
                destIdx < 0 || destIdx >= NUM_LOCATIONS) {
                printf("Invalid selections.\n\n");
            } else {
                dijkstraDirections(graph, srcIdx, destIdx);
                printf("\n");
            }
        } else if (choice == 4) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n\n");
        }
    }

    return 0;
}

// Create empty graph
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i < NUM_LOCATIONS; ++i) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

// Add edge to adjacency list
void addEdge(Graph* graph, int src, int dest, int distance) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->vertex = dest;
    node->distance = distance;
    node->next = graph->adjLists[src];
    graph->adjLists[src] = node;
}

// Initialize hardcoded campus graph
void initCampus(Graph* graph) {
    // Names
    strcpy(graph->names[0], "Gate");
    strcpy(graph->names[1], "Library");
    strcpy(graph->names[2], "Admin");
    strcpy(graph->names[3], "Lab");
    strcpy(graph->names[4], "Cafeteria");

    // Undirected edges with distance
    addEdge(graph, 0, 1, 100);  // Gate - Library
    addEdge(graph, 1, 0, 100);

    addEdge(graph, 1, 3, 30);   // Library - Lab
    addEdge(graph, 3, 1, 30);

    addEdge(graph, 3, 4, 80);   // Lab - Cafeteria
    addEdge(graph, 4, 3, 80);

    addEdge(graph, 0, 2, 200);  // Gate - Admin
    addEdge(graph, 2, 0, 200);

    addEdge(graph, 1, 2, 50);   // Library - Admin
    addEdge(graph, 2, 1, 50);

    addEdge(graph, 2, 4, 120);  // Admin - Cafeteria
    addEdge(graph, 4, 2, 120);
}

// Display list of all buildings
void displayLocations(Graph* graph) {
    printf("\nCampus Locations (Buildings):\n");
    for (int i = 0; i < NUM_LOCATIONS; ++i) {
        printf("%d. %s\n", i + 1, graph->names[i]);
    }
    printf("\n");
}

// Display schematic map + fixed connection list
void displayMapAndConnections(Graph* graph) {
    (void)graph; // graph not used, kept for future extension

    printf("\nCampus Map (schematic, not to scale):\n\n");
    printf(" [Gate] ---- [Library] ---- [Lab] ---- [Cafeteria]\n");
    printf("    \\           |\n");
    printf("     \\          |\n");
    printf("      [Admin] ---+\n\n");

    printf("Connections (distance in meters):\n");
    printf("Gate       <-> Library    : 100\n");
    printf("Gate       <-> Admin      : 200\n");
    printf("Library    <-> Lab        : 30\n");
    printf("Library    <-> Admin      : 50\n");
    printf("Lab        <-> Cafeteria  : 80\n");
    printf("Admin      <-> Cafeteria  : 120\n\n");
}

// Dijkstra’s algorithm + directions
void dijkstraDirections(Graph* graph, int src, int dest) {
    int n = NUM_LOCATIONS;
    int dist[NUM_LOCATIONS];
    int prev[NUM_LOCATIONS];
    int visited[NUM_LOCATIONS] = {0};

    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[src] = 0;

    // Simple O(V^2) Dijkstra
    for (int i = 0; i < n; ++i) {
        int u = -1;
        int minDist = INT_MAX;
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
            if (!visited[v] && dist[u] != INT_MAX &&
                dist[u] + d < dist[v]) {
                dist[v] = dist[u] + d;
                prev[v] = u;
            }
            adj = adj->next;
        }
    }

    if (dist[dest] == INT_MAX) {
        printf("No path found between %s and %s.\n",
               graph->names[src], graph->names[dest]);
        return;
    }

    // Reconstruct path
    int path[NUM_LOCATIONS];
    int count = 0;
    for (int at = dest; at != -1; at = prev[at]) {
        path[count++] = at;
    }

    printf("\nDirections to reach %s from %s:\n",
           graph->names[dest], graph->names[src]);

    for (int i = count - 1; i > 0; --i) {
        // Find edge distance between path[i] and path[i-1]
        Node* tmp = graph->adjLists[path[i]];
        int stepDist = 0;
        while (tmp != NULL) {
            if (tmp->vertex == path[i - 1]) {
                stepDist = tmp->distance;
                break;
            }
            tmp = tmp->next;
        }
        printf("%d. Move from %s to %s (%d meters)\n",
               (count - i),
               graph->names[path[i]],
               graph->names[path[i - 1]],
               stepDist);
    }

    printf("\nTotal distance: %d meters\n", dist[dest]);
}
