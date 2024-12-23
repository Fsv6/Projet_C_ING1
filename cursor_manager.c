#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cursor_manager.h"

// Dynamic array to store cursors
static Cursor *cursors = NULL;
static int cursor_count = 0;

/*
Create a new cursor and add it to the dynamic array.

Args:
    id (int): Unique identifier for the cursor.
    x (int): Initial x-coordinate of the cursor.
    y (int): Initial y-coordinate of the cursor.
    visible (int): Visibility state of the cursor (1 for visible, 0 for hidden).
    color (const char*): Initial color of the cursor.
    thickness (int): Thickness of the cursor's drawing line.

Returns:
    Cursor*: Pointer to the newly created cursor, or NULL if memory allocation fails.
*/
Cursor *create_cursor(int id, int x, int y, int visible, const char *color, int thickness) {
    cursors = realloc(cursors, (cursor_count + 1) * sizeof(Cursor));
    if (!cursors) {
        fprintf(stderr, "Erreur : Mémoire insuffisante pour créer un nouveau curseur.\n");
        return NULL;
    }

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

/*
Retrieve a cursor by its unique ID.

Args:
    id (int): The unique identifier of the cursor.

Returns:
    Cursor*: Pointer to the cursor if found, or NULL if no such cursor exists.
*/
Cursor *get_cursor_by_id(int id) {
    for (int i = 0; i < cursor_count; i++) {
        if (cursors[i].id == id) {
            return &cursors[i];
        }
    }
    return NULL;
}

/*
Update the position of a cursor identified by its ID.

Args:
    id (int): The unique identifier of the cursor to update.
    x (int): The new x-coordinate for the cursor.
    y (int): The new y-coordinate for the cursor.
*/
void update_cursor_position(int id, int x, int y) {
    Cursor *cursor = get_cursor_by_id(id);
    if (cursor) {
        cursor->x = x;
        cursor->y = y;
    } else {
        fprintf(stderr, "Erreur : Curseur avec ID %d introuvable.\n", id);
    }
}

/*
Update the color of a cursor identified by its ID.

Args:
    id (int): The unique identifier of the cursor to update.
    color (const char*): The new color for the cursor.
*/
void update_cursor_color(int id, const char *color) {
    Cursor *cursor = get_cursor_by_id(id);
    if (cursor) {
        strncpy(cursor->color, color, sizeof(cursor->color) - 1);
        cursor->color[sizeof(cursor->color) - 1] = '\0';
    } else {
        fprintf(stderr, "Erreur : Curseur avec ID %d introuvable.\n", id);
    }
}

/*
Free the memory allocated for the dynamic cursor array and reset the cursor count.

Args:
    None

Returns:
    None
*/
void free_cursors() {
    free(cursors);
    cursors = NULL;
    cursor_count = 0;
}
