#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "commands.h"

#include <ctype.h>

#include "cursor_manager.h"

#include "interpreter.h"



/*
List of available commands and their handlers.

Structure:
    commands[]: An array of Command structures linking command names to their corresponding handler functions.
*/
Command commands[] = {
    {"CURSOR", call_cursor_func_py, create_cursor_func_py},
    {"COLOR", call_color_func_py, create_color_func_py},
    {"THICKNESS", call_thickness_func_py, create_thickness_func_py},
    {"MOVE", call_move_func_py, create_move_func_py},
    {"ROTATE", call_rotate_func_py, create_rotate_func_py},
    {"LINE", call_line_func_py, create_line_func_py},
    {"CIRCLE", call_circle_func_py, create_circle_func_py},
    {"SQUARE", call_square_func_py, create_square_func_py},
    {"RECTANGLE", call_rectangle_func_py, call_rectangle_func_py},
    {"POINT", call_point_func_py, create_point_func_py},
    {"ARC", call_arc_func_py, create_arc_func_py},
    {"ANIME", call_animation_func_py, create_animation_func_py},
    {"IF",handle_if_python , NULL},
    {"FOR", handle_for_python, NULL},
    {"SET", handle_set_variable, NULL},
    {"WHILE", handle_while_python,NULL},
    {"BREAK", handle_break_python, NULL},
    {"CONTINUE", handle_continue_python, NULL},
    {"PASS", handle_end_python, NULL},

    {NULL, NULL, NULL} // Fin de tableau
};


// Fonction pour récupérer les noms des commandes
const char** get_command_names(Command commands[], int* count) {
    int command_count = 0;

    // Comptez le nombre de commandes valides
    for (int i = 0; commands[i].name != NULL; i++) {
        command_count++;
    }
    *count = command_count;

    // Allouez un tableau pour les noms des commandes
    const char** names = (const char**)malloc(command_count * sizeof(const char*));
    for (int i = 0; i < command_count; i++) {
        names[i] = commands[i].name;
    }
    return names;
}


