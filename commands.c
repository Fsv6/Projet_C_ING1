#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "cursor_manager.h"

/*
List of available commands and their handlers.

Structure:
    commands[]: An array of Command structures linking command names to their corresponding handler functions.
*/
Command commands[] = {
    {"CURSOR", handle_cursor},
    {"COLOR", handle_color},
    {"THICKNESS", handle_thickness},
    {"MOVE", handle_move},
    {"ROTATE", handle_rotate},
    {"LINE", handle_line},
    {"CIRCLE", handle_circle},
    {"SQUARE", handle_square},
    {"POINT", handle_point},
    {"ARC", handle_arc},
    {NULL, NULL}
};

/*
CURSOR command: Creates a new cursor.

Args:
    args (const char*): The command arguments (ID, x, y, visibility).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    This command initializes a new turtle cursor in Python, placing it at the
    specified (x, y) position, and optionally setting its visibility.

Returns:
    None
*/
void handle_cursor(const char *args, FILE *python_file) {
    int id, x, y, visible = 1;

    // Lecture des arguments
    if (sscanf(args, "%d %d %d %d", &id, &x, &y, &visible) >= 3) {
        // Vérifie si le curseur existe déjà
        if (get_cursor_by_id(id)) {
            printf("Erreur : Curseur avec ID %d existe déjà.\n", id);
            return;
        }

        // Crée un nouveau curseur
        create_cursor(id, x, y, visible, "black", 1);

        // Génère le code Python pour initialiser le curseur
        fprintf(python_file, "if 'cursors' not in globals():\n");
        fprintf(python_file, "    cursors = {}\n");
        fprintf(python_file, "cursors[%d] = {\n", id);
        fprintf(python_file, "    'turtle': turtle.Turtle(),\n");
        fprintf(python_file, "    'x': %d,\n", x);
        fprintf(python_file, "    'y': %d,\n", y);
        fprintf(python_file, "    'visible': %d\n", visible);
        fprintf(python_file, "}\n");
        fprintf(python_file, "cursors[%d]['turtle'].color('black')\n", id);
        fprintf(python_file, "cursors[%d]['turtle'].pensize(1)\n", id);
        if (!visible) {
            fprintf(python_file, "cursors[%d]['turtle'].hideturtle()\n", id);
        }
        fprintf(python_file, "cursors[%d]['turtle'].penup()\n", id);
        fprintf(python_file, "cursors[%d]['turtle'].goto(%d, %d)\n", id, x, y);
    } else {
        printf("Erreur : Commande CURSOR invalide : %s\n", args);
    }
}


/*
COLOR command: Changes the color of a cursor.

Args:
    args (const char*): The command arguments (ID, color).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Updates the pen color for a specific cursor.

Returns:
    None
*/
void handle_color(const char *args, FILE *python_file) {
    int id;
    char color[64];

    if (sscanf(args, "%d %63s", &id, color) == 2) {
        if (get_cursor_by_id(id)) {
            update_cursor_color(id, color);
            fprintf(python_file, "if %d in cursors:\n", id);
            fprintf(python_file, "    cursors[%d]['turtle'].color('%s')\n", id, color);
        } else {
            printf("Error: Cursor with ID %d not found.\n", id);
        }
    } else {
        printf("Error: Invalid COLOR command: %s\n", args);
    }
}

/*
THICKNESS command: Changes the thickness of the pen for a cursor.

Args:
    args (const char*): The command arguments (ID, thickness).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Sets the pen thickness for a specific cursor, affecting all future drawings.

Returns:
    None
*/
void handle_thickness(const char *args, FILE *python_file) {
    int id, thickness;

    if (sscanf(args, "%d %d", &id, &thickness) == 2) {
        if (get_cursor_by_id(id)) {
            fprintf(python_file, "if %d in cursors:\n", id);
            fprintf(python_file, "    cursors[%d]['turtle'].pensize(%d)\n", id, thickness);
        } else {
            printf("Error: Cursor with ID %d not found.\n", id);
        }
    } else {
        printf("Error: Invalid THICKNESS command: %s\n", args);
    }
}

/*
MOVE command: Moves a cursor by a specified offset.

Args:
    args (const char*): The command arguments (ID, delta_x, delta_y).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Moves a cursor relative to its current position.

Returns:
    None
*/
void handle_move(const char *args, FILE *python_file) {
    int id, delta_x, delta_y;

    if (sscanf(args, "%d %d %d", &id, &delta_x, &delta_y) == 3) {
        Cursor *cursor = get_cursor_by_id(id);
        if (cursor) {
            update_cursor_position(id, cursor->x + delta_x, cursor->y + delta_y);
            fprintf(python_file, "if %d in cursors:\n", id);
            fprintf(python_file, "    t = cursors[%d]['turtle']\n", id);
            fprintf(python_file, "    t.penup()\n");
            fprintf(python_file, "    t.goto(%d, %d)\n", cursor->x, cursor->y);
            fprintf(python_file, "    t.pendown()\n");
        } else {
            printf("Error: Cursor with ID %d not found.\n", id);
        }
    } else {
        printf("Error: Invalid MOVE command: %s\n", args);
    }
}

/*
ROTATE command: Rotates the cursor by a specified angle.

Args:
    args (const char*): The command arguments (ID, angle).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Rotates a cursor by the given angle in degrees. Positive angles rotate
    counterclockwise, and negative angles rotate clockwise.

Returns:
    None
*/
void handle_rotate(const char *args, FILE *python_file) {
    int id;
    double angle;

    if (sscanf(args, "%d %lf", &id, &angle) == 2) {
        if (get_cursor_by_id(id)) {
            fprintf(python_file, "if %d in cursors:\n", id);
            fprintf(python_file, "    t = cursors[%d]['turtle']\n", id);
            if (angle >= 0) {
                fprintf(python_file, "    t.left(%lf)\n", angle);
            } else {
                fprintf(python_file, "    t.right(%lf)\n", -angle);
            }
        } else {
            printf("Erreur : Curseur avec ID %d introuvable.\n", id);
        }
    } else {
        printf("Erreur : Commande ROTATE invalide : %s\n", args);
    }
}

