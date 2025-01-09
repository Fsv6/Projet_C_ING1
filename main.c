#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "commands_parser.h"
#include "commands.h"

void create_file();
void open_file(const char *directory);
void modify_file(const char *filepath, const char *filename);
void compile();
void display_colored_line(const char* line);

int main() {

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Chemin courant : %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    int choice;
    char files_directory[256] = "draw_files";

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Creer un fichier .draw\n");
        printf("2. Ouvrir un fichier .draw\n");
        printf("3. Compiler un fichier\n");
        printf("4. Quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choice);
        getchar();

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
                printf("Au revoir !\n");
            break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choice != 4);

    return 0;
}


void create_file() {
    char filename[256];
    printf("Entrez le nom du fichier a creer (sans extension) : ");
    scanf("%s", filename);
    getchar();


    char filepath[512];
    snprintf(filepath, sizeof(filepath), "../draw_files/%s.draw", filename);


    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Erreur : Impossible de creer le fichier %s.\n", filepath);
        return;
    }

    printf("Le fichier %s a ete cree.\n", filepath);


    char command[256];
    printf("Entrez vos commandes (tapez 'FIN' pour terminer) :\n");
    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "FIN") == 0) {
            break;
        }
        fprintf(file, "%s\n", command);
    }

    fclose(file);
    printf("Le fichier %s a ete sauvegarde dans le repertoire draw_files.\n", filename);
}

int list_files_in_directory(const char *directory) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(directory); // Utilisez le paramètre passé en argument
    if (dir == NULL) {
        printf("Erreur : Impossible d'ouvrir le répertoire '%s'.\n", directory);
        return 0;
    }

    printf("Fichiers dans le repertoire '%s' :\n", directory);
    while ((entry = readdir(dir)) != NULL) {
        // Vérifie si le fichier a l'extension ".draw"
        if (strstr(entry->d_name, ".draw") != NULL) {
            printf("- %s\n", entry->d_name);
            count++;
        }
    }

    closedir(dir);
    return count;
}


void open_file(const char *directory) {
    char filename[256];
    char filepath[512];


    printf("=== Liste des fichiers disponibles ===\n");
    int file_count = list_files_in_directory(directory);

    if (file_count == 0) {
        printf("Aucun fichier disponible dans le repertoire '%s'.\n", directory);
        return;
    }

    printf("\nEntrez le nom du fichier a ouvrir (sans extension) : ");
    scanf("%s", filename);
    getchar();


    snprintf(filepath, sizeof(filepath), "../draw_files/%s.draw", filename);


    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", filepath);
        return;
    }


    printf("\n=== Contenu du fichier %s ===\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        display_colored_line(line); // Utilise la fonction pour afficher les lignes colorées
    }
    fclose(file);


    printf("\nVoulez-vous modifier ce fichier ? (o/n) : ");
    char response;
    scanf(" %c", &response);
    getchar();

    if (response == 'o' || response == 'O') {
        modify_file(filepath, filename);
    } else {
        printf("Aucune modification n'a ete effectuee.\n");
    }
}


