#include <python_functions_writer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "commands.h"
#include "axes_generator.h"

// Maximum line length for file reading
#define MAX_LINE_LENGTH 1024
// Maximum number of keywords in the dictionary
#define MAX_KEYWORDS 100
// Maximum length for a single keyword
#define MAX_KEYWORD_LENGTH 100

// Finalizes the execution process by closing files and executing the generated Python script
// Parameters:
// - draw_file: Pointer to the opened .draw file
// - python_file: Pointer to the created Python file
// - output_python_filename: Name of the generated Python file
void finalize_execution(FILE *draw_file, FILE *python_file, const char *output_python_filename) {
    end_python_file(python_file);
    fclose(draw_file);
    fclose(python_file);

    char command[512];
    snprintf(command, sizeof(command), "py %s", output_python_filename);
    int result = system(command);
    if (result != 0) {
        printf("Error: Unable to execute the Python file '%s'.\n", output_python_filename);
    } else {
        printf("Execution completed successfully.\n");
    }
}

// Processes a single command line or a block command  and executes the corresponding function from the commands array
// Parameters:
// - command: Command from the .draw file
// - python_file: Pointer to the Python file being generated
void execute_command(const char *command, FILE *python_file) {
    char command_name[64], args[512];

    // Parse the command name and arguments from the line
    if (sscanf(command, "%63s %512[^\n]", command_name, args) < 1) {
        printf("Invalid or empty command: '%s'\n", command);
        return;
    }


    // Match the command name to a known command and execute its associated function
    for (int j = 0; commands[j].name != NULL; j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            commands[j].compile(args, python_file);
            return;
        }
    }

    printf("Unknown command: '%s'\n", command_name);
}

// Reads and processes commands from a .draw file, to delimitate one from another, handling block structures and single commands line
// Parameters:
// - draw_filename: Name of the .draw file
// - python_file: Pointer to the Python file being generated
void delimit_commands(const char *draw_filename, FILE *python_file) {

    FILE *draw_file = fopen(draw_filename, "r");
    if (draw_file == NULL) {
        printf("Error: Unable to open file %s.\n", draw_filename);
        return;
    }

    char line[256];
    char *buffer = NULL;
    size_t buffer_size = 0;
    int inside_block = 0;

    while (fgets(line, sizeof(line), draw_file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove the newline character

        // Detect the start of a block
        if (strchr(line, '{')) {
            if (!inside_block) {
                inside_block = 1;

                // Initialize the buffer
                buffer_size = strlen(line) + 2;
                buffer = malloc(buffer_size);
                if (!buffer) {
                    fprintf(stderr, "Memory allocation error\n");
                    fclose(draw_file);
                    return;
                }
                buffer[0] = '\0'; // Initialize as an empty string
            }

            // Append the line to the buffer
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);
            strcat(buffer, " ");
            continue;
        }

        // Detect continuation of a block in the case of "if {...} else {...}" structure
        if (strchr(line, '}') && strchr(line, '{')) {

            // Append the line to the buffer
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);
            strcat(buffer, " ");
            continue;
        }

        // Detect the end of a block
        if (strrchr(line, '}')) {

            // Append the line to the buffer
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);

            // Execute the complete block
            execute_command(buffer, python_file);

            // Clear the buffer
            free(buffer);
            buffer = NULL;
            buffer_size = 0;
            inside_block = 0;
            continue;
        }

        // Append to the buffer if inside a block
        if (inside_block) {
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);
            strcat(buffer, " ");
        } else {
            // Execute single-line commands outside of blocks
            execute_command(line, python_file);
        }
    }

    // Check for unclosed blocks
    if (buffer) {
        fprintf(stderr, "Warning: Unclosed block detected. Buffer content: '%s'\n", buffer);
        free(buffer);
    }

    fclose(draw_file);
}

// Checks if a word is in a list
// Parameters:
// - word: The word to check
// - list: The list of words
// - list_size: The size of the list
// Returns true if the word is found, otherwise false
bool is_in_list(const char *word, const char **list, int list_size) {
    for (int i = 0; i < list_size; i++) {
        if (strcmp(word, list[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Main function to process and interpret a .draw file : reads the file,
// detects commands keyword to and executes the associated functions and create the adequate functions in the Python script
// Parameters:
// - draw_filename: Name of the .draw file
// - output_python_filename: Name of the output Python file
void draw_handler(const char *draw_filename, const char *output_python_filename) {
    FILE *draw_file = fopen(draw_filename, "r");
    if (draw_file == NULL) {
        printf("Error: Unable to open file %s.\n", draw_filename);
        return;
    }

    FILE *python_file = fopen(output_python_filename, "w");
    if (python_file == NULL) {
        fclose(draw_file);
        printf("Error: Unable to create file %s.\n", output_python_filename);
        return;
    }

    set_python_file(python_file);

    const char *detected_commands[MAX_KEYWORDS];
    int detected_count = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), draw_file)) {
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            for (int i = 0; commands[i].name != NULL; i++) {
                if (strcmp(token, commands[i].name) == 0 && !is_in_list(token, detected_commands, detected_count)) {
                    detected_commands[detected_count] = strdup(token);
                    detected_count++;
                    break;
                }
            }
            token = strtok(NULL, " \t\n");
        }
    }

    // Execute associated functions for detected command keywords
    for (int i = 0; i < detected_count; i++) {
        bool handler_found = false;
        for (int j = 0; commands[j].name != NULL; j++) {
            if (strcmp(detected_commands[i], commands[j].name) == 0) {
                handler_found = true;
                if (commands[j].create != NULL) {
                    commands[j].create(python_file);
                } else {
                    printf("No handler function for command: %s\n", commands[j].name);
                }
                break;
            }
        }
        if (!handler_found) {
            printf("No handler found for command : %s\n", detected_commands[i]);
        }
    }

    // Free allocated memory for detected commands
    for (int i = 0; i < detected_count; i++) {
        free((void *)detected_commands[i]);
    }

    fclose(draw_file);

    delimit_commands(draw_filename, python_file);

    finalize_execution(draw_file, python_file, output_python_filename);
}
