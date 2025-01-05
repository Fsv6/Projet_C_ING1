#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure_manager.h"

// Liste dynamique pour stocker les figures
static Figure *figures = NULL;
static int figure_count = 0;

// Fonction pour créer une figure
Figure *create_figure(const char *type, int x, int y, int size, const char *color, int cursor_id) {
    Figure *new_figure = malloc(sizeof(Figure));
    if (!new_figure) {
        fprintf(stderr, "Erreur : Mémoire insuffisante pour créer une nouvelle figure.\n");
        return NULL;
    }

    new_figure->id = ++figure_count;
    strncpy(new_figure->type, type, sizeof(new_figure->type) - 1);
    new_figure->x = x;
    new_figure->y = y;
    new_figure->size = size;
    strncpy(new_figure->color, color, sizeof(new_figure->color) - 1);
    new_figure->cursor_id = cursor_id;
    new_figure->next = figures;
    figures = new_figure;

    return new_figure;
}

// Fonction pour récupérer une figure par ID
Figure *get_figure_by_id(int id) {
    Figure *current = figures;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Fonction pour supprimer une figure
void delete_figure(int id) {
    Figure *current = figures;
    Figure *prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) {
                prev->next = current->next;
            } else {
                figures = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    fprintf(stderr, "Erreur : Figure avec ID %d introuvable.\n", id);
}

Figure *get_all_figures(void) {
    return figures;  // Renvoie le pointeur sur la tête de la liste
}

