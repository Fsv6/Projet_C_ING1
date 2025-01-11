#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../bloc_commands_manager.h"
#include "../commands_parser.h"

/*
Generates a Python `if` statement with optional `else` block.
This function parses conditions on cursor properties (X and Y),
which must follow the format: `<operator> <value>`, where `<operator>`
is a valid comparison operator (e.g., >, <, ==, !=), and `<value>` is
a numeric value or a variable. The function extracts commands for the `if` and `else`
blocks and writes the corresponding Python code with proper indentation.
Commands inside the blocks are processed using `execute_command`,
ensuring consistent handling as single-line commands.
*/
void handle_if_python(const char *args, FILE *python_file) {
    char clean_args[512] = "";
    char cursor_id[64] = "";
    char x_condition[64] = "";
    char y_condition[64] = "";
    char if_block[256] = "";
    char else_block[256] = "";

    // Clean extra spaces in the argument string
    int i = 0, j = 0;
    while (args[i] != '\0') {
        if (!isspace(args[i]) || (j > 0 && !isspace(clean_args[j - 1]))) {
            clean_args[j++] = args[i];
        }
        i++;
    }
    clean_args[j] = '\0';

    // Extract cursor ID from the arguments
    char *cursor_start = strstr(clean_args, "CURSOR WITH ID = ");
    if (cursor_start) {
        cursor_start += strlen("CURSOR WITH ID = ");
        sscanf(cursor_start, "%63s", cursor_id);
    }

    // Extract X condition
    char *x_start = strstr(clean_args, "X ");
    if (x_start) {
        x_start += 2; // Skip "X "
        sscanf(x_start, "%63[^A{]", x_condition);
    }

    // Extract Y condition
    char *y_start = strstr(clean_args, "Y ");
    if (y_start) {
        y_start += 2; // Skip "Y "
        sscanf(y_start, "%63[^A{]", y_condition);
    }

    // Extract IF and ELSE blocks
    char *if_start = strchr(clean_args, '{');
    char *else_start = strstr(clean_args, "} ELSE {");
    if (if_start && else_start) {
        // Extract IF block
        strncpy(if_block, if_start + 1, else_start - if_start - 1);
        if_block[else_start - if_start - 1] = '\0';

        // Extract ELSE block
        char *else_content_start = else_start + strlen("} ELSE {");
        char *else_content_end = strchr(else_content_start, '}');
        if (else_content_end) {
            strncpy(else_block, else_content_start, else_content_end - else_content_start);
            else_block[else_content_end - else_content_start] = '\0';
        }
    }

    // Generate the Python `if` condition
    fprintf(python_file, "if ");
    if (strlen(x_condition) > 0 && strlen(y_condition) > 0) {
        fprintf(python_file, "cursors[%s]['x'] %s and cursors[%s]['y'] %s:\n",
                cursor_id, x_condition, cursor_id, y_condition);
    } else if (strlen(x_condition) > 0) {
        fprintf(python_file, "cursors[%s]['x'] %s:\n", cursor_id, x_condition);
    } else if (strlen(y_condition) > 0) {
        fprintf(python_file, "cursors[%s]['y'] %s:\n", cursor_id, y_condition);
    }

    // Process commands in the IF block
    char *command_start = if_block;
    while (*command_start != '\0') {
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        // Trim spaces
        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        fprintf(python_file, "    "); // Add indentation
        execute_command(trimmed_command, python_file);

        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }

    // Process commands in the ELSE block
    fprintf(python_file, "else:\n");
    command_start = else_block;
    while (*command_start != '\0') {
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        fprintf(python_file, "    "); // Add indentation
        execute_command(trimmed_command, python_file);

        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }
}