void set_python_file(FILE *python_file) {
    // Génère le code Python pour initialiser le curseur
    fprintf(python_file, "import turtle\n");
    fprintf(python_file, "import time\n");
    fprintf(python_file, "import math\n");
    fprintf(python_file, "if 'cursors' not in globals():\n");
    fprintf(python_file, "    cursors = {}\n");
    fprintf(python_file, "    turtle.setup(800, 600)\n");
    fprintf(python_file, "    turtle.title('Projects Cursors')\n");
    fprintf(python_file, "    turtle.tracer(n=2, delay=10)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "def set_all_cursors_speed(speed):\n");
    fprintf(python_file, "    for cursor in cursors.values():\n");
    fprintf(python_file, "        cursor['turtle'].speed(speed)\n");
    fprintf(python_file, "shapes = []\n");
    fprintf(python_file, "animation_shapes = []\n");

}
void end_python_file(FILE *python_file) {
    // Génère le code Python pour initialiser le curseur
    fprintf(python_file, "turtle.done()\n");
}


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
void create_cursor_func_py(FILE *python_file) {
    int id, x, y, visible = 1;
    printf("coucouoeiruzoeiruzoieru");
    // Crée un nouveau curseur
    create_cursor(id, x, y, visible, "black", 1);

    // Génère le code Python pour initialiser le curseur
    fprintf(python_file, "def handle_cursor(id, x, y, visible):\n");
    fprintf(python_file, "    if id not in cursors:\n");
    fprintf(python_file, "        cursors[id] = {\n");
    fprintf(python_file, "            'turtle': turtle.Turtle(),\n");
    fprintf(python_file, "            'x': x,\n");
    fprintf(python_file, "            'y': y,\n");
    fprintf(python_file, "            'visible': visible,\n");
    fprintf(python_file, "            'color': 'black',\n");
    fprintf(python_file, "            'thickness': 1\n");
    fprintf(python_file, "        }\n");
    fprintf(python_file, "        cursors[id]['turtle'].color('black')\n");
    fprintf(python_file, "        cursors[id]['turtle'].pensize(1)\n");
    fprintf(python_file, "        cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "        cursors[id]['turtle'].speed(1)\n");
    fprintf(python_file, "    turtle.tracer(0)\n");
    fprintf(python_file, "    cursors[id]['x'] = x\n");
    fprintf(python_file, "    cursors[id]['y'] = y\n");
    fprintf(python_file, "    cursors[id]['turtle'].setpos(x, y)\n");
    fprintf(python_file, "    if  (visible == 'TRUE') :\n");
    fprintf(python_file, "        cursors[id]['turtle'].showturtle()\n");
    fprintf(python_file, "    else :\n");
    fprintf(python_file, "        cursors[id]['turtle'].hideturtle()\n");
    fprintf(python_file, "    turtle.tracer(1)\n");
    fprintf(python_file, "    turtle.update()\n");


}
//handle_cursor('cursor1', 100, 100, True)

void call_cursor_func_py(const char *args,FILE *python_file) {
    char id[64];
    char visible[64];
    char x[64];
    char y[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s %63s %63s", id, &x, &y, visible) != 4) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Vérifiez que les valeurs extraites sont valides
    printf("Parsed values - id: %s, x: %s, y: %s, visible: %s\n", id, x, y, visible);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_cursor(%s, %s, %s, '%s')\n", id, x, y, visible);
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
void create_color_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_color(id, color):\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['color'] = color\n\n");
    //handle_color('cursor1', 'red')
}
void call_color_func_py(const char *args, FILE *python_file) {
    char id[64];
    char color[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s", id, color) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, color : %s", id, color);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_color(%s, '%s')\n", id, color);
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
void create_thickness_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_thickness(id, thickness):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pensize(thickness)\n");
    fprintf(python_file, "    cursors[id]['thickness'] = thickness\n");
    //handle_thickness('cursor1', 1)
}
void call_thickness_func_py(const char *args, FILE *python_file) {
    char id[64];
    int thickness;

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %d", id, thickness) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, thickness : %d", id, thickness);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_thickness('%s', %d)\n", id, thickness);
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
void create_move_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_move(id, distance):\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(distance)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
}
void call_move_func_py(const char *args, FILE *python_file) {
    char id[64];
    char distance[100];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %100s", id, distance) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, distance : %s", id, distance);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_move(%s, %s)\n", id, distance);
}


void create_goto_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_goto(id, x, y):\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(x, y)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
}
void call_goto_func_py(const char *args, FILE *python_file) {
    char id[64];
    char x[100];
    char y[100];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %100s %100s", id, x, y) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, x : %s, y : %s", id, x, y);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_goto(%s, %s, %s)\n", id, x, y);
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
void create_rotate_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_rotate(id, angle):\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(angle)\n");
}
void call_rotate_func_py(const char *args, FILE *python_file) {
    char id[64];
    char angle[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s", id, angle) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, angle : %s", id, angle);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_rotate('%s', %s)\n", id, angle);
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
void create_line_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_line(id, position, distance, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(distance)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), distance, 'line'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}
void call_line_func_py(const char *args, FILE *python_file) {
    char id[64];
    char distance[64];
    char id_form[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s %63s", id, distance, id_form) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, distance : %s, id_form : %s", id, distance, id_form);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_line(%s, 'null', %s, %s)\n", id, distance, id_form);
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
void create_circle_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_circle(id, position, radius, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].circle(radius)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), radius, 'circle'))\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}
void call_circle_func_py(const char *args, FILE *python_file) {
    char id[64];
    char radius[64];
    char id_form[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s %63s", id, radius, id_form) !=3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, radius : %s, id_form: %s", id, radius, id_form);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_circle(%s, 'null', %s, %s)\n", id, radius, id_form);
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
void create_square_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_square(id, position, side_length, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    for _ in range(4):\n");
    fprintf(python_file, "        cursors[id]['turtle'].forward(side_length)\n");
    fprintf(python_file, "        cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), side_length, 'square'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}
void call_square_func_py(const char *args, FILE *python_file) {
    char id[64];
    char side_length[64];
    char id_form[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s %63s", id, side_length, id_form) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, side_length : %s, id_form: %s", id, side_length, id_form);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_square(%s, 'null', %s, %s)\n", id, side_length, id_form);
}






void create_rectangle_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_rectangle(id, position, side_1, side_2, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_1)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_2)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_1)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_2)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), side_length, 'square'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}
void call_rectangle_func_py(const char *args, FILE *python_file) {
    char id[64];
    char side_1[64];
    char side_2[64];
    char id_form[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s %63s %63s", id, side_1, side_2, id_form) != 4) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, side_1 : %s, side_2 : %s, id_form: %s", id, side_1, side_2, id_form);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_rectangle(%s, 'null', %s, %s, %s)\n", id, side_1, side_2, id_form);
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
void create_point_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_point(id, position, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].dot()\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), 0, 'point'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}
void call_point_func_py(const char *args, FILE *python_file) {
    char id[64];
    char id_form[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s 63s", id, id_form) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, id_form: %s", id, id_form);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_point('%s', 'null', %s)\n", id, id_form);
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
void create_arc_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_semi_circle(id, position, radius, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].circle(radius, 180)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(180)\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), radius, 'semi-circle'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}
void call_arc_func_py(const char *args, FILE *python_file) {
    char id[64];
    char radius[64];
    char id_form[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s %63s", id, radius, id_form) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, radius : %s, id_form: %s", id, radius, id_form);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_square(%s, 'null', %s, %s)\n", id, radius, id_form);
}




