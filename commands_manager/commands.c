#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "commands.h"
#include <ctype.h>
#include "commands_parser.h"
#include "python_functions_writer.h"
#include "bloc_commands_manager.h"


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
    {"RECTANGLE", call_rectangle_func_py, create_rectangle_func_py},
    {"POINT", call_point_func_py, create_point_func_py},
    {"ARC", call_arc_func_py, create_arc_func_py},
    {"ANIME", call_animation_func_py, create_animation_func_py},
    {"GOTO", call_goto_func_py, create_goto_func_py},
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




/*
call CURSOR command: call a new cursor.
Args:
    args (const char*): The command arguments (ID, x, y, visibility).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to create a new turtle cursor in Python.
Returns:
    None
*/
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
call COLOR command: Changes the color of a cursor.
Args:
    args (const char*): The command arguments (ID, color).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to changes the color of a cursor.
Returns:
    None
*/
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
call THICKNESS command: Changes the thickness of the pen for a cursor.
Args:
    args (const char*): The command arguments (ID, thickness).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to Changes the thickness of the pen for a cursor.
Returns:
    None
*/
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
call MOVE command: Move a cursor.
Args:
    args (const char*): The command arguments (ID, distance).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to a cursor in a direction giving the distance.
Returns:
    None
*/
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



/*
call GOTO command: Teleport a cursor.
Args:
    args (const char*): The command arguments (ID, x, y).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to teleport the cursor to a position.
Returns:
    None
*/
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
call ROTATE command: Rotates the cursor by a specified angle.
Args:
    args (const char*): The command arguments (ID, angle).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to rotates a cursor by the given angle in degrees.
Returns:
    None
*/
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
call LINE command: Make a line.
Args:
    args (const char*): The command arguments (ID, distance, ID_form).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to make a line with a given distance.
Returns:
    None
*/
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
call CIRCLE command: Make a circle.
Args:
    args (const char*): The command arguments (ID, radius, ID_form).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to make a circle.
Returns:
    None
*/
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
call SQUARE command: Make a square.
Args:
    args (const char*): The command arguments (ID, side_length, ID_form).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to make a square.
Returns:
    None
*/
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


/*
call RECTANGLE command: Make a rectangle.
Args:
    args (const char*): The command arguments (ID, side_1, side_2, ID_form).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to make a rectangle.
Returns:
    None
*/
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
call POINT command: Make a point.
Args:
    args (const char*): The command arguments (ID, ID_form).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to make a point.
Returns:
    None
*/
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
call ARC command: Make a arc.
Args:
    args (const char*): The command arguments (ID, radius, ID_form).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to make a arc.
Returns:
    None
*/
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


/*
call ANIMATION command: Animate figure.
Args:
    args (const char*): The command arguments (ID, move_distance, move_r, angle_degrees).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create the call of the function to animate figures.
Returns:
    None
*/
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
    fprintf(python_file, "turtle.tracer(n=10, delay=0)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "for i in range(%s):\n", move_r);
    fprintf(python_file, "    time.sleep(0.1)\n");
    fprintf(python_file, "    if (i == %s - 1):\n", move_r);
    fprintf(python_file, "        show = True\n");
    fprintf(python_file, "    else:\n");
    fprintf(python_file, "        show = False\n");
    fprintf(python_file, "    animation(%s, %s, %s, show)\n", ids, move_distance, angle_degrees);
    fprintf(python_file, "\n");
    fprintf(python_file, "turtle.tracer(n=1, delay=10)\n");
    fprintf(python_file, "set_all_cursors_speed(1)\n");
}


