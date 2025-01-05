#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "commands.h"

#include <ctype.h>

#include "cursor_manager.h"
#include "figure_manager.h"
#include "interpreter.h"
#include "parser.h"
#include "variable.h"


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
    {"POINT", call_point_func_py, create_point_func_py},
    {"ARC", call_arc_func_py, create_arc_func_py},
    {"IF",handle_if_python , NULL},
    {"FOR", handle_for_python, NULL},
    //{"SET", handle_set_variable, create_set_variable_func_py},

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
    fprintf(python_file, "if 'cursors' not in globals():\n");
    fprintf(python_file, "    cursors = {}\n");
    fprintf(python_file, "turtle.setup(800, 600)\n");
    fprintf(python_file, "turtle.title('Test cursors')\n\n");
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
    fprintf(python_file, "\n");
    fprintf(python_file, "    turtle.tracer(0)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['x'] = x\n");
    fprintf(python_file, "    cursors[id]['y'] = y\n");
    fprintf(python_file, "    cursors[id]['turtle'].setpos(x, y)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    if  (visible == 'TRUE') :\n");
    fprintf(python_file, "        cursors[id]['turtle'].showturtle()\n");
    fprintf(python_file, "    else :\n");
    fprintf(python_file, "        cursors[id]['turtle'].hideturtle()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    turtle.tracer(1)\n");
    fprintf(python_file, "    turtle.update()\n");
}
//handle_cursor('cursor1', 100, 100, True)

void call_cursor_func_py(const char *args,FILE *python_file) {
    char id[64];
    char visible[64];
    int x, y;

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %d %d %63s", id, &x, &y, visible) != 4) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    // Vérifiez que les valeurs extraites sont valides
    printf("Parsed values - id: %s, x: %d, y: %d, visible: %s\n", id, x, y, visible);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_cursor('%s', %d, %d, '%s')\n\n", id, x, y, visible);
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
    fprintf(python_file, "handle_color('%s', '%s')\n\n", id, color);
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
    fprintf(python_file, "handle_move('%s', %s)\n", id, distance);
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
    int angle;

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %d", id, angle) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, angle : %d", id, angle);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_rotate('%s', %d)\n", id, angle);
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
    fprintf(python_file, "def handle_line(id, distance):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(distance)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
}
void call_line_func_py(const char *args, FILE *python_file) {
    char id[64];
    int distance;

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %d", id, distance) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, distance : %d", id, distance);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_rotate('%s', %d)\n", id, distance);
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
    fprintf(python_file, "def handle_circle(id, radius):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].circle(radius)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
}
void call_circle_func_py(const char *args, FILE *python_file) {
    char id[64];
    int radius;

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %d", id, radius) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, radius : %d", id, radius);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_circle('%s', %d)\n", id, radius);
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
    fprintf(python_file, "def handle_square(id, side_length):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    for _ in range(4):\n");
    fprintf(python_file, "        cursors[id]['turtle'].forward(side_length)\n");
    fprintf(python_file, "        cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
}
void call_square_func_py(const char *args, FILE *python_file) {
    char id[64];
    int side_length;

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %d", id, side_length) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, side_length : %d", id, side_length);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_square('%s', %d)\n", id, side_length);
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
    fprintf(python_file, "def handle_point(id):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].dot()\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
}
void call_point_func_py(const char *args, FILE *python_file) {
    char id[64];

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s", id) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s", id);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_square('%s')\n", id);
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
    fprintf(python_file, "def handle_semi_circle(id, radius):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].circle(radius, 180)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
}
void call_arc_func_py(const char *args, FILE *python_file) {
    char id[64];
    int radius;

    // Extraire les valeurs depuis la chaîne args
    if (sscanf(args, "%63s %d", id, radius) != 2) {
        fprintf(stderr, "Error: Failed to parse args: '%s'\n", args);
        return;
    }

    printf("Parsed values - id: %s, radius : %d", id, radius);

    // faire un appel dans python à la fonction qui gère les curseurs
    fprintf(python_file, "handle_semi_circle('%s', %d)\n", id, radius);
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
    printf("ID du curseur : '%s'\n", cursor_id);
    printf("Condition X : '%s'\n", x_condition);
    printf("Condition Y : '%s'\n", y_condition);
    printf("Bloc IF : '%s'\n", if_block);
    printf("Bloc ELSE : '%s'\n", else_block);

    // Générer la condition Python
    fprintf(python_file, "if ");
    if (strlen(x_condition) > 0 && strlen(y_condition) > 0) {
        // Conditions sur X et Y
        fprintf(python_file, "cursors['%s']['x'] %s and cursors[%s]['y'] %s:\n",
                cursor_id, x_condition, cursor_id, y_condition);
    } else if (strlen(x_condition) > 0) {
        // Condition sur X uniquement
        fprintf(python_file, "cursors['%s']['x'] %s:\n", cursor_id, x_condition);
    } else if (strlen(y_condition) > 0) {
        // Condition sur Y uniquement
        fprintf(python_file, "cursors['%s']['y'] %s:\n", cursor_id, y_condition);
    }


    fprintf(python_file, "    ");
    execute_command(if_block, python_file);

    // Compiler et écrire le bloc ELSE
    fprintf(python_file, "else:\n");
    fprintf(python_file, "    ");
    execute_command(else_block, python_file);
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

    // Vérifier si les éléments nécessaires sont présents
    if (strlen(loop_var) > 0 && strlen(range_expr) > 0 && strlen(loop_block) > 0) {
        // Générer le code Python
        fprintf(python_file, "for %s in range(%s):\n", loop_var, range_expr);
        fprintf(python_file, "    ");
        execute_command(loop_block, python_file);
    } else {
        fprintf(stderr, "Erreur : commande invalide ou mal formatée\n");
    }
}










// void handle_set_variable(const char *command, FILE *python_file) {
//     char variable_name[64];
//     char expression[256];
//
//     // Extraction de la commande
//     int result = sscanf(command, "VARIABLE %63s = %255[^\n]", variable_name, expression);
//     if (result != 2) {
//         fprintf(stderr, "Erreur : commande VARIABLE mal formatée.\n");
//         fprintf(stderr, "Commande reçue : '%s'\n", command);
//         fprintf(stderr, "Résultat sscanf : %d\n", result);
//         return;
//     }
//
//     printf("Variable : '%s', Expression : '%s'\n", variable_name, expression);
//
//     // Gérer les variables en C
//     set_variable_in_c(variable_name, expression);
//
//     // Écrire dans le fichier Python via le module dédié
//     write_variable_to_python(python_file, variable_name, expression);
// }
//



















