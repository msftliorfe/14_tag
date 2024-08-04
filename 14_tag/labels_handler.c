#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "labels_handler.h"

struct LabelsManager {
    Labels* array;
    size_t used;
    size_t size;
};

LabelsManager* createLabelsManager() {
    LabelsManager* manager = malloc(sizeof(LabelsManager));
    if (manager == NULL) {
        perror("Failed to create LabelsManager");
        exit(EXIT_FAILURE);
    }
    manager->array = malloc(5 * sizeof(Labels)); // Initial size of 5
    if (manager->array == NULL) {
        perror("Failed to allocate memory for Labels array");
        free(manager);
        exit(EXIT_FAILURE);
    }
    manager->used = 0;
    manager->size = 5;
    return manager;
}

void addLabel(LabelsManager* manager, const char* label_name, int label_location) {
    if (manager->used == manager->size) {
        manager->size *= 2;
        Labels* new_array = realloc(manager->array, manager->size * sizeof(Labels));
        if (new_array == NULL) {
            perror("Failed to reallocate memory for Labels array");
            free(manager->array);
            free(manager);
            exit(EXIT_FAILURE);
        }
        manager->array = new_array;
    }
    manager->array[manager->used].label_name = strdup(label_name); // Make a copy of the string
    if (manager->array[manager->used].label_name == NULL) {
        perror("Failed to duplicate label_name");
        free(manager->array);
        free(manager);
        exit(EXIT_FAILURE);
    }
    manager->array[manager->used].label_location = label_location;
    manager->used++;
}

void printLabels(const LabelsManager* manager) {
    for (size_t i = 0; i < manager->used; i++) {
        printf("Label: %s, Location: %d\n", manager->array[i].label_name, manager->array[i].label_location);
    }
}

int getLabelLocation(const LabelsManager* manager, const char* label_name) {
    for (size_t i = 0; i < manager->used; i++) {
        if (strcmp(manager->array[i].label_name, label_name) == 0) {
            return manager->array[i].label_location;
        }
    }
    return -1; // Indicate that the label was not found
}

void destroyLabelsManager(LabelsManager* manager) {
    for (size_t i = 0; i < manager->used; i++) {
        free(manager->array[i].label_name); // Free the copied strings
    }
    free(manager->array);
    free(manager);
}