void create_animation_func_py(FILE *python_file) {

    printf("create_animation_func_py\n");

    fprintf(python_file, "def animation(ids, move_distance, angle_degrees, show):\n");
    fprintf(python_file, "    global animation_shapes\n");
    fprintf(python_file, "    global shapes\n");
    fprintf(python_file, "    final_animation = []\n");
    fprintf(python_file, "    angle_radians = math.radians(angle_degrees)\n");
    fprintf(python_file, "    dx = move_distance * math.cos(angle_radians)\n");
    fprintf(python_file, "    dy = move_distance * math.sin(angle_radians)\n");
    fprintf(python_file, "    for shape in shapes[:]:  # On parcourt une copie de la liste shapes\n");
    fprintf(python_file, "        if shape[1] in ids:  # Si l'id est dans la liste d'animation\n");
    fprintf(python_file, "            animation_shapes.append(shape)\n");
    fprintf(python_file, "            shapes.remove(shape)  # Retirer l'élément de shapes\n");
    fprintf(python_file, "    if (show):\n");
    fprintf(python_file, "        for shape in animation_shapes:\n");
    fprintf(python_file, "            if shape[1] in ids:\n");
    fprintf(python_file, "                cursors[shape[1]]['turtle'].showturtle()\n");
    fprintf(python_file, "    else:\n");
    fprintf(python_file, "        for shape in animation_shapes:\n");
    fprintf(python_file, "            if shape[1] in ids:\n");
    fprintf(python_file, "                cursors[shape[1]]['turtle'].hideturtle()\n");
    fprintf(python_file, "    for shape in animation_shapes[:]:\n");
    fprintf(python_file, "        if shape[1] in ids:  # Si l'id est dans la liste d'animation\n");
    fprintf(python_file, "            if shape[4] == 'circle':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_circle(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le cercle en blanc\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_circle(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'line':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_line(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le carré en blanc\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_line(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'square':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_square(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le carré en blanc\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_square(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'semi-circle':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_semi_circle(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le carré en blanc\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_semi_circle(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'point':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_point(shape[1], shape[2], shape[0], 'white')  # Effacer le carré en blanc\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_point(shape[1], new_position, shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "    # Redessiner les formes restantes de shapes qui ne sont pas animées\n");
    fprintf(python_file, "    for shape in shapes:\n");
    fprintf(python_file, "        if shape[4] == 'circle':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_circle(shape[1], shape[2], shape[3])  # Redessiner le cercle\n");
    fprintf(python_file, "        elif shape[4] == 'line':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_line(shape[1], shape[2], shape[3])  # Redessiner le carré\n");
    fprintf(python_file, "        elif shape[4] == 'square':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_line(shape[1], shape[2], shape[3])  # Redessiner le carré\n");
    fprintf(python_file, "        elif shape[4] == 'semi-circle':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_semi_circle(shape[1], shape[2], shape[3])  # Redessiner le carré\n");
    fprintf(python_file, "        elif shape[4] == 'point':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_point(shape[1], shape[2], shape[3])  # Redessiner le carré\n");
    fprintf(python_file, "    shapes.extend(final_animation)\n");
    fprintf(python_file, "    animation_shapes = []  # Reset animation list after the animation is done\n");
}
void call_animation_func_py(const char *args, FILE *python_file) {
    char ids[64];
    char move_distance[64];
    char move_r[64];
    char angle_degrees[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %63s %63s %63s", ids, move_distance, move_r, angle_degrees) != 4) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - ids: %s, move_distance : %s, move_r : %s, angle_degrees : %s", ids, move_distance, move_r, angle_degrees);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "set_all_cursors_speed(0)\n");
    fprintf(python_file, "turtle.tracer(n=1, delay=0)\n");
    fprintf(python_file, "for i in range(%s):\n", move_r);
    fprintf(python_file, "    time.sleep(0.1)\n");
    fprintf(python_file, "    if (i == %s - 1):\n", move_r);
    fprintf(python_file, "        show = True\n");
    fprintf(python_file, "    else:\n");
    fprintf(python_file, "        show = False\n");
    fprintf(python_file, "    animation(%s, %s, %s, show)\n", ids, move_distance, angle_degrees);
    fprintf(python_file, "turtle.tracer(n=1, delay=10)\n");
    fprintf(python_file, "set_all_cursors_speed(1)\n");
}


