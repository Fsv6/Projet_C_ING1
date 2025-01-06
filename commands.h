#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

typedef struct {
    const char *name;
    void (*compile)(const char *args, FILE *python_file);
    void (*create)(FILE *python_file);
} Command;

const char** get_command_names(Command commands[], int* count);



void set_python_file(FILE *python_file);
void end_python_file(FILE *python_file);
// Prototypes des fonctions
void create_cursor_func_py(FILE *python_file);
void create_color_func_py(FILE *python_file);
void create_thickness_func_py( FILE *python_file);
void create_move_func_py(FILE *python_file);
void create_rotate_func_py(FILE *python_file);
void create_line_func_py( FILE *python_file);
void create_circle_func_py(FILE *python_file);
void create_square_func_py( FILE *python_file);
void create_rectangle_func_py( FILE *python_file);
void create_point_func_py( FILE *python_file);
void create_arc_func_py( FILE *python_file);
void create_animation_func_py( FILE *python_file);

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


void handle_while_python(const char *args, FILE *python_file);
void handle_set_variable(const char *args, FILE *python_file);
void handle_if_python(const char *args, FILE *python_file);
void handle_for_python(const char *args, FILE *python_file);
void handle_break_python(const char *args, FILE *python_file);
void handle_continue_python(const char *args, FILE *python_file);
void handle_end_python(const char *args, FILE *python_file);

extern Command commands[];

#endif // COMMANDS_H
