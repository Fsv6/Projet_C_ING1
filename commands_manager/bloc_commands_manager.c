
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../commands_parser.h"


/*
IF command: Make a if.
Args:
    args (const char*): The command arguments (Todo).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a if in python.
Returns:
    None
*/
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


/*
FOR command: Make a for.
Args:
    args (const char*): The command arguments (Todo).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a for in python.
Returns:
    None
*/
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


/*
WHILE command: Make a while.
Args:
    args (const char*): The command arguments (Todo).
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a while in python.
Returns:
    None
*/
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


/*
BREAK command: Make a break.
Args:
    args (const char*): The command arguments ().
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a break in python.
Returns:
    None
*/
void handle_break_python(const char *args, FILE *python_file) {
    fprintf(python_file, "break\n");
}


/*
CONTINUE command: Make a continue.
Args:
    args (const char*): The command arguments ().
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a continue in python.
Returns:
    None
*/
void handle_continue_python(const char *args, FILE *python_file) {
    fprintf(python_file, "continue\n");
}


/*
END command: Make a end
Args:
    args (const char*): The command arguments ().
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a end in python.
Returns:
    None
*/
void handle_end_python(const char *args, FILE *python_file) {
    fprintf(python_file, "pass\n");
}


#include "../bloc_commands_manager.h"