void handle_if_python(const char *args, FILE *python_file) {
    char clean_args[512] = "";
    char cursor_id[64] = "";
    char x_condition[64] = "";
    char y_condition[64] = "";
    char if_block[256] = "";
    char else_block[256] = "";

    // Nettoyer les espaces superflus
    int i = 0, j = 0;
    while (args[i] != '\0') {
        if (!isspace(args[i]) || (j > 0 && !isspace(clean_args[j - 1]))) {
            clean_args[j++] = args[i];
        }
        i++;
    }
    clean_args[j] = '\0';

    // Afficher les arguments nettoyés pour débogage
    printf("Arguments nettoyés : '%s'\n", clean_args);

    // Localiser l'ID du curseur
    char *cursor_start = strstr(clean_args, "CURSOR WITH ID = ");
    if (cursor_start) {
        cursor_start += strlen("CURSOR WITH ID = ");
        sscanf(cursor_start, "%63s", cursor_id);
    }

    // Localiser la condition X
    char *x_start = strstr(clean_args, "X ");
    if (x_start) {
        x_start += 2; // Sauter "X "
        sscanf(x_start, "%63[^A{]", x_condition);
    }

    // Localiser la condition Y
    char *y_start = strstr(clean_args, "Y ");
    if (y_start) {
        y_start += 2; // Sauter "Y "
        sscanf(y_start, "%63[^A{]", y_condition);
    }

    // Localiser le bloc IF
    char *if_start = strchr(clean_args, '{');
    char *else_start = strstr(clean_args, "} ELSE {");
    if (if_start && else_start) {
        // Extraire le bloc IF
        strncpy(if_block, if_start + 1, else_start - if_start - 1);
        if_block[else_start - if_start - 1] = '\0';

        // Extraire le bloc ELSE
        char *else_content_start = else_start + strlen("} ELSE {");
        char *else_content_end = strchr(else_content_start, '}');
        if (else_content_end) {
            strncpy(else_block, else_content_start, else_content_end - else_content_start);
            else_block[else_content_end - else_content_start] = '\0';
        }
    }
    // Afficher les résultats pour débogage
    printf("ID du curseur : %s\n", cursor_id);
    printf("Condition X : '%s'\n", x_condition);
    printf("Condition Y : '%s'\n", y_condition);
    printf("Bloc IF : '%s'\n", if_block);
    printf("Bloc ELSE : '%s'\n", else_block);

    // Générer la condition Python
    fprintf(python_file, "if ");
    if (strlen(x_condition) > 0 && strlen(y_condition) > 0) {
        // Conditions sur X et Y
        fprintf(python_file, "cursors[%s]['x'] %s and cursors[%s]['y'] %s:\n",
                cursor_id, x_condition, cursor_id, y_condition);
    } else if (strlen(x_condition) > 0) {
        // Condition sur X uniquement
        fprintf(python_file, "cursors[%s]['x'] %s:\n", cursor_id, x_condition);
    } else if (strlen(y_condition) > 0) {
        // Condition sur Y uniquement
        fprintf(python_file, "cursors[%s]['y'] %s:\n", cursor_id, y_condition);
    }

    // Traiter chaque commande du bloc IF avec indentation
    char *command_start = if_block;
    while (*command_start != '\0') {
        // Localiser la fin de la commande (virgule ou fin du bloc)
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        // Extraire la commande
        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        // Supprimer les espaces en début et fin de commande
        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        // Ajouter l'indentation avant d'appeler execute_command
        fprintf(python_file, "    ");
        execute_command(trimmed_command, python_file);

        // Passer à la commande suivante
        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }

    // Compiler et écrire le bloc ELSE
    fprintf(python_file, "else:\n");
    command_start = else_block;
    while (*command_start != '\0') {
        // Localiser la fin de la commande (virgule ou fin du bloc)
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        // Extraire la commande
        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        // Supprimer les espaces en début et fin de commande
        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        // Ajouter l'indentation avant d'appeler execute_command
        fprintf(python_file, "    ");
        execute_command(trimmed_command, python_file);

        // Passer à la commande suivante
        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }
}


