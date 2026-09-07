#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 6
#define INF 999999

typedef struct Edge
{
    int dest;
    float distance;
    int time;

    struct Edge *next;

} Edge;

typedef struct
{
    char name[50];

    Edge *head;

} Vertex;

Vertex graph[MAX_NODES + 1];

#endif