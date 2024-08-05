#include "file_manager.h"
#include "strings.h"
#include "macro_manager.h"
#include <string.h>


void initialize_file_manager(FileManager* manager) {
    manager->post_macro = NULL;
    manager->row_count = 0;

}

void free_file_manager(FileManager* manager) {
    for (size_t i = 0; i < manager->row_count; ++i) {
        for (size_t j = 0; manager->post_macro[i][j] != NULL; ++j) {
            free(manager->post_macro[i][j]);
        }
        free(manager->post_macro[i]);
    }
    free(manager->post_macro);
}

void input_process(FileManager* fileManager, MacroManager* macroManager, const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", file_path);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        char** split_line = split_string(line);
        size_t split_count = 0;
        while (split_line[split_count] != NULL) split_count++;

        char** processed_line = process_file_line(macroManager, split_line, split_count);

        if (processed_line != NULL) {
            fileManager->post_macro = realloc(fileManager->post_macro, (fileManager->row_count + 1) * sizeof(char**));
            if (fileManager->post_macro == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            fileManager->post_macro[fileManager->row_count] = processed_line;
                fileManager->row_count++;
        }

        // Free the split line
        for (size_t i = 0; split_line[i] != NULL; ++i) {
            free(split_line[i]);
        }
        free(split_line);
    }


    //print_post_macro(fileManager);
    fclose(file);


}

void print_post_macro(FileManager* manager) {
    for (size_t i = 0; i < manager->row_count; ++i) {
        for (size_t j = 0; manager->post_macro[i][j] != NULL; ++j) {
            printf("%s ", manager->post_macro[i][j]);
        }
        printf("\n");
    }
}
