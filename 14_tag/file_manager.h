#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char*** post_macro;
    size_t row_count;
} FileManager;

void initialize_file_manager(FileManager* manager);
void free_file_manager(FileManager* manager);
void input_process(FileManager* fileManager, MacroManager* macroManager, const char* file_path);
void print_post_macro(FileManager* manager);

#endif // FILE_MANAGER_H
