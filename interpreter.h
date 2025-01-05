
#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <stdbool.h>

void process_command(FILE *draw_file, FILE *python_file);
void interpret_draw_file(const char *draw_filename, const char *output_python_filename);
bool is_in_list(const char *word, const char **list, int list_size);
void execute_command(const char *line, FILE *python_file);
void finalize_execution(FILE *draw_file,  FILE *python_file, const char *output_python_filename);




#endif //INTERPRETER_H