/*
Generates a Python `while` loop.
This function parses conditions on cursor properties (X and Y),
which must follow the format: `<operator> <value>`, where `<operator>`
is a valid comparison operator (e.g., >, <, ==, !=), and `<value>` is
a numeric value or a variable. It extracts commands inside the loop and writes the
corresponding Python code with proper indentation. Commands inside
the loop are processed using `execute_command` for consistency.
*/
void handle_while_python(const char *args, FILE *python_file) {
    char clean_args[1024] = "";
    char cursor_id[64] = "";
    char x_condition[64] = "";
    char y_condition[64] = "";
    char loop_block[512] = "";

    // Clean extra spaces in the argument string
    int i = 0, j = 0;
    while (args[i] != '\0') {
        if (!(args[i] == ' ' && args[i + 1] == ' ')) {
            clean_args[j++] = args[i];
        }
        i++;
    }
    clean_args[j] = '\0';

    // Extract cursor ID
    char *cursor_start = strstr(clean_args, "CURSOR WITH ID = ");
    if (cursor_start) {
        cursor_start += strlen("CURSOR WITH ID = ");
        sscanf(cursor_start, "%63s", cursor_id);
    }

    // Extract X condition
    char *x_start = strstr(clean_args, "X ");
    if (x_start) {
        x_start += 2; // Skip "X "
        sscanf(x_start, "%63[^A{]", x_condition);
    }

    // Extract Y condition
    char *y_start = strstr(clean_args, "Y ");
    if (y_start) {
        y_start += 2; // Skip "Y "
        sscanf(y_start, "%63[^A{]", y_condition);
    }

    // Extract loop block
    char *block_start = strchr(clean_args, '{');
    char *block_end = strchr(clean_args, '}');
    if (block_start && block_end && block_end > block_start) {
        strncpy(loop_block, block_start + 1, block_end - block_start - 1);
        loop_block[block_end - block_start - 1] = '\0';
    } else {
        fprintf(stderr, "Error: Invalid block format\n");
        return;
    }

    // Generate the Python `while` condition
    fprintf(python_file, "while ");
    if (strlen(x_condition) > 0 && strlen(y_condition) > 0) {
        fprintf(python_file, "cursors[%s]['x'] %s and cursors[%s]['y'] %s:\n",
                cursor_id, x_condition, cursor_id, y_condition);
    } else if (strlen(x_condition) > 0) {
        fprintf(python_file, "cursors[%s]['x'] %s:\n", cursor_id, x_condition);
    } else if (strlen(y_condition) > 0) {
        fprintf(python_file, "cursors[%s]['y'] %s:\n", cursor_id, y_condition);
    } else {
        fprintf(stderr, "Error: No valid condition found\n");
        return;
    }

    // Process commands in the loop block
    char *command_start = loop_block;
    while (*command_start != '\0') {
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        // Trim spaces
        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        fprintf(python_file, "    "); // Add indentation
        execute_command(trimmed_command, python_file);

        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }
}

/*
Generates a Python `for` loop.
This function parses the loop variable, range expression, and commands inside the loop.
The range expression must follow Python's `range(start, end, step)` syntax, and any `RANGE` keyword
in the input is converted to valid Python syntax. Commands inside the loop are processed
using `execute_command`, ensuring consistent handling as single-line commands.
*/
void handle_for_python(const char *args, FILE *python_file) {
    char clean_args[512] = "";
    char loop_var[64] = "";
    char range_expr[128] = "";
    char loop_block[256] = "";

    // Clean extra spaces in the argument string
    int i = 0, j = 0;
    while (args[i] != '\0') {
        if (!isspace(args[i]) || (j > 0 && !isspace(clean_args[j - 1]))) {
            clean_args[j++] = args[i];
        }
        i++;
    }
    clean_args[j] = '\0';

    // Detect loop variable and range expression
    char *in_pos = strstr(clean_args, " IN ");
    if (in_pos) {
        strncpy(loop_var, clean_args, in_pos - clean_args);
        loop_var[in_pos - clean_args] = '\0';

        char *block_start = strchr(in_pos, '{');
        if (block_start) {
            strncpy(range_expr, in_pos + 4, block_start - (in_pos + 4));
            range_expr[block_start - (in_pos + 4)] = '\0';

            char *block_end = strrchr(block_start, '}');
            if (block_end) {
                strncpy(loop_block, block_start + 1, block_end - block_start - 1);
                loop_block[block_end - block_start - 1] = '\0';
            }
        }
    }

    fprintf(python_file, "for %s in range(%s):\n", loop_var, range_expr);

    // Process commands in the loop block
    char *command_start = loop_block;
    while (*command_start != '\0') {
        char *command_end = strchr(command_start, ',');
        if (!command_end) command_end = command_start + strlen(command_start);

        char command[256] = "";
        strncpy(command, command_start, command_end - command_start);
        command[command_end - command_start] = '\0';

        char *trimmed_command = command;
        while (*trimmed_command == ' ') trimmed_command++;
        char *end_trim = trimmed_command + strlen(trimmed_command) - 1;
        while (end_trim > trimmed_command && *end_trim == ' ') *end_trim-- = '\0';

        fprintf(python_file, "    ");
        execute_command(trimmed_command, python_file);

        if (*command_end == ',') command_start = command_end + 1;
        else break;
    }
}

/*
Writes a Python `break` statement to exit the current loop.
*/
void handle_break_python(const char *args, FILE *python_file) {
    fprintf(python_file, "break\n");
}

/*
Writes a Python `continue` statement to skip the rest of the current loop iteration.
*/
void handle_continue_python(const char *args, FILE *python_file) {
    fprintf(python_file, "continue\n");
}

/*
Writes a Python `pass` statement as a placeholder for no operation.
*/
void handle_end_python(const char *args, FILE *python_file) {
    fprintf(python_file, "pass\n");
}



