#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HistoryNode {
    char source[50];
    char destination[50];
    float distance;
    int time;
    struct HistoryNode *next;
} HistoryNode;

HistoryNode *head = NULL;

void addHistory(char source[], char destination[], float distance, int time) {
    HistoryNode *newNode = (HistoryNode*)malloc(sizeof(HistoryNode));

    strcpy(newNode->source, source);
    strcpy(newNode->destination, destination);
    newNode->distance = distance;
    newNode->time = time;

    newNode->next = head;
    head = newNode;
}

void showHistory() {
    HistoryNode *temp = head;

    printf("\n===== HISTORY =====\n");

    while(temp) {
        printf("%s -> %s | %.2f km | %d min\n", temp->source, temp->destination, temp->distance, temp->time);
        temp = temp->next;
    }
}

#endif