#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variable.h"

#include <ctype.h>
#include <parser.h>

// Taille maximum du tableau de variables
#define MAX_VARIABLES 100

// Liste de variables
static Variable variables[MAX_VARIABLES];
static int variable_count = 0;

void set_variable(const char *name, int value) {
    for (int i = 0; i < variable_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }

    if (variable_count < MAX_VARIABLES) {
        strncpy(variables[variable_count].name, name, sizeof(variables[variable_count].name) - 1);
        variables[variable_count].name[sizeof(variables[variable_count].name) - 1] = '\0';
        variables[variable_count].value = value;
        variable_count++;
    } else {
        fprintf(stderr, "Erreur : limite de variables atteinte.\n");
    }
}

// Fonction pour récupérer la valeur d'une variable
int get_variable(const char *name) {
    for (int i = 0; i < variable_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    fprintf(stderr, "Erreur : variable %s introuvable.\n", name);
    return 0; // Retourne 0 par défaut en cas d'erreur
}


void print_variables() {
    printf("Liste des variables :\n");
    for (int i = 0; i < variable_count; ++i) {
        printf("  %s = %d\n", variables[i].name, variables[i].value);
    }
    printf("\n");
}

void set_variable_in_c(const char *variable_name, const char *expression) {

    char *endptr;
    int value = strtol(expression, &endptr, 10);
    if (*endptr == '\0') {
        set_variable(variable_name, value);
    } else if (isalpha(expression[0]) && strchr(expression, ' ') == NULL && strchr(expression, '+') == NULL &&
               strchr(expression, '-') == NULL && strchr(expression, '*') == NULL && strchr(expression, '/') == NULL) {
        // Expression est une variable simple
        int variable_value = get_variable(expression);
        if (variable_value != 0 || strcmp(expression, "0") == 0) {

            set_variable(variable_name, variable_value);
        }
               } else {
                   value = evaluate_expression(expression);
                   set_variable(variable_name, value);
               }

}



