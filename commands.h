#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

// Définition de la structure Command
typedef struct {
    const char *name;
    void (*execute)(const char *args, FILE *python_file);
} Command;

// Prototypes des fonctions
void handle_cursor(const char *args, FILE *python_file);
void handle_color(const char *args, FILE *python_file);
void handle_thickness(const char *args, FILE *python_file);
void handle_move(const char *args, FILE *python_file);
void handle_rotate(const char *args, FILE *python_file);
void handle_line(const char *args, FILE *python_file);
void handle_circle(const char *args, FILE *python_file);
void handle_square(const char *args, FILE *python_file);
void handle_point(const char *args, FILE *python_file);
void handle_arc(const char *args, FILE *python_file);

// Déclaration du tableau de commandes
extern Command commands[];

#endif // COMMANDS_H

