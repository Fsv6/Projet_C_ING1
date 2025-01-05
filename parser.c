#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

#include <ctype.h>

#include "commands.h"
#include "variable.h"





// Fonction pour ignorer les espaces
static void skip_spaces(const char **expression) {
    while (isspace(**expression)) {
        (*expression)++;
    }
}

// Fonction pour analyser une expression (opérateurs + et -)
static int parse_expression(const char **expression);
static int parse_term(const char **expression);
static int parse_factor(const char **expression);

static int parse_expression(const char **expression) {
    int result = parse_term(expression);
    skip_spaces(expression);


    while (**expression == '+' || **expression == '-') {
        char operator = **expression;
        (*expression)++;
        skip_spaces(expression);

        int value = parse_term(expression);

        if (operator == '+') {
            result += value;
        } else if (operator == '-') {
            result -= value;
        }

        skip_spaces(expression);
    }

    return result;
}


// Fonction pour analyser un terme (opérateurs * et /)
static int parse_term(const char **expression) {
    int result = parse_factor(expression);
    skip_spaces(expression);

    while (**expression == '*' || **expression == '/') {
        char operator = **expression;
        (*expression)++;
        skip_spaces(expression);

        int value = parse_factor(expression);

        if (operator == '*') {
            result *= value;
        } else if (operator == '/') {
            if (value == 0) {
                fprintf(stderr, "Erreur : division par zéro.\n");
                exit(EXIT_FAILURE);
            }
            result /= value;
        }
        skip_spaces(expression);
    }

    return result;
}

static int parse_factor(const char **expression) {
    skip_spaces(expression);

    if (**expression == '(') {
        (*expression)++;
        int result = parse_expression(expression);

        if (**expression != ')') {
            fprintf(stderr, "Erreur : parenthèse fermante manquante.\n");
            exit(EXIT_FAILURE);
        }
        (*expression)++;
        return result;
    }

    if (isdigit(**expression) || (**expression == '-' && isdigit(*(*expression + 1)))) {
        int value = strtol(*expression, (char **)expression, 10);
        return value;
    }

    if (isalpha(**expression)) {
        char variable_name[64];
        int i = 0;

        while (isalnum(**expression) && i < 63) {
            variable_name[i++] = *(*expression)++;
        }
        variable_name[i] = '\0';

        int value = get_variable(variable_name);
        return value;
    }

    fprintf(stderr, "Erreur : facteur invalide.\n");
    exit(EXIT_FAILURE);
}

int evaluate_expression(const char *expression) {
    int result = parse_expression(&expression);
    return result;
}



