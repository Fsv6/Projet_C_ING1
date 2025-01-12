#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

typedef struct {
    const char *name;
    void (*compile)(const char *args, FILE *python_file);
    void (*create)(FILE *python_file);
} Command;

const char** get_command_names(Command commands[], int* count);





void call_cursor_func_py(const char *args, FILE *python_file);
void call_color_func_py(const char *args, FILE *python_file);
void call_move_func_py(const char *args, FILE *python_file);
void call_rotate_func_py(const char *args, FILE *python_file);
void call_line_func_py(const char *args, FILE *python_file);
void call_circle_func_py(const char *args, FILE *python_file);
void call_square_func_py(const char *args, FILE *python_file);
void call_rectangle_func_py(const char *args, FILE *python_file);
void call_point_func_py(const char *args, FILE *python_file);
void call_arc_func_py(const char *args, FILE *python_file);
void call_thickness_func_py(const char *args, FILE *python_file);
void call_animation_func_py(const char *args, FILE *python_file);
void call_goto_func_py(const char *args, FILE *python_file);

void handle_set_variable(const char *args, FILE *python_file);



extern Command commands[];

#endif // COMMANDS_H
