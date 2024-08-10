#include "file_manager.h"
#include "strings_manager.h"
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
		if (is_macro_name(macroManager, *split_line))
		{
			char*** processed_lines = get_macro_content(macroManager, *split_line);

			for (int i = 0; processed_lines[i] != NULL; i++) {
				char** row = processed_lines[i];  // Each row is a char**

				fileManager->post_macro = realloc(fileManager->post_macro, (fileManager->row_count + 1) * sizeof(char**));
				if (fileManager->post_macro == NULL) {
					fprintf(stderr, "Memory allocation failed\n");
					exit(EXIT_FAILURE);
				}
				fileManager->post_macro[fileManager->row_count] = row;
				fileManager->row_count++;
			}

		}
		else {
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

		}


		// Free the split line
		for (size_t i = 0; split_line[i] != NULL; ++i) {
			free(split_line[i]);
		}
		free(split_line);
	}
	fclose(file);
}

void print_post_macro(FileManager* manager) {
	printf("post_macro\n");
	if (manager->row_count == 0) {
		printf("No data to display.\n");
		return;
	}

	// Determine the maximum number of columns for proper formatting
	// depende on input file
	size_t max_columns = 0;
	for (size_t i = 0; i < manager->row_count; ++i) {
		size_t j = 0;
		while (manager->post_macro[i][j] != NULL) j++;
		if (j > max_columns) {
			max_columns = j;
		}
	}

	// Print the table
	printf("+");
	for (size_t col = 0; col < max_columns; ++col) {
		printf("------------+"); // Adjust the width of columns as needed
	}
	printf("\n");

	for (size_t i = 0; i < manager->row_count; ++i) {
		printf("|");
		for (size_t j = 0; manager->post_macro[i][j] != NULL; ++j) {
			printf(" %-10s |", manager->post_macro[i][j]); // Adjust the width of columns as needed
		}

		// Fill the remaining columns with empty spaces if the current row has fewer columns
		for (size_t j = 0; j < max_columns; ++j) {
			if (manager->post_macro[i][j] == NULL) {
				printf(" %-10s |", "");
			}
		}
		printf("\n");

		// Print the row separator
		printf("+");
		for (size_t col = 0; col < max_columns; ++col) {
			printf("------------+"); // Adjust the width of columns as needed
		}
		printf("\n");
	}
}

