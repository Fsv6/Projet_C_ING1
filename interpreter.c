#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "axes_generator.h"
#include "cursor_manager.h"



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


void interpret_draw_file(const char *draw_filename, const char *output_python_filename) {
    FILE *draw_file = fopen(draw_filename, "r");
    if (draw_file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", draw_filename);
        return;
    }

    FILE *python_file = fopen(output_python_filename, "w");
    if (python_file == NULL) {
        printf("Erreur : Impossible de creer le fichier %s.\n", output_python_filename);
        fclose(draw_file);
        return;
    }


    fprintf(python_file, "import turtle\n\n");
    fprintf(python_file, "if 'cursors' not in globals():\n");
    fprintf(python_file, "    cursors = {}\n\n");


    generate_axes_code(python_file);

    char line[256];
    while (fgets(line, sizeof(line), draw_file)) {
        line[strcspn(line, "\n")] = '\0'; // Supprime le \n à la fin
        process_command(line, python_file);
    }


    fprintf(python_file, "turtle.done()\n");

    fclose(draw_file);
    fclose(python_file);

    free_cursors();

    printf("Fichier Python '%s' genere a partir de '%s'.\n", output_python_filename, draw_filename);

    char command[512];
    snprintf(command, sizeof(command), "py %s", output_python_filename);
    int result = system(command);
    if (result != 0) {
        printf("Erreur : Impossible d'executer le fichier Python '%s'.\n", output_python_filename);
    } else {
        printf("Execution terminee avec succes.\n");
    }
}
