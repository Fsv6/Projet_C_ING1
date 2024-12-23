#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "axes_generator.h"
#include "cursor_manager.h"


/*
Process a single command from the input file and generate the corresponding Python code.

Args:
    line (const char*): The line containing the command to process.
    python_file (FILE*): The file where the generated Python code will be written.

Returns:
    None
*/
void process_command(const char *line, FILE *python_file) {
    char command_name[64], args[192];

    if (sscanf(line, "%63s %191[^\n]", command_name, args) < 1) {
        printf("Commande invalide ou vide : %s\n", line);
        return;
    }


    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(command_name, commands[i].name) == 0) {

            commands[i].execute(args, python_file);
            return;
        }
    }


    printf("Erreur : Commande inconnue : %s\n", command_name);
}



/*
Interpret a .draw file and generate a corresponding Python script.

Args:
    draw_filename (const char*): The path to the .draw file to interpret.
    output_python_filename (const char*): The path to the Python script to generate.

Returns:
    None
*/
void interpret_draw_file(const char *draw_filename, const char *output_python_filename) {
    FILE *draw_file = fopen(draw_filename, "r");
    if (draw_file == NULL) {
        printf("Error: Unable to open file %s.\n", draw_filename);
        return;
    }

    FILE *python_file = fopen(output_python_filename, "w");
    if (python_file == NULL) {
        printf("Error: Unable to create file %s.\n", output_python_filename);
        fclose(draw_file);
        return;
    }

    // Initialize Python script with imports and global variables
    fprintf(python_file, "import turtle\n\n");
    fprintf(python_file, "if 'cursors' not in globals():\n");
    fprintf(python_file, "    cursors = {}\n\n");

    // Generate code for axes
    generate_axes_code(python_file);

    char line[256];
    while (fgets(line, sizeof(line), draw_file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline character
        process_command(line, python_file);
    }

    // Finalize the Python script
    fprintf(python_file, "turtle.done()\n");

    fclose(draw_file);
    fclose(python_file);

    // Free allocated resources
    free_cursors();

    printf("Python file '%s' generated from '%s'.\n", output_python_filename, draw_filename);

    // Execute the generated Python script
    char command[512];
    snprintf(command, sizeof(command), "py %s", output_python_filename);
    int result = system(command);
    if (result != 0) {
        printf("Error: Unable to execute the Python file '%s'.\n", output_python_filename);
    } else {
        printf("Execution completed successfully.\n");
    }
}