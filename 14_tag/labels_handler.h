#ifndef LABELS_HANDLER_H
#define LABELS_HANDLER_H

typedef struct {
    char* label_name;
    int label_location;
} Labels;

typedef struct LabelsManager LabelsManager;

// Function to create a LabelsManager instance
LabelsManager* createLabelsManager(void);

// Function to add a label to the manager
void addLabel(LabelsManager* manager, const char* label_name, int label_location);

// Function to print all labels
void printLabels(const LabelsManager* manager);

// Function to get the location of a label by its name
int getLabelLocation(const LabelsManager* manager, const char* label_name);

// Function to destroy a LabelsManager instance and free resources
void destroyLabelsManager(LabelsManager* manager);

#endif // LABELS_HANDLER_H
