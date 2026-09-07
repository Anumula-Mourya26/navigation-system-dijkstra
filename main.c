#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "navigation.h"
#include "graph.h"
#include "history.h"
FILE *resultFile;


void addEdge(int src, int dest, float distance, int time) {
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->distance = distance;
    newEdge->time = time;
    newEdge->next = graph[src].head;
    graph[src].head = newEdge;
}

void loadGraphFromFile() {
    FILE *fp = fopen("graph.txt", "r");

    if(fp == NULL) {
        printf("Cannot open graph.txt\n");
        return;
    }

    int src;
    int dest;
    float distance;
    int time;

    while(fscanf(fp, "%d %d %f %d", &src, &dest, &distance, &time) == 4) {
        addEdge(src, dest, distance, time);
    }

    fclose(fp);
}

void initializeGraph() {
    strcpy(graph[1].name, "VNR College");
    strcpy(graph[2].name, "Bachupally");
    strcpy(graph[3].name, "Miyapur");
    strcpy(graph[4].name, "GSM Mall");
    strcpy(graph[5].name, "Gandimaisamma");
    strcpy(graph[6].name, "JNTU");

    for(int i = 1; i <= MAX_NODES; i++) {
        graph[i].head = NULL;
    }
    loadGraphFromFile();
}

void displayGraph() {
    for(int i = 1; i <= MAX_NODES; i++) {
        printf("\n%s connects to:\n", graph[i].name);
        Edge *temp = graph[i].head;
        while(temp != NULL) {
            printf(" -> %s (%.2f km, %d min)\n", graph[temp->dest].name, temp->distance, temp->time);
            temp = temp->next;
        }
    }
}

int getMinDistance(float dist[], int visited[]) {
    float min = INF;
    int index = -1;

    for(int i = 1; i <= MAX_NODES; i++) {
        if(!visited[i] && dist[i] < min) {
            min = dist[i];
            index = i;
        }
    }
    return index;
}

void printPath(int parent[], int node) {
    if(parent[node] == -1) {
        fprintf(resultFile, "%s", graph[node].name);
        return;
    }
    printPath(parent, parent[node]);
    fprintf(resultFile, " --> %s", graph[node].name);
}

void printRouteDirections(int src, int dest) {
    FILE *fp = fopen("routes.txt", "r");

    if(fp == NULL) {
        fprintf(resultFile, "Directions unavailable\n");
        return;
    }

    int fileSrc, fileDest;
    char line[200];

    while(fscanf(fp, "%d %d", &fileSrc, &fileDest) == 2) {
        if(fileSrc == src && fileDest == dest) {
            while(fgets(line, sizeof(line), fp)) {
                if(strncmp(line, "END", 3) == 0) {
                    break;
                }
                char action[50];
                int distance;
                char road[100];

                if(sscanf(line, "%s %d %s", action, &distance, road) == 3) {
                    if(strcmp(action, "left") == 0) {
                        fprintf(resultFile, "Turn Left and continue for %d m\n", distance);
                    } else if(strcmp(action, "right") == 0) {
                        fprintf(resultFile, "Turn Right and continue for %d m\n", distance);
                    } else if(strcmp(action, "straight") == 0) {
                        fprintf(resultFile, "Continue Straight for %d m\n", distance);
                    } else if(strcmp(action, "roundabout") == 0) {
                        fprintf(resultFile, "Take Roundabout and continue for %d m\n", distance);
                    } else if(strcmp(action, "arrive") == 0) {
                        fprintf(resultFile, "Destination Reached\n");
                    }
                }
            }
            fclose(fp);
            return;
        } else {
            while(fgets(line, sizeof(line), fp)) {
                if(strncmp(line, "END", 3) == 0) {
                    break;
                }
            }
        }
    }

    fclose(fp);
}

void dijkstra(int source, int destination) {
    float dist[MAX_NODES + 1];
    int travelTime[MAX_NODES + 1];
    int visited[MAX_NODES + 1];
    int parent[MAX_NODES + 1];

    resultFile = fopen("result.txt", "w");

    if(resultFile == NULL) {
        printf("Cannot open result.txt\n");
        return;
    }

    for(int i = 1; i <= MAX_NODES; i++) {
        dist[i] = INF;
        travelTime[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[source] = 0;
    travelTime[source] = 0;

    for(int count = 1; count <= MAX_NODES; count++) {
        int u = getMinDistance(dist, visited);

        if(u == -1) break;

        visited[u] = 1;

        Edge *temp = graph[u].head;

        while(temp) {
            int v = temp->dest;

            if(!visited[v] && dist[u] + temp->distance < dist[v]) {
                dist[v] = dist[u] + temp->distance;
                travelTime[v] = travelTime[u] + temp->time;
                parent[v] = u;
            }

            temp = temp->next;
        }
    }

    fprintf(resultFile, "PATH:\n");
    printPath(parent, destination);
    fprintf(resultFile, "\n\nDISTANCE: %.2f km\n", dist[destination]);
    fprintf(resultFile, "TIME: %d min\n", travelTime[destination]);
    fprintf(resultFile, "\nDIRECTIONS:\n\n");

    int path[20];
    int count = 0;
    int current = destination;

    while(current != -1) {
        path[count++] = current;
        current = parent[current];
    }


    for(int i = count - 1; i > 0; i--)
    {
        fprintf( resultFile, "\n=== %s -> %s ===\n", graph[path[i]].name, graph[path[i - 1]].name);

        printRouteDirections(path[i],path[i - 1] );
    }

    fprintf( resultFile,"\nDestination Reached: %s\n", graph[destination].name);
}
int main() {
    initializeGraph();

    FILE *fp = fopen("request.txt", "r");
    if(fp == NULL) {
        printf("Cannot open request.txt\n");
        return 1;
    }

    int source, destination;
    fscanf(fp, "%d %d", &source, &destination);
    fclose(fp);

    dijkstra(source, destination);

    return 0;
}