#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "interpreter.h"


typedef struct {
    const char *name;
    void (*execute)(const char *args, FILE *python_file);
} Command;


void handle_cursor(const char *args, FILE *python_file) {
    int x, y, visible = 1;
    if (sscanf(args, "%d %d %d", &x, &y, &visible) >= 2) {
        fprintf(python_file, "t.penup()\n");
        fprintf(python_file, "t.goto(%d, %d)\n", x, y);
        if (visible) {
            fprintf(python_file, "t.pendown()\n");
        }
    } else {
        printf("Erreur : Commande CURSOR invalide : %s\n", args);
    }
}

// Tableau des commandes disponibles
Command commands[] = {
    {"CURSOR", handle_cursor},
    {NULL, NULL}
};

// Fonction pour interpréter et traiter une commande
void process_command(const char *line, FILE *python_file) {
    char command_name[64], args[192];
    // Séparer le nom de la commande et ses arguments
    if (sscanf(line, "%63s %191[^\n]", command_name, args) < 1) {
        printf("Commande invalide ou vide : %s\n", line);
        return;
    }

    // Parcourir les commandes disponibles
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(command_name, commands[i].name) == 0) {
            // Trouvé, appeler la fonction associée
            commands[i].execute(args, python_file);
            return;
        }
    }

    // Si aucune commande correspondante n'a été trouvée
    printf("Erreur : Commande inconnue : %s\n", command_name);
}

// Fonction principale pour interpréter un fichier .draw
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

    // Initialisation du fichier Python
    fprintf(python_file, "import turtle\n\n");
    fprintf(python_file, "t = turtle.Turtle()\n\n");

    char line[256];
    while (fgets(line, sizeof(line), draw_file)) {
        line[strcspn(line, "\n")] = '\0'; // Supprime le \n à la fin
        process_command(line, python_file);
    }

    // Finalisation du fichier Python
    fprintf(python_file, "turtle.done()\n");

    fclose(draw_file);
    fclose(python_file);

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