void handle_for_python(const char *args, FILE *python_file) {
    char clean_args[512] = "";
    char loop_var[64] = "";
    char range_expr[128] = "";
    char loop_block[256] = "";

    // Nettoyer les espaces superflus
    int i = 0, j = 0;
    while (args[i] != '\0') {
        if (!isspace(args[i]) || (j > 0 && !isspace(clean_args[j - 1]))) {
            clean_args[j++] = args[i];
        }
        i++;
    }
    clean_args[j] = '\0';

    // Afficher les arguments nettoyés pour débogage
    printf("Arguments nettoyés : '%s'\n", clean_args);

    // Détecter automatiquement la variable de boucle et l'expression IN
    char *in_pos = strstr(clean_args, " IN ");
    if (in_pos) {
        // Extraire la variable de boucle (avant " IN ")
        strncpy(loop_var, clean_args, in_pos - clean_args);
        loop_var[in_pos - clean_args] = '\0';

        // Extraire l'expression après " IN "
        char *block_start = strchr(in_pos, '{');
        if (block_start) {
            strncpy(range_expr, in_pos + 4, block_start - (in_pos + 4));
            range_expr[block_start - (in_pos + 4)] = '\0';

            // Supprimer le terme "RANGE" si présent
            char *range_keyword = strstr(range_expr, "RANGE");
            if (range_keyword) {
                char *range_start = strchr(range_expr, '(');
                char *range_end = strchr(range_expr, ')');
                if (range_start && range_end && range_end > range_start) {
                    // Extraire uniquement l'expression à l'intérieur des parenthèses
                    memmove(range_expr, range_start + 1, range_end - range_start - 1);
                    range_expr[range_end - range_start - 1] = '\0';
                }
            }

            // Extraire le bloc de la boucle (entre { et })
            char *block_end = strrchr(block_start, '}');
            if (block_end) {
                strncpy(loop_block, block_start + 1, block_end - block_start - 1);
                loop_block[block_end - block_start - 1] = '\0';
            }
        }
    }

    // Vérifications de débogage
    printf("Variable de boucle : '%s'\n", loop_var);
    printf("Expression de range/liste : '%s'\n", range_expr);
    printf("Bloc de la boucle : '%s'\n", loop_block);


    fprintf(python_file, "for %s in range(%s):\n", loop_var, range_expr);


    // Traiter chaque commande du bloc avec indentation
    char *command_start = loop_block;
    while (*command_start != '\0') {
        // Localiser la fin de la commande (virgule ou fin du bloc)
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        // Extraire la commande
        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        // Supprimer les espaces en début et fin de commande
        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        // Ajouter l'indentation avant d'appeler execute_command
        fprintf(python_file, "    ");
        execute_command(trimmed_command, python_file);

        // Passer à la commande suivante
        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }
}



