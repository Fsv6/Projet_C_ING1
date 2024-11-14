// draw_functions.c
#include "draw_functions.h"
#include <stdio.h>

void generate_circle(FILE *py_file, int x, int y, int radius) {
    fprintf(py_file, "circle = plt.Circle((%d, %d), %d, color='blue', fill=False)\n", x, y, radius);
    fprintf(py_file, "ax.add_patch(circle)\n");
}

void initialize_figure(FILE *py_file) {
    fprintf(py_file, "import matplotlib.pyplot as plt\n");
    fprintf(py_file, "fig, ax = plt.subplots()\n");
}

void finalize_figure(FILE *py_file, int width, int height) {
    fprintf(py_file, "plt.xlim(0, %d)\n", width);
    fprintf(py_file, "plt.ylim(0, %d)\n", height);
    fprintf(py_file, "plt.gca().set_aspect('equal', adjustable='box')\n");
    fprintf(py_file, "plt.show()\n");
}
