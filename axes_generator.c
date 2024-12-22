#include <stdio.h>

void generate_axes_code(FILE *python_file) {
    fprintf(python_file, "def draw_axes():\n");
    fprintf(python_file, "    screen = turtle.Screen()\n");
    fprintf(python_file, "    screen.setworldcoordinates(-300, -300, 300, 300)\n");
    fprintf(python_file, "    axis_turtle = turtle.Turtle()\n");
    fprintf(python_file, "    axis_turtle.speed(0)\n");
    fprintf(python_file, "    axis_turtle.hideturtle()\n");
    fprintf(python_file, "    axis_turtle.penup()\n");
    fprintf(python_file, "\n");
    // Axe X
    fprintf(python_file, "    axis_turtle.goto(-300, 0)\n");
    fprintf(python_file, "    axis_turtle.pendown()\n");
    fprintf(python_file, "    axis_turtle.goto(300, 0)\n");
    fprintf(python_file, "    axis_turtle.penup()\n");
    fprintf(python_file, "    for x in range(-300, 301, 50):\n");
    fprintf(python_file, "        axis_turtle.goto(x, -10)\n");
    fprintf(python_file, "        axis_turtle.write(f\"{x}\", align=\"center\")\n");
    fprintf(python_file, "        axis_turtle.goto(x, 0)\n");
    fprintf(python_file, "        axis_turtle.pendown()\n");
    fprintf(python_file, "        axis_turtle.goto(x, 5)\n");
    fprintf(python_file, "        axis_turtle.penup()\n");
    fprintf(python_file, "\n");
    // Axe Y
    fprintf(python_file, "    axis_turtle.goto(0, -300)\n");
    fprintf(python_file, "    axis_turtle.pendown()\n");
    fprintf(python_file, "    axis_turtle.goto(0, 300)\n");
    fprintf(python_file, "    axis_turtle.penup()\n");
    fprintf(python_file, "    for y in range(-300, 301, 50):\n");
    fprintf(python_file, "        axis_turtle.goto(-10, y)\n");
    fprintf(python_file, "        axis_turtle.write(f\"{y}\", align=\"right\")\n");
    fprintf(python_file, "        axis_turtle.goto(0, y)\n");
    fprintf(python_file, "        axis_turtle.pendown()\n");
    fprintf(python_file, "        axis_turtle.goto(5, y)\n");
    fprintf(python_file, "        axis_turtle.penup()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "draw_axes()\n");
}
