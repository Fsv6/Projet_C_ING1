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

    {NULL, NULL, NULL} // End of table
};

// Retrieves the names of available commands
const char** get_command_names(Command commands[], int* count) {
    int command_count = 0;

    for (int i = 0; commands[i].name != NULL; i++) {
        command_count++;
    }
    *count = command_count;

    const char** names = (const char**)malloc(command_count * sizeof(const char*));
    for (int i = 0; i < command_count; i++) {
        names[i] = commands[i].name;
    }
    return names;
}

/*
Handles the "CURSOR" command.
Parses the arguments to extract cursor details (ID, x, y, visible) and writes a call
to the Python function for handling cursors.
*/
void call_cursor_func_py(const char *args, FILE *python_file) {
    char id[64], visible[64], x[64], y[64];

    if (sscanf(args, "%63s %63s %63s %63s", id, x, y, visible) != 4) {
        fprintf(stderr, "Error: Failed to parse CURSOR command arguments: '%s'\n", args);
        return;
    }

    fprintf(python_file, "handle_cursor(%s, %s, %s, '%s')\n", id, x, y, visible);
}

/*
Handles the "COLOR" command.
Parses the arguments to extract the cursor ID and new color, then writes a call
to the Python function for changing the cursor's color.
*/
void call_color_func_py(const char *args, FILE *python_file) {
    char id[64], color[64];

    if (sscanf(args, "%63s %63s", id, color) != 2) {
        fprintf(stderr, "Error: Failed to parse COLOR command arguments: '%s'\n", args);
        return;
    }

    fprintf(python_file, "handle_color(%s, '%s')\n", id, color);
}

/*
Handles the "THICKNESS" command.
Parses the arguments to extract the cursor ID and new pen thickness, then writes
a call to the Python function for updating the cursor's pen size.
*/
void call_thickness_func_py(const char *args, FILE *python_file) {
    char id[64];
    int thickness;

    if (sscanf(args, "%63s %d", id, &thickness) != 2) {
        fprintf(stderr, "Error: Failed to parse THICKNESS command arguments: '%s'\n", args);
        return;
    }

    fprintf(python_file, "handle_thickness(%s, %d)\n", id, thickness);
}

/*
Handles the "SET" command.
Parses the arguments to define or update a Python variable. Validates the variable name
and expression before writing the assignment to the Python script.
*/
void handle_set_variable(const char *args, FILE *python_file) {

    char variable_name[64], expression[256];

    int result = sscanf(args, "VARIABLE %63s = %255[^\n]", variable_name, expression);
    if (result != 2) {
        fprintf(stderr, "Error: Invalid SET command format: '%s'\n", args);
        return;
    }

    for (char *c = variable_name; *c; ++c) {
        if (!((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c == '_') || (*c >= '0' && *c <= '9' && c != variable_name))) {
            fprintf(stderr, "Error: Invalid variable name '%s'.\n", variable_name);
            return;
        }
    }

    if (strlen(expression) == 0) {
        fprintf(stderr, "Error: Empty expression for variable '%s'.\n", variable_name);
        return;
    }

    fprintf(python_file, "%s = %s\n", variable_name, expression);
}