void modify_file(const char *filepath, const char *filename) {
    int choice;
    char line[256];
    FILE *file;

    do {
        printf("\n=== Modification du fichier %s ===\n", filename);
        printf("Options :\n");
        printf("1. Ajouter du contenu\n");
        printf("2. Remplacer tout le contenu\n");
        printf("Votre choix : ");
        scanf("%d", &choice);
        getchar(); // Consomme le caractère de nouvelle ligne

        if (choice == 1) {
            // Ajouter du contenu
            file = fopen(filepath, "a"); // Mode ajout
            if (file == NULL) {
                printf("Erreur : Impossible de modifier le fichier %s.\n", filepath);
                return;
            }
            printf("Entrez le contenu à ajouter (tapez 'FIN' pour terminer) :\n");
            while (1) {
                printf("> ");
                fgets(line, sizeof(line), stdin);
                line[strcspn(line, "\n")] = '\0'; // Supprime le retour à la ligne
                if (strcmp(line, "FIN") == 0) {
                    break;
                }
                fprintf(file, "%s\n", line);
            }
            fclose(file);

        } else if (choice == 2) {
            // Remplacer tout le contenu
            file = fopen(filepath, "w"); // Mode écrasement
            if (file == NULL) {
                printf("Erreur : Impossible de modifier le fichier %s.\n", filepath);
                return;
            }
            printf("Entrez le nouveau contenu (tapez 'FIN' pour terminer) :\n");
            while (1) {
                printf("> ");
                fgets(line, sizeof(line), stdin);
                line[strcspn(line, "\n")] = '\0'; // Supprime le retour à la ligne
                if (strcmp(line, "FIN") == 0) {
                    break;
                }
                fprintf(file, "%s\n", line);
            }
            fclose(file);

        } else {
            printf("Option invalide. Veuillez réessayer.\n");
        }

        // Réafficher le contenu du fichier
        printf("\n=== Contenu actuel du fichier %s ===\n", filename);
        file = fopen(filepath, "r");
        if (file == NULL) {
            printf("Erreur : Impossible de lire le fichier %s.\n", filepath);
            return;
        }
        while (fgets(line, sizeof(line), file)) {
            display_colored_line(line); // Utilise votre fonction pour afficher le contenu coloré
        }
        fclose(file);

        // Demander à l'utilisateur s'il souhaite valider ou continuer
        char response;
        printf("\nÊtes-vous satisfait de cette modification ? (o pour valider, n pour continuer) : ");
        scanf(" %c", &response);
        getchar(); // Consomme le caractère de nouvelle ligne

        if (response == 'o' || response == 'O') {
            printf("Modifications validées.\n");
            return;
        }

        printf("Reprenons la modification...\n");

    } while (1); // Boucle jusqu'à validation
}





void compile() {
    char draw_filename[256];
    printf("Entrez le nom du fichier .draw a interpreter (sans extension) : ");
    scanf("%s", draw_filename);
    getchar();


    char draw_filepath[512], python_filepath[512];
    snprintf(draw_filepath, sizeof(draw_filepath), "../draw_files/%s.draw", draw_filename);

    snprintf(python_filepath, sizeof(python_filepath), "../py_files_directory/%s.py", draw_filename);

    draw_handler(draw_filepath, python_filepath);


    printf("Le fichier Python correspondant a ete genere : %s\n", python_filepath);
}


void display_colored_line(const char* line) {
    // Liste des mots-clés supplémentaires à ajouter manuellement
    const char* additional_keywords[] = {
        "WITH", "IS", "IN", "POSITION", "TRUE", "FALSE", "RANGE"
    };
    int num_additional_keywords = sizeof(additional_keywords) / sizeof(additional_keywords[0]);

    char temp[256];
    strcpy(temp, line);

    int i = 0;
    while (temp[i] != '\0') {
        // Vérifier les accolades ouvrantes et fermantes
        if (temp[i] == '{') {
            printf("\033[1;34m{\033[0m "); // Bleu pour l'accolade ouvrante
            i++;
            continue;
        }
        if (temp[i] == '}') {
            printf("\033[1;34m}\033[0m "); // Bleu pour l'accolade fermante
            i++;
            continue;
        }

        // Construire un mot (token)
        char token[256] = {0};
        int j = 0;
        while (temp[i] != '\0' && temp[i] != ' ' && temp[i] != '\t' && temp[i] != '{' && temp[i] != '}') {
            token[j++] = temp[i++];
        }
        token[j] = '\0';

        // Nettoyer le token
        token[strcspn(token, "\n")] = '\0'; // Supprime les retours à la ligne

        if (strlen(token) > 0) {
            int is_keyword = 0;

            // Vérifier si le mot est un mot-clé dynamique
            for (int k = 0; commands[k].name != NULL; k++) {
                if (strcmp(token, commands[k].name) == 0) {
                    printf("\033[1;31m%s\033[0m ", token); // Rouge pour les mots-clés dynamiques
                    is_keyword = 1;
                    break;
                }
            }

            // Vérifier si le mot est un mot-clé additionnel
            if (!is_keyword) {
                for (int k = 0; k < num_additional_keywords; k++) {
                    if (strcmp(token, additional_keywords[k]) == 0) {
                        printf("\033[1;31m%s\033[0m ", token); // Rouge pour les mots-clés additionnels
                        is_keyword = 1;
                        break;
                    }
                }
            }

            // Afficher les autres éléments en vert
            if (!is_keyword) {
                printf("\033[1;32m%s\033[0m ", token); // Vert pour les autres mots
            }
        }

        // Passer à l'espace ou caractère suivant
        if (temp[i] == ' ' || temp[i] == '\t') {
            printf(" "); // Afficher l'espace
            i++;
        }
    }

    printf("\n");
}








