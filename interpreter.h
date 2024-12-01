
#ifndef INTERPRETER_H
#define INTERPRETER_H

void interpret_draw_file(const char *draw_filename, const char *output_python_filename);
void process_command(const char *command, FILE *python_file);

#endif //INTERPRETER_H
