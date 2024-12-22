#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cursor_manager.h"

// Tableau dynamique pour stocker les curseurs
static Cursor *cursors = NULL;
static int cursor_count = 0;

// Fonction pour initialiser un curseur
Cursor *create_cursor(int id, int x, int y, int visible, const char *color, int thickness) {
    // Agrandir le tableau dynamique
    cursors = realloc(cursors, (cursor_count + 1) * sizeof(Cursor));
    if (!cursors) {
        fprintf(stderr, "Erreur : Mémoire insuffisante pour créer un nouveau curseur.\n");
        return NULL;
    }

    // Initialiser les valeurs du nouveau curseur
    Cursor *new_cursor = &cursors[cursor_count++];
    new_cursor->id = id;
    new_cursor->x = x;
    new_cursor->y = y;
    new_cursor->visible = visible;
    strncpy(new_cursor->color, color, sizeof(new_cursor->color) - 1);
    new_cursor->color[sizeof(new_cursor->color) - 1] = '\0';
    new_cursor->thickness = thickness;

    return new_cursor;
}

// Fonction pour rechercher un curseur par son ID
Cursor *get_cursor_by_id(int id) {
    for (int i = 0; i < cursor_count; i++) {
        if (cursors[i].id == id) {
            return &cursors[i];
        }
    }
    return NULL;
}

// Fonction pour modifier les coordonnées d'un curseur
void update_cursor_position(int id, int x, int y) {
    Cursor *cursor = get_cursor_by_id(id);
    if (cursor) {
        cursor->x = x;
        cursor->y = y;
    } else {
        fprintf(stderr, "Erreur : Curseur avec ID %d introuvable.\n", id);
    }
}

// Fonction pour modifier la couleur d'un curseur
void update_cursor_color(int id, const char *color) {
    Cursor *cursor = get_cursor_by_id(id);
    if (cursor) {
        strncpy(cursor->color, color, sizeof(cursor->color) - 1);
        cursor->color[sizeof(cursor->color) - 1] = '\0';
    } else {
        fprintf(stderr, "Erreur : Curseur avec ID %d introuvable.\n", id);
    }
}


void free_cursors() {
    free(cursors);
    cursors = NULL;
    cursor_count = 0;
}