/*
LINE command: Draws a line from the cursor's current position to a specified point.

Args:
    args (const char*): The command arguments (ID, x, y).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Draws a straight line between the cursor's current position and the specified (x, y).

Returns:
    None
*/
void handle_line(const char *args, FILE *python_file) {
    int id, x, y;

    if (sscanf(args, "%d %d %d", &id, &x, &y) == 3) {
        Cursor *cursor = get_cursor_by_id(id);
        if (cursor) {
            update_cursor_position(id, x, y);
            fprintf(python_file, "if %d in cursors:\n", id);
            fprintf(python_file, "    t = cursors[%d]['turtle']\n", id);
            fprintf(python_file, "    t.pendown()\n");
            fprintf(python_file, "    t.goto(%d, %d)\n", x, y);
            fprintf(python_file, "    t.penup()\n");
        } else {
            printf("Erreur : Curseur avec ID %d introuvable.\n", id);
        }
    } else {
        printf("Erreur : Commande LINE invalide : %s\n", args);
    }
}

/*
CIRCLE command: Draws a circle of a given radius.

Args:
    args (const char*): The command arguments (ID, radius).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Draws a circle centered at the cursor's current position with the specified radius.

Returns:
    None
*/
void handle_circle(const char *args, FILE *python_file) {
    int id;
    double radius;

    if (sscanf(args, "%d %lf", &id, &radius) == 2) {
        if (get_cursor_by_id(id)) {
            fprintf(python_file, "if %d in cursors:\n", id);
            fprintf(python_file, "    t = cursors[%d]['turtle']\n", id);
            fprintf(python_file, "    t.pendown()\n");
            fprintf(python_file, "    t.circle(%lf)\n", radius);
            fprintf(python_file, "    t.penup()\n");
        } else {
            printf("Erreur : Curseur avec ID %d introuvable.\n", id);
        }
    } else {
        printf("Erreur : Commande CIRCLE invalide : %s\n", args);
    }
}

/*
SQUARE command: Draws a square with a specified side length.

Args:
    args (const char*): The command arguments (ID, side_length).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Draws a square starting from the cursor's current position with the specified side length.

Returns:
    None
*/
void handle_square(const char *args, FILE *python_file) {
    int id;
    double side_length;

    if (sscanf(args, "%d %lf", &id, &side_length) == 2) {
        if (get_cursor_by_id(id)) {
            fprintf(python_file, "if %d in cursors:\n", id);
            fprintf(python_file, "    t = cursors[%d]['turtle']\n", id);
            fprintf(python_file, "    t.pendown()\n");
            fprintf(python_file, "    for _ in range(4):\n");
            fprintf(python_file, "        t.forward(%lf)\n", side_length);
            fprintf(python_file, "        t.left(90)\n");
            fprintf(python_file, "    t.penup()\n");
        } else {
            printf("Erreur : Curseur avec ID %d introuvable.\n", id);
        }
    } else {
        printf("Erreur : Commande SQUARE invalide : %s\n", args);
    }
}

/*
POINT command: Draws a dot at the cursor's current position.

Args:
    args (const char*): The command arguments (ID).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Marks the cursor's current position with a dot.

Returns:
    None
*/
void handle_point(const char *args, FILE *python_file) {
    int id;
    if (sscanf(args, "%d", &id) == 1) {
        fprintf(python_file, "if %d in cursors:\n", id);
        fprintf(python_file, "    t = cursors[%d]['turtle']\n", id);
        fprintf(python_file, "    t.pendown()\n");
        fprintf(python_file, "    t.dot()\n");
        fprintf(python_file, "    t.penup()\n");
        fprintf(python_file, "    cursors[%d]['x'] = t.xcor()\n", id);
        fprintf(python_file, "    cursors[%d]['y'] = t.ycor()\n", id);
        fprintf(python_file, "    cursors[%d]['heading'] = t.heading()\n", id);
    } else {
        printf("Erreur : Commande POINT invalide : %s\n", args);
    }
}

/*
ARC command: Draws an arc with a given radius and angle.

Args:
    args (const char*): The command arguments (ID, radius, angle).
    python_file (FILE*): The Python file where the generated code will be written.

Purpose:
    Draws an arc of a circle using the specified radius and angle, starting from
    the cursor's current position.

Returns:
    None
*/
void handle_arc(const char *args, FILE *python_file) {
    int id;
    double radius, angle;
    if (sscanf(args, "%d %lf %lf", &id, &radius, &angle) == 3) {
        fprintf(python_file, "if %d in cursors:\n", id);
        fprintf(python_file, "    t = cursors[%d]['turtle']\n", id);
        fprintf(python_file, "    t.pendown()\n");
        fprintf(python_file, "    t.circle(%lf, %lf)\n", radius, angle);
        fprintf(python_file, "    t.penup()\n");
        fprintf(python_file, "    cursors[%d]['x'] = t.xcor()\n", id);
        fprintf(python_file, "    cursors[%d]['y'] = t.ycor()\n", id);
        fprintf(python_file, "    cursors[%d]['heading'] = t.heading()\n", id);
    } else {
        printf("Erreur : Commande ARC invalide : %s\n", args);
    }
}