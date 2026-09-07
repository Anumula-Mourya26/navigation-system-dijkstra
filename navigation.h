#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <stdio.h>
#include <string.h>

void showNavigationSteps(int source, int destination) {
    FILE *fp = fopen("routes.txt", "r");

    if(fp == NULL) {
        printf("\nCould not open routes.txt\n");
        return;
    }

    int src, dst;
    char line[200];
    int found = 0;

    while(fscanf(fp, "%d %d\n", &src, &dst) == 2) {
        if(src == source && dst == destination) {
            found = 1;
            printf("\nNavigation Instructions:\n\n");

            while(fgets(line, sizeof(line), fp)) {
                line[strcspn(line, "\n")] = '\0';

                if(strcmp(line, "END") == 0) break;

                printf("- %s\n", line);
            }
            break;
        }

        while(fgets(line, sizeof(line), fp)) {
            if(strcmp(line, "END\n") == 0) break;
        }
    }

    fclose(fp);

    if(!found) {
        printf("\nNo directions found.\n");
    }
}

#endif