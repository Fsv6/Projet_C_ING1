#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "commands_parser.h"
#include "commands.h"

// Structure defining a command's description
// Contains the command name, its description, and its parameters
typedef struct {
    const char *name;       // Command name (e.g., CURSOR, MOVE, etc.)
    const char *description; // Description of what the command does
    char parameters[256];   // Parameters required for the command
} Command_description;

// Array containing descriptions of all available commands
Command_description commands_description[] = {
    {"CURSOR", "Create a cursor at a given position.", "id (string), x (int), y (int), visible (bool)"},
    {"COLOR", "Change the color of a cursor.", "id (string), color (string)"},
    {"THICKNESS", "Change the line thickness for a cursor.", "id (string), thickness (float)"},
    {"MOVE", "Move a cursor forward by a certain distance.", "id (string), distance (float)"},
    {"GOTO", "Move a cursor to specific coordinates.", "id (string), x (float), y (float)"},
    {"ROTATE", "Rotate a cursor by a given angle.", "id (string), angle (float)"},
    {"LINE", "Draw a line forward from the actual cursor position to a certain point according to the distance given.", "id (string), distance (float), id_form (string)"},
    {"CIRCLE", "Draw a circle.", "id (string), radius (float), id_form (string)"},
    {"SQUARE", "Draw a square.", "id (string), side_length (float), id_form (string)"},
    {"RECTANGLE", "Draw a rectangle.", "id (string), side1_length (float), side2_length (float), id_form (string)"},
    {"POINT", "Draw a point at the current cursor position.", "id (string), id_form (string)"},
    {"ARC", "Draw an arc.", "id (string), radius (float), id_form (string)"},
    {"ANIME", "Animate a figure's movement : move it in a certain direction according to a given angle.", "ids (string), move_distance (float), angle (float)"},
    {"SET", "Assign a value to a variable.",
      "Format: SET VARIABLE variable_name = expression, where:\n"
      "- variable_name: a valid identifier (letters, digits, '_')\n"
      "- expression: any valid numerical or logical expression."},
    {"IF", "Conditional statement: executes code if a condition is true.",
     "Format: IF CURSOR WITH ID = id IS IN POSITION X op value AND/OR Y op value {...} ELSE {...}, where:\n"
     "- op: comparison operator (e.g., >, <, ==, !=)\n"
     "- value: a numeric value or variable\n"
     "Commands inside the block must be separated by commas."},
    {"FOR", "FOR loop: repeats an action a defined number of times.",
     "Format: FOR variable IN RANGE(start, end, step) {...}, where:\n"
     "- variable: loop variable (e.g., i)\n"
     "- start, end, step: integers defining the loop range\n"
     "Commands inside the block must be separated by commas."},
    {"WHILE", "WHILE loop: repeats an action while a condition is true.",
     "Format: WHILE CURSOR WITH ID = id HAS X op value AND/OR Y op value {...}, where:\n"
     "- op: comparison operator (e.g., >, <, ==, !=)\n"
     "- value: a numeric value or variable\n"
     "Commands inside the block must be separated by commas."},
    {"BREAK", "Break out of the current loop.", "None"},
    {"CONTINUE", "Skip the rest of the current iteration in a loop.", "None"},
    {"PASS", "No operation placeholder.", "None"},
    {"END", "Ends the command input.", "None"},
    {"help", "Displays the list of available commands and their descriptions.", "None"}
};



const int commands_count = sizeof(commands_description) / sizeof(commands_description[0]);

// Function prototypes
void create_file();
void open_file(const char *directory);
void modify_file(const char *filepath, const char *filename);
void compile();
void display_help();
int detect_help_or_end(const char* input);
int list_files_in_directory(const char *directory);
void display_colored_line(const char* line);
void detailed_help(const char *command_name);

int main() {
    char input[256];
    char files_directory[256] = "draw_files";

    do {
        // Main menu prompt
        printf("\n=== MAIN MENU ===\n");
        printf("1. Create a .draw file\n");
        printf("2. Open a .draw file\n");
        printf("3. Compile a file\n");
        printf("4. Quit\n");
        printf("Enter a number for your choice or type 'help' for assistance: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strncmp(input, "help", 4) == 0) {
            const char *command_name = input + 5;
            while (*command_name == ' ') command_name++;
            if (*command_name != '\0') {
                detailed_help(command_name);
            } else {
                display_help();
            }
            continue;
        }

        int choice = atoi(input);

        switch (choice) {
            case 1:
                create_file();
            break;
            case 2:
                open_file(files_directory);
            break;
            case 3:
                compile(files_directory);
            break;
            case 4:
                printf("Goodbye!\n");
            break;
            default:
                printf("Invalid choice, please try again or type 'help' for assistance.\n");
        }
    } while (strcmp(input, "4") != 0);

    return 0;
}