void handle_set_variable(const char *args, FILE *python_file) {
    char variable_name[64];
    char expression[256];

    // Extraction de la commande
    int result = sscanf(args, "VARIABLE %63s = %255[^\n]", variable_name, expression);
    if (result != 2) {
        fprintf(stderr, "Erreur : commande VARIABLE mal formatée.\n");
        fprintf(stderr, "Commande reçue : '%s'\n", args);
        fprintf(stderr, "Résultat sscanf : %d\n", result);
        return;
    }

    printf("Variable : '%s', Expression : '%s'\n", variable_name, expression);

    // Vérification du nom de la variable
    for (char *c = variable_name; *c; ++c) {
        if (!((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c == '_') || (*c >= '0' && *c <= '9' && c != variable_name))) {
            fprintf(stderr, "Erreur : nom de variable invalide '%s'.\n", variable_name);
            return;
        }
    }

    // Vérification de l'expression
    if (strlen(expression) == 0) {
        fprintf(stderr, "Erreur : expression vide pour la variable '%s'.\n", variable_name);
        return;
    }

    // Écriture de la commande dans le fichier Python
    fprintf(python_file, "%s = %s\n", variable_name, expression);
    fflush(python_file); // Assurer que les données sont écrites immédiatement
}

void handle_while_python(const char *args, FILE *python_file) {
    char clean_args[1024];
    char cursor_id[64] = "";
    char x_condition[64] = "";
    char y_condition[64] = "";
    char loop_block[512] = "";

    // Nettoyer les espaces superflus
    int i = 0, j = 0;
    while (args[i] != '\0') {
        if (!(args[i] == ' ' && args[i + 1] == ' ')) {
            clean_args[j++] = args[i];
        }
        i++;
    }
    clean_args[j] = '\0';

    // Localiser l'ID du curseur
    char *cursor_start = strstr(clean_args, "CURSOR WITH ID = ");
    if (cursor_start) {
        cursor_start += strlen("CURSOR WITH ID = ");
        sscanf(cursor_start, "%63s", cursor_id);
    }

    // Localiser la condition X
    char *x_start = strstr(clean_args, "X ");
    if (x_start) {
        x_start += 2; // Sauter "X "
        sscanf(x_start, "%63[^A{]", x_condition); // Capturer toute la condition jusqu'à '{' ou 'A'
    }

    // Localiser la condition Y
    char *y_start = strstr(clean_args, "Y ");
    if (y_start) {
        y_start += 2; // Sauter "Y "
        sscanf(y_start, "%63[^A{]", y_condition); // Capturer toute la condition jusqu'à '{' ou 'A'
    }

    // Localiser le bloc while
    char *block_start = strchr(clean_args, '{');
    char *block_end = strchr(clean_args, '}');
    if (block_start && block_end && block_end > block_start) {
        strncpy(loop_block, block_start + 1, block_end - block_start - 1);
        loop_block[block_end - block_start - 1] = '\0';
    } else {
        fprintf(stderr, "Erreur : bloc d'instructions mal formaté\n");
        return;
    }


    // Générer la condition Python
    fprintf(python_file, "while ");
    if (strlen(x_condition) > 0 && strlen(y_condition) > 0) {
        // Conditions sur X et Y
        fprintf(python_file, "cursors[%s]['x'] %s and cursors[%s]['y'] %s:\n",
                cursor_id, x_condition, cursor_id, y_condition);
    } else if (strlen(x_condition) > 0) {
        // Condition sur X uniquement
        fprintf(python_file, "cursors[%s]['x'] %s:\n", cursor_id, x_condition);
    } else if (strlen(y_condition) > 0) {
        // Condition sur Y uniquement
        fprintf(python_file, "cursors[%s]['y'] %s:\n", cursor_id, y_condition);
    } else {
        fprintf(stderr, "Erreur : aucune condition valide trouvée\n");
        return;
    }

    // Traiter chaque commande du bloc avec indentation
    char *command_start = loop_block;
    while (*command_start != '\0') {
        // Localiser la fin de la commande (virgule ou fin du bloc)
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        // Extraire la commande
        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        // Supprimer les espaces en début et fin de commande
        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        // Ajouter l'indentation avant d'appeler execute_command
        fprintf(python_file, "    ");
        execute_command(trimmed_command, python_file);

        // Passer à la commande suivante
        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }
}

void handle_break_python(const char *args, FILE *python_file) {
    fprintf(python_file, "break\n");
}

void handle_continue_python(const char *args, FILE *python_file) {
    fprintf(python_file, "continue\n");
}
void handle_end_python(const char *args, FILE *python_file) {
    fprintf(python_file, "pass\n");
}
