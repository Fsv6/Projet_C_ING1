
#ifndef PYTHON_FUNCTIONS_WRITER_H
#define PYTHON_FUNCTIONS_WRITER_H
#include <stdio.h>

void set_python_file(FILE *python_file);
void end_python_file(FILE *python_file);

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
void create_goto_func_py( FILE *python_file);

#endif //PYTHON_FUNCTIONS_WRITER_H