// Displays a list of all available commands and their descriptions in the console when the user types "help"
void display_help() {
    printf("\n=== HELP: AVAILABLE COMMANDS ===\n");
    for (int i = 0; i < commands_count; i++) {
        printf("- %s : %s\n", commands_description[i].name, commands_description[i].description);
    }
    printf("\n");
}

// Displays detailed information about a specific command when the user types "help [command]"
void detailed_help(const char *command_name) {
    if (command_name == NULL || strlen(command_name) == 0) {
        printf("Error: Please provide a command after 'help'.\n");
        return;
    }

    for (int i = 0; i < commands_count; i++) {
        if (strcmp(commands_description[i].name, command_name) == 0) {
            printf("\n=== DETAILED HELP ===\n");
            printf("- Command: %s\n", commands_description[i].name);
            printf("- Description: %s\n", commands_description[i].description);
            printf("- Parameters: %s\n", commands_description[i].parameters);
            printf("\n");
            return;
        }
    }
    printf("Error: The command '%s' does not exist.\n", command_name);
}

// Detects if input is "help" or "END", and handles the appropriate response (display help or exit)
int detect_help_or_end(const char* input) {
    if (strncmp(input, "help", 4) == 0) {
        if (strlen(input) > 5) {
            const char *command_name = input + 5;
            while (*command_name == ' ') command_name++;
            detailed_help(command_name);
        } else {
            display_help();
        }
        return 1;
    }
    if (strcmp(input, "END") == 0) {
        return 2;
    }
    return 0;
}

// Creates a new .draw file and accepts user input for commands, until the user types "END"
void create_file() {
    char filename[256];
    printf("Enter the name of the file to create (without extension): ");
    scanf("%s", filename);
    getchar();

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "./draw_files/%s.draw", filename);

    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error: Unable to create the file %s.\n", filepath);
        return;
    }

    printf("The file %s has been created.\n", filepath);

    char command[256];
    printf("Enter your commands (type 'END' to finish, or 'help' for assistance):\n");
    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        int result = detect_help_or_end(command);
        if (result == 1) continue;
        if (result == 2) break;

        fprintf(file, "%s\n", command);
    }

    fclose(file);
    printf("The file %s has been saved in the draw_files directory.\n", filename);
}

// Lists all .draw files in a directory
int list_files_in_directory(const char *directory) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(directory);
    if (dir == NULL) {
        printf("Error: Unable to open directory '%s'.\n", directory);
        return 0;
    }

    printf("Files in directory '%s':\n", directory);
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".draw") != NULL) {
            printf("- %s\n", entry->d_name);
            count++;
        }
    }

    closedir(dir);
    return count;
}

// Opens a specified .draw file, displays its content, and allows modification if wanted
void open_file(const char *directory) {
    char filename[256];
    char filepath[512];

    printf("=== List of available files ===\n");
    int file_count = list_files_in_directory(directory);

    if (file_count == 0) {
        printf("No files available in directory '%s'.\n", directory);
        return;
    }

    printf("\nEnter the name of the file to open (without extension): ");
    scanf("%s", filename);
    getchar();

    snprintf(filepath, sizeof(filepath), "./draw_files/%s.draw", filename);

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s.\n", filepath);
        return;
    }

    printf("\n=== Content of file %s ===\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        display_colored_line(line);
    }
    fclose(file);

    printf("\nWould you like to modify this file? (y/n): ");
    char response;
    scanf(" %c", &response);
    getchar();

    if (response == 'y' || response == 'Y') {
        modify_file(filepath, filename);
    } else {
        printf("No modifications were made.\n");
    }
}

