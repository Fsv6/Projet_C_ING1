#include "../python_functions_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Writes the initial Python code to set up the environment for turtle cursors.
// Includes imports, global variable initialization, and utility functions.
// Creates global variables: 'cursors' (dictionary for cursor properties), 'shapes' (list of drawn shapes),
// and 'animation_shapes' (list of shapes used in animations).
void set_python_file(FILE *python_file) {
    fprintf(python_file, "import turtle\n");
    fprintf(python_file, "import time\n");
    fprintf(python_file, "import math\n");
    fprintf(python_file, "if 'cursors' not in globals():\n");
    fprintf(python_file, "    cursors = {}\n");
    fprintf(python_file, "    turtle.setup(800, 600)\n");
    fprintf(python_file, "    turtle.title('Projects Cursors')\n");
    fprintf(python_file, "    turtle.tracer(n=2, delay=10)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "def set_all_cursors_speed(speed):\n");
    fprintf(python_file, "    for cursor in cursors.values():\n");
    fprintf(python_file, "        cursor['turtle'].speed(speed)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "shapes = []\n");
    fprintf(python_file, "animation_shapes = []\n\n");
}

// Writes the final Python code to terminate the turtle program.
void end_python_file(FILE *python_file){
    fprintf(python_file, "turtle.done()\n\n");
}

// Writes the Python function to handle cursor creation.
// The function initializes a new cursor with specified attributes like position and visibility.
// Each cursor is stored as a dictionary within the global 'cursors' dictionary. This dictionary
// holds properties such as position (x, y), visibility, color, and thickness. By default, the
// visibility is set to "TRUE", and the color is "black".
void create_cursor_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_cursor(id, x, y, visible='TRUE'):\n");
    fprintf(python_file, "    if id not in cursors:\n");
    fprintf(python_file, "        cursors[id] = {\n");
    fprintf(python_file, "            'turtle': turtle.Turtle(),\n");
    fprintf(python_file, "            'x': x,\n");
    fprintf(python_file, "            'y': y,\n");
    fprintf(python_file, "            'visible': visible,\n");
    fprintf(python_file, "            'color': 'black',\n");
    fprintf(python_file, "            'thickness': 1\n");
    fprintf(python_file, "        }\n");
    fprintf(python_file, "        cursors[id]['turtle'].color('black')\n");
    fprintf(python_file, "        cursors[id]['turtle'].pensize(1)\n");
    fprintf(python_file, "        cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "        cursors[id]['turtle'].speed(1)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    turtle.tracer(0)\n");
    fprintf(python_file, "    cursors[id]['x'] = x\n");
    fprintf(python_file, "    cursors[id]['y'] = y\n");
    fprintf(python_file, "    cursors[id]['turtle'].setpos(x, y)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    if visible == 'TRUE':\n");
    fprintf(python_file, "        cursors[id]['turtle'].showturtle()\n");
    fprintf(python_file, "    else:\n");
    fprintf(python_file, "        cursors[id]['turtle'].hideturtle()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    turtle.tracer(1)\n");
    fprintf(python_file, "    turtle.update()\n\n");
}

// Writes the Python function to change a cursor's color.
void create_color_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_color(id, color='black'):\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['color'] = color\n\n");
}

// Writes the Python function to change a cursor's pen thickness.
void create_thickness_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_thickness(id, thickness):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pensize(thickness)\n");
    fprintf(python_file, "    cursors[id]['thickness'] = thickness\n\n");
}

// Writes the Python function to move a cursor forward by a specified distance.
void create_move_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_move(id, distance):\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(distance)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n\n");
}

// Writes the Python function to teleport a cursor to a specified position.
void create_goto_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_goto(id, x, y):\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(x, y)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n\n");
}

// Writes the Python function to rotate a cursor by a specified angle.
void create_rotate_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_rotate(id, angle):\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(angle)\n\n");
}

// Writes the Python function to draw a line.
// The line is drawn from the specified position to a point calculated based on the given distance.
// If no position is provided ('null'), the cursor's current position is used as the starting point.
void create_line_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_line(id, position, distance, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(distance)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), distance, 'line'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n\n");
}

