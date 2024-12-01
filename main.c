#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "interpreter.h"

void create_file();
void open_file();
void save_file(const char *filename, const char *content);
void change_file(char *current_file);
void modify_file(const char *filepath, const char *filename);
void execute_and_interpret();

int main() {
    int choice;
    char files_directory[256] = "draw_files";
    char current_file[256] = "";


    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Creer un fichier .draw\n");
        printf("2. Ouvrir un fichier .draw\n");
        printf("3. Sauvegarder le fichier courant\n");
        printf("4. Changer de fichier\n");
        printf("5. Executer un fichier\n");
        printf("6. Quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                create_file();
            break;
            case 2:
                open_file();
            break;
            case 3:
                if (strlen(current_file) > 0) {
                    char content[1024];
                    printf("Entrez le contenu à sauvegarder dans %s : ", current_file);
                    fgets(content, sizeof(content), stdin);
                    content[strcspn(content, "\n")] = '\0'; // Supprime le \n
                    save_file(current_file, content);
                } else {
                    printf("Aucun fichier courant. Veuillez d'abord en ouvrir ou en creer un.\n");
                }
            break;
            case 4:
                change_file(current_file);
            break;
            case 5:
                execute_and_interpret(files_directory);
            break;
            case 6:
                printf("Au revoir !\n");
            break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choice != 5);

    return 0;
}


void create_file() {
    char filename[256];
    printf("Entrez le nom du fichier a creer (avec extension .draw) : ");
    scanf("%s", filename);
    getchar(); // Consomme le caractère de nouvelle ligne


    char filepath[512];
    snprintf(filepath, sizeof(filepath), "H:/Documents/C/Projet/draw_files/%s", filename);


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
    printf("Le fichier %s a été sauvegarde dans le repertoire draw_files.\n", filename);
}






void list_files_in_directory(const char *directory) {
    DIR *dir = opendir("H:/Documents/C/Projet/draw_files");
    if (dir == NULL) {
        printf("Erreur : Impossible d'ouvrir le répertoire %s.\n", directory);
        return;
    }

    struct dirent *entry;
    printf("Fichiers disponibles dans %s :\n", directory);
    int index = 1;
    while ((entry = readdir(dir)) != NULL) {

        if (strstr(entry->d_name, ".draw") != NULL) {
            printf("%d. %s\n", index++, entry->d_name);
        }
    }
    closedir(dir);
}



void open_file(const char *directory) {
    char filename[256];
    char filepath[512];


    printf("=== Liste des fichiers disponibles ===\n");
    list_files_in_directory(directory);

//TODO : ne pas demander d'entrer de fichier si la liste n'est pas affiché

    printf("\nEntrez le nom du fichier a ouvrir (avec extension .draw) : ");
    scanf("%s", filename);
    getchar();


    snprintf(filepath, sizeof(filepath), "H:/Documents/C/Projet/draw_files/%s", filename);


    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", filepath);
        return;
    }


    printf("\n=== Contenu du fichier %s ===\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);


    printf("\nVoulez-vous modifier ce fichier ? (o/n) : ");
    char response;
    scanf(" %c", &response);
    getchar(); // Consomme le caractère de nouvelle ligne

    if (response == 'o' || response == 'O') {
        modify_file(filepath, filename);
    } else {
        printf("Aucune modification n'a ete effectuee.\n");
    }
}


void save_file(const char *filename, const char *content) {
    //TODO
}



void modify_file(const char *filepath, const char *filename) {
    printf("\n=== Modification du fichier %s ===\n", filename);
    printf("Options :\n");
    printf("1. Ajouter du contenu\n");
    printf("2. Remplacer tout le contenu\n");
    printf("Votre choix : ");

    int choice;
    scanf("%d", &choice);
    getchar(); // Consomme le caractère de nouvelle ligne

    char line[256];
    FILE *file;

    if (choice == 1) {
        // Ajouter du contenu
        file = fopen(filepath, "a"); // Append mode
        if (file == NULL) {
            printf("Erreur : Impossible de modifier le fichier %s.\n", filepath);
            return;
        }
        printf("Entrez le contenu a ajouter (tapez 'FIN' pour terminer) :\n");
        while (1) {
            printf("> ");
            fgets(line, sizeof(line), stdin);
            line[strcspn(line, "\n")] = '\0'; // Supprime le \n
            if (strcmp(line, "FIN") == 0) {
                break;
            }
            fprintf(file, "%s\n", line);
        }
        fclose(file);
    } else if (choice == 2) {
        // Remplacer tout le contenu
        file = fopen(filepath, "w"); // Write mode (overwrite)
        if (file == NULL) {
            printf("Erreur : Impossible de modifier le fichier %s.\n", filepath);
            return;
        }
        printf("Entrez le nouveau contenu (tapez 'FIN' pour terminer) :\n");
        while (1) {
            printf("> ");
            fgets(line, sizeof(line), stdin);
            line[strcspn(line, "\n")] = '\0'; // Supprime le \n
            if (strcmp(line, "FIN") == 0) {
                break;
            }
            fprintf(file, "%s\n", line);
        }
        fclose(file);
    } else {
        printf("Option invalide. Retour au menu principal.\n");
    }

    printf("Les modifications ont ete sauvegardees dans le fichier %s.\n", filename);
}


void change_file(char *current_file) {

    //TODO

}

void execute_and_interpret() {
    char draw_filename[256], python_filename[256];


    printf("Entrez le nom du fichier .draw a interpreter : ");
    scanf("%s", draw_filename);
    getchar();

    printf("Entrez le nom du fichier Python a generer (avec extension .py) : ");
    scanf("%s", python_filename);
    getchar();

    // Construire les chemins complets
    char draw_filepath[512], python_filepath[512];
    snprintf(draw_filepath, sizeof(draw_filepath), "H:/Documents/C/Projet/draw_files/%s", draw_filename);
    snprintf(python_filepath, sizeof(python_filepath), "H:/Documents/C/Projet/py_files_directory/%s",  python_filename);

    // Appeler l'interpréteur
    interpret_draw_file(draw_filepath, python_filepath);
}