// Modifies the content of an existing .draw file. It is possible to add or replace content
void modify_file(const char *filepath, const char *filename) {
    int choice;
    char line[256];
    FILE *file;

    do {
        printf("\n=== Modifying file %s ===\n", filename);
        printf("Options:\n");
        printf("1. Append content\n");
        printf("2. Replace all content\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            file = fopen(filepath, "a");
            if (file == NULL) {
                printf("Error: Unable to modify file %s.\n", filepath);
                return;
            }
            printf("Enter the content to append (type 'END' to finish, or 'help' for assistance):\n");
            while (1) {
                printf("> ");
                fgets(line, sizeof(line), stdin);
                line[strcspn(line, "\n")] = '\0';

                int result = detect_help_or_end(line);
                if (result == 1) continue;
                if (result == 2) break;

                fprintf(file, "%s\n", line);
            }
            fclose(file);

        } else if (choice == 2) {
            file = fopen(filepath, "w");
            if (file == NULL) {
                printf("Error: Unable to modify file %s.\n", filepath);
                return;
            }
            printf("Enter the new content (type 'END' to finish, or 'help' for assistance):\n");
            while (1) {
                printf("> ");
                fgets(line, sizeof(line), stdin);
                line[strcspn(line, "\n")] = '\0';

                int result = detect_help_or_end(line);
                if (result == 1) continue;
                if (result == 2) break;

                fprintf(file, "%s\n", line);
            }
            fclose(file);

        } else {
            printf("Invalid option. Please try again.\n");
        }

        printf("\n=== Current content of file %s ===\n", filename);
        file = fopen(filepath, "r");
        if (file == NULL) {
            printf("Error: Unable to read file %s.\n", filepath);
            return;
        }
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);

        char response;
        printf("\nAre you satisfied with this modification? (y to confirm, n to continue): ");
        scanf(" %c", &response);
        getchar();

        if (response == 'y' || response == 'Y') {
            printf("Modifications confirmed.\n");
            return;
        }

        printf("Resuming modification...\n");

    } while (1);
}

// Compiles a .draw file to traduce it into a Python file
void compile() {
    char draw_filename[256];
    printf("Enter the name of the .draw file to interpret (without extension): ");
    scanf("%s", draw_filename);
    getchar();

    char draw_filepath[512], python_filepath[512];
    snprintf(draw_filepath, sizeof(draw_filepath), "./draw_files/%s.draw", draw_filename);

    snprintf(python_filepath, sizeof(python_filepath), "./py_files_directory/%s.py", draw_filename);

    draw_handler(draw_filepath, python_filepath);

    printf("The corresponding Python file has been generated: %s\n", python_filepath);
}

// Displays a line with color-coded syntax highlighting
void display_colored_line(const char* line) {
    const char* additional_keywords[] = {
        "WITH", "IS", "IN", "POSITION", "TRUE", "FALSE", "RANGE"
    };
    int num_additional_keywords = sizeof(additional_keywords) / sizeof(additional_keywords[0]);

    char temp[256];
    strcpy(temp, line);

    int i = 0;
    while (temp[i] != '\0') {
        if (temp[i] == '{') {
            printf("\033[1;34m{\033[0m ");
            i++;
            continue;
        }
        if (temp[i] == '}') {
            printf("\033[1;34m}\033[0m ");
            i++;
            continue;
        }

        char token[256] = {0};
        int j = 0;
        while (temp[i] != '\0' && temp[i] != ' ' && temp[i] != '\t' && temp[i] != '{' && temp[i] != '}') {
            token[j++] = temp[i++];
        }
        token[j] = '\0';

        token[strcspn(token, "\n")] = '\0';

        if (strlen(token) > 0) {
            int is_keyword = 0;

            for (int k = 0; commands[k].name != NULL; k++) {
                if (strcmp(token, commands[k].name) == 0) {
                    printf("\033[1;31m%s\033[0m ", token);
                    is_keyword = 1;
                    break;
                }
            }

            if (!is_keyword) {
                for (int k = 0; k < num_additional_keywords; k++) {
                    if (strcmp(token, additional_keywords[k]) == 0) {
                        printf("\033[1;31m%s\033[0m ", token);
                        is_keyword = 1;
                        break;
                    }
                }
            }

            if (!is_keyword) {
                printf("\033[1;32m%s\033[0m ", token);
            }
        }

        if (temp[i] == ' ' || temp[i] == '\t') {
            printf(" ");
            i++;
        }
    }

    printf("\n");
}
