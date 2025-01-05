#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "commands.h"
#include "axes_generator.h"
#include "cursor_manager.h"


// Taille maximale pour une ligne du fichier
#define MAX_LINE_LENGTH 1024
// Nombre maximum de mots-clés dans le dictionnaire
#define MAX_KEYWORDS 100
// Taille maximale pour un mot-clé
#define MAX_KEYWORD_LENGTH 100

void finalize_execution(FILE *draw_file,  FILE *python_file, const char *output_python_filename) {

    end_python_file(python_file);

    fclose(draw_file);
    fclose(python_file);


    free_cursors();


    char command[512];
    snprintf(command, sizeof(command), "py %s", output_python_filename);
    int result = system(command);
    if (result != 0) {
        printf("Error: Unable to execute the Python file '%s'.\n", output_python_filename);
    } else {
        printf("Execution completed successfully.\n");
    }
}


void execute_command(const char *line, FILE *python_file) {
    char command_name[64], args[192];

    if (sscanf(line, "%63s %191[^\n]", command_name, args) < 1) {
        printf("Commande invalide ou vide : '%s'\n", line);
        return;
    }

    printf("Commande : '%s', Arguments : '%s'\n", command_name, args);

    // Parcourir les commandes disponibles pour trouver une correspondance
    for (int j = 0; commands[j].name != NULL; j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            commands[j].compile(args, python_file);
            return;
        }
    }


    printf("Commande inconnue : '%s'\n", command_name);
}


void process_command(const char *draw_filename, FILE *python_file) {
    printf("Processing commands...\n");

    FILE *draw_file = fopen(draw_filename, "r");
    if (draw_file == NULL) {
        printf("Error: Unable to open file %s.\n", draw_filename);
        return;
    }

    char line[256];
    char *buffer = NULL;
    size_t buffer_size = 0;
    int inside_block = 0;

    while (fgets(line, sizeof(line), draw_file)) {
        line[strcspn(line, "\n")] = '\0'; // Supprime le caractère de fin de ligne
        printf("Read line: '%s'\n", line);

        // Détecte le début d'un bloc (commence par IF ou contient `{`)
        if (strchr(line, '{')) {
            if (!inside_block) {
                printf("Detected start of block.\n");
                inside_block = 1;

                // Initialise le tampon
                buffer_size = strlen(line) + 2;
                buffer = malloc(buffer_size);
                if (!buffer) {
                    fprintf(stderr, "Memory allocation error\n");
                    fclose(draw_file);
                    return;
                }
                buffer[0] = '\0'; // Initialise comme chaîne vide
            }

            // Ajoute la ligne au tampon
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);
            strcat(buffer, " ");
            continue;
        }

        // Si une ligne contient à la fois `}` et `{`, le bloc continue
        if (strchr(line, '}') && strchr(line, '{')) {
            printf("Detected continuation of block with '} ... {'.\n");

            // Ajoute la ligne au tampon
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);
            strcat(buffer, " ");
            continue;
        }

        // Détecte la fin d'un bloc (contient uniquement `}`)
        if (strrchr(line, '}')) {
            printf("Detected end of block.\n");

            // Ajoute la ligne au tampon
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);

            // Exécute le bloc complet
            printf("Executing block: '%s'\n", buffer);
            execute_command(buffer, python_file);

            // Nettoie le tampon
            free(buffer);
            buffer = NULL;
            buffer_size = 0;
            inside_block = 0;
            continue;
        }

        // Ajoute au tampon si nous sommes dans un bloc
        if (inside_block) {
            printf("Appending to block: '%s'\n", line);
            buffer_size += strlen(line) + 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(draw_file);
                return;
            }
            strcat(buffer, line);
            strcat(buffer, " ");
        } else {
            // Exécute les commandes hors blocs
            printf("Executing single line command: '%s'\n", line);
            execute_command(line, python_file);
        }
    }

    // Vérifie si un bloc est resté ouvert
    if (buffer) {
        fprintf(stderr, "Warning: Unclosed block detected. Buffer content: '%s'\n", buffer);
        free(buffer);
    }

    fclose(draw_file);
}





bool is_in_list(const char *word, const char **list, int list_size) {
    for (int i = 0; i < list_size; i++) {
        if (strcmp(word, list[i]) == 0) {
            return true;
        }
    }
    return false;
}


// Fonction principale pour lire et interpréter le fichier .draw
void interpret_draw_file(const char *draw_filename, const char *output_python_filename) {
    FILE *draw_file = fopen(draw_filename, "r");
    if (draw_file == NULL) {
        printf("Error: Unable to open file %s.\n", draw_filename);
        return;
    }

    FILE *python_file = fopen(output_python_filename, "w");
    if (python_file == NULL) {
        fclose(draw_file);
        printf("Error: Unable to create file %s.\n", output_python_filename);
        return;
    }

    set_python_file(python_file);

    const char *detected_keywords[MAX_KEYWORDS];
    int detected_count = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), draw_file)) {
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            for (int i = 0; commands[i].name != NULL; i++) {
                if (strcmp(token, commands[i].name) == 0 && !is_in_list(token, detected_keywords, detected_count)) {
                    detected_keywords[detected_count] = strdup(token);
                    detected_count++;
                    break;
                }
            }
            token = strtok(NULL, " \t\n");
        }
    }

    // Appeler les fonctions associées aux mots-clés détectés
    for (int i = 0; i < detected_count; i++) {
        bool handler_found = false; // Pour vérifier si un gestionnaire a été trouvé
        for (int j = 0; commands[j].name != NULL; j++) {
            if (strcmp(detected_keywords[i], commands[j].name) == 0) {
                handler_found = true; // Marque qu'un gestionnaire a été trouvé
                printf("Executing handler for keyword: %s\n", commands[j].name);
                if (commands[j].create != NULL) {
                    commands[j].create(python_file); // Appelle la fonction si elle est définie
                } else {
                    printf("No handler function for keyword: %s\n", commands[j].name);
                }
                break;
            }
        }
        if (!handler_found) {
            printf("No handler found for keyword: %s\n", detected_keywords[i]);
        }
    }

    // Libérer la mémoire allouée
    for (int i = 0; i < detected_count; i++) {
        free((void *)detected_keywords[i]);
    }

    fclose(draw_file);

    process_command(draw_filename, python_file);

    finalize_execution(draw_file, python_file, output_python_filename);

}


