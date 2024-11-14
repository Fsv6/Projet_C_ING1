// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "draw_functions.h"  // Inclut les déclarations des fonctions stockées

void parse_draw_file(const char *input_filename, const char *output_filename) {
    FILE *draw_file = fopen(input_filename, "r");
    if (!draw_file) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *py_file = fopen(output_filename, "w");
    if (!py_file) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }


    initialize_figure(py_file);

    char line[256];
    int x, y, radius;

    while (fgets(line, sizeof(line), draw_file)) {
        if (sscanf(line, "DRAW CIRCLE x=%d y=%d radius=%d", &x, &y, &radius) == 3) {
            generate_circle(py_file, x, y, radius);
        } else {
            printf("Unrecognized command: %s", line);
        }
    }

    finalize_figure(py_file, x * 2, y * 2);

    fclose(draw_file);
    fclose(py_file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.draw> <output.py>\n", argv[0]);
        return 1;
    }
    parse_draw_file(argv[1], argv[2]);
    printf("Python file generated: %s\n", argv[2]);
    return 0;
}
