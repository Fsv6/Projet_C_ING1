#ifndef BLOC_COMMANDS_MANAGER_H
#define BLOC_COMMANDS_MANAGER_H
#include <stdio.h>

void handle_while_python(const char *args, FILE *python_file);
void handle_set_variable(const char *args, FILE *python_file);
void handle_if_python(const char *args, FILE *python_file);
void handle_for_python(const char *args, FILE *python_file);
void handle_break_python(const char *args, FILE *python_file);
void handle_continue_python(const char *args, FILE *python_file);
void handle_end_python(const char *args, FILE *python_file);

#endif //BLOC_COMMANDS_MANAGER_H
