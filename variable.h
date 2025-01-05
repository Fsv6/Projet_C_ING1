#ifndef VARIABLES_H
#define VARIABLES_H

// Structure pour stocker une variable utilisateur
typedef struct {
    char name[64]; // Nom de la variable
    int value;     // Valeur associée
} Variable;

void set_variable(const char *name, int value);
int get_variable(const char *name);
void print_variables();
void set_variable_in_c(const char *variable_name, const char *expression);

#endif // VARIABLES_H