/*
Creates the Python function for handling circles.
This function defines the Python logic for drawing a circle with a specified
radius at a given position using the Turtle module. The function stores the
circle's properties in the `shapes` list for tracking.
*/
void create_circle_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_circle(id, position, radius, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].circle(radius)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), radius, 'circle'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
    fprintf(python_file, "\n");
}

/*
Creates the Python function for handling squares.
This function defines the Python logic for drawing a square with a specified
side length at a given position. The function also stores the square's properties
in the `shapes` list for tracking.
*/
void create_square_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_square(id, position, side_length, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    for _ in range(4):\n");
    fprintf(python_file, "        cursors[id]['turtle'].forward(side_length)\n");
    fprintf(python_file, "        cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), side_length, 'square'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
    fprintf(python_file, "\n");
}

/*
Creates the Python function for handling rectangles.
This function defines the Python logic for drawing a rectangle with specified
width and height. The rectangle's properties are also stored in the `shapes` list.
*/
void create_rectangle_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_rectangle(id, position, side_1, side_2, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_1)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_2)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_1)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(side_2)\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(90)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), side_1, 'rectangle'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
    fprintf(python_file, "\n");
}

/*
Creates the Python function for handling points.
This function defines the Python logic for drawing a point at a specific position.
The function records the point's properties in the `shapes` list for tracking.
*/
void create_point_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_point(id, position, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].dot()\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), 0, 'point'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
    fprintf(python_file, "\n");
}

/*
Creates the Python function for handling arcs.
This function defines the Python logic for drawing an arc (semi-circle) with a
specific radius at a given position. The arc's properties are recorded in the `shapes` list.
*/
void create_arc_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_semi_circle(id, position, radius, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "    if color != 'null':\n");
    fprintf(python_file, "        cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    cursors[id]['turtle'].pendown()\n");
    fprintf(python_file, "    cursors[id]['turtle'].circle(radius, 180)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    cursors[id]['turtle'].penup()\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(180)\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), radius, 'semi-circle'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
    fprintf(python_file, "\n");
}



// Writes the Python function to animate shapes.
// This function moves a figure (shape) stored in the global 'shapes' list.
// The movement is defined by a specified distance and angle, applied to the shape's position.
void create_animation_func_py(FILE *python_file) {
    fprintf(python_file, "def animation(ids, move_distance, angle_degrees, show):\n");
    fprintf(python_file, "    global animation_shapes\n");
    fprintf(python_file, "    global shapes\n");
    fprintf(python_file, "    final_animation = []\n");
    fprintf(python_file, "    angle_radians = math.radians(angle_degrees)\n");
    fprintf(python_file, "    dx = move_distance * math.cos(angle_radians)\n");
    fprintf(python_file, "    dy = move_distance * math.sin(angle_radians)\n");

    // Process each shape to update its position
    fprintf(python_file, "    for shape in shapes[:]:\n");
    fprintf(python_file, "        if shape[0] in ids:\n");
    fprintf(python_file, "            animation_shapes.append(shape)\n");
    fprintf(python_file, "            shapes.remove(shape)\n");

    // Handle visibility of shapes during animation
    fprintf(python_file, "    if show:\n");
    fprintf(python_file, "        for shape in animation_shapes:\n");
    fprintf(python_file, "            if shape[1] in ids:\n");
    fprintf(python_file, "                cursors[shape[1]]['turtle'].showturtle()\n");
    fprintf(python_file, "    else:\n");
    fprintf(python_file, "        for shape in animation_shapes:\n");
    fprintf(python_file, "            if shape[1] in ids:\n");
    fprintf(python_file, "                cursors[shape[1]]['turtle'].hideturtle()\n\n");

    // Update positions for animated shapes
    fprintf(python_file, "    for shape in animation_shapes[:]:\n");
    fprintf(python_file, "        if shape[0] in ids:\n");
    fprintf(python_file, "            new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "            handle_circle(shape[1], shape[2], shape[3], shape[0], 'white')\n");
    fprintf(python_file, "            shapes.pop()\n");
    fprintf(python_file, "            handle_circle(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "            final_animation.append(shapes.pop())\n\n");

    // Restore shapes to final positions
    fprintf(python_file, "    shapes.extend(final_animation)\n");
    fprintf(python_file, "    animation_shapes = []\n\n");
}
