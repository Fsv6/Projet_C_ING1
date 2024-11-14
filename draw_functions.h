// draw_functions.h
#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <stdio.h>

void generate_circle(FILE *py_file, int x, int y, int radius);
void initialize_figure(FILE *py_file);
void finalize_figure(FILE *py_file, int width, int height);

#endif