/*
Handles the `MOVE` command.
Parses the arguments to extract the cursor ID and distance to move.
Writes a call to the corresponding Python function to move the cursor.
*/
void call_move_func_py(const char *args, FILE *python_file) {
    char id[64];
    char distance[100];

    // Extract values from the argument string
    if (sscanf(args, "%63s %100s", id, distance) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the move command
    fprintf(python_file, "handle_move(%s, %s)\n", id, distance);
}

/*
Handles the `GOTO` command.
Parses the arguments to extract the cursor ID and the target coordinates (x, y).
Writes a call to the Python function that teleports the cursor to the specified position.
*/
void call_goto_func_py(const char *args, FILE *python_file) {
    char id[64];
    char x[100];
    char y[100];

    // Extract values from the argument string
    if (sscanf(args, "%63s %100s %100s", id, x, y) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the goto command
    fprintf(python_file, "handle_goto(%s, %s, %s)\n", id, x, y);
}

/*
Handles the `ROTATE` command.
Parses the arguments to extract the cursor ID and rotation angle.
Writes a call to the Python function that rotates the cursor by the given angle.
*/
void call_rotate_func_py(const char *args, FILE *python_file) {
    char id[64];
    char angle[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s", id, angle) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the rotate command
    fprintf(python_file, "handle_rotate(%s, %s)\n", id, angle);
}

/*
Handles the `LINE` command.
Parses the arguments to extract the cursor ID, the distance, and the line's form ID.
Writes a call to the Python function that draws a line with the specified properties.
*/
void call_line_func_py(const char *args, FILE *python_file) {
    char id[64];
    char distance[64];
    char id_form[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s %63s", id, distance, id_form) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the line command
    fprintf(python_file, "handle_line(%s, 'null', %s, %s)\n", id, distance, id_form);
}

/*
Handles the `CIRCLE` command.
Parses the arguments to extract the cursor ID, radius, and form ID.
Writes a call to the Python function that draws a circle with the specified properties.
*/
void call_circle_func_py(const char *args, FILE *python_file) {
    char id[64];
    char radius[64];
    char id_form[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s %63s", id, radius, id_form) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the circle command
    fprintf(python_file, "handle_circle(%s, 'null', %s, %s)\n", id, radius, id_form);
}

/*
Handles the `SQUARE` command.
Parses the arguments to extract the cursor ID, side length, and form ID.
Writes a call to the Python function that draws a square with the specified properties.
*/
void call_square_func_py(const char *args, FILE *python_file) {
    char id[64];
    char side_length[64];
    char id_form[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s %63s", id, side_length, id_form) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the square command
    fprintf(python_file, "handle_square(%s, 'null', %s, %s)\n", id, side_length, id_form);
}

/*
Handles the `RECTANGLE` command.
Parses the arguments to extract the cursor ID, side lengths, and form ID.
Writes a call to the Python function that draws a rectangle with the specified properties.
*/
void call_rectangle_func_py(const char *args, FILE *python_file) {
    char id[64];
    char side_1[64];
    char side_2[64];
    char id_form[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s %63s %63s", id, side_1, side_2, id_form) != 4) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the rectangle command
    fprintf(python_file, "handle_rectangle(%s, 'null', %s, %s, %s)\n", id, side_1, side_2, id_form);
}

/*
Handles the `POINT` command.
Parses the arguments to extract the cursor ID and form ID.
Writes a call to the Python function that draws a point at the current cursor position.
*/
void call_point_func_py(const char *args, FILE *python_file) {
    char id[64];
    char id_form[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s", id, id_form) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the point command
    fprintf(python_file, "handle_point(%s, 'null', %s)\n", id, id_form);
}

/*
Handles the `ARC` command.
Parses the arguments to extract the cursor ID, radius, and form ID.
Writes a call to the Python function that draws an arc with the specified properties.
*/
void call_arc_func_py(const char *args, FILE *python_file) {
    char id[64];
    char radius[64];
    char id_form[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s %63s", id, radius, id_form) != 3) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write a Python call to the function handling the arc command
    fprintf(python_file, "handle_semi_circle(%s, 'null', %s, %s)\n", id, radius, id_form);
}

/*
Handles the `ANIMATION` command.
Parses the arguments to extract the figure IDs, movement distance, rotation steps, and rotation angle.
Writes Python code to animate the specified figures over the given number of steps.
*/
void call_animation_func_py(const char *args, FILE *python_file) {
    char ids[64];
    char move_distance[64];
    char move_r[64];
    char angle_degrees[64];

    // Extract values from the argument string
    if (sscanf(args, "%63s %63s %63s %63s", ids, move_distance, move_r, angle_degrees) != 4) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Write the Python code for animating the figures
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
