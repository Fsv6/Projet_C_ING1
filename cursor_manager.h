#ifndef CURSOR_MANAGER_H
#define CURSOR_MANAGER_H

// Définition de la structure Cursor
typedef struct {
    int id;
    int x, y;
    int visible;
    char color[64];
    int thickness;
} Cursor;

// Fonctions publiques
Cursor *create_cursor(int id, int x, int y, int visible, const char *color, int thickness);
Cursor *get_cursor_by_id(int id);
void update_cursor_position(int id, int x, int y);
void update_cursor_color(int id, const char *color);
void free_cursors();

#endif
