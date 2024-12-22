#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "interpreter.h"

void create_file();
void open_file(const char *directory);
void save_file(const char *filename, const char *content);
void change_file(char *current_file);
void modify_file(const char *filepath, const char *filename);
void execute_and_interpret();

int main() {
    int choice;
    char files_directory[256] = "draw_files";

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
                open_file(files_directory);
            break;
            case 3:
            break;
            case 4:
                //change_file(current_file);
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
    } while (choice != 6);

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

    dir = opendir("../draw_files");
    if (dir == NULL) {
        printf("Erreur : Impossible d'ouvrir le repertoire '%s'.\n", directory);
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name == ".draw") {
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
        printf("%s", line);
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

        file = fopen(filepath, "w"); // Write mode (overwrite)
        if (file == NULL) {
            printf("Erreur : Impossible de modifier le fichier %s.\n", filepath);
            return;
        }
        printf("Entrez le nouveau contenu (tapez 'FIN' pour terminer) :\n");
        while (1) {
            printf("> ");
            fgets(line, sizeof(line), stdin);
            line[strcspn(line, "\n")] = '\0';
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
    char draw_filename[256];
    printf("Entrez le nom du fichier .draw a interpreter (sans extension) : ");
    scanf("%s", draw_filename);
    getchar();


    char draw_filepath[512], python_filepath[512];
    snprintf(draw_filepath, sizeof(draw_filepath), "../draw_files/%s.draw", draw_filename);

    snprintf(python_filepath, sizeof(python_filepath), "../py_files_directory/%s.py", draw_filename);

    interpret_draw_file(draw_filepath, python_filepath);

    printf("Le fichier Python correspondant a ete genere : %s\n", python_filepath);
}








