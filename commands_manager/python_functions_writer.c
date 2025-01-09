#include "../python_functions_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



void set_python_file(FILE *python_file) {
    // Génère le code Python pour initialiser le curseur
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
    fprintf(python_file, "animation_shapes = []\n");
}


void end_python_file(FILE *python_file){
    // Génère le code Python pour initialiser le curseur
    fprintf(python_file, "turtle.done()\n");
}

/*
create CURSOR command: Creates a new cursor.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to initialize a new turtle cursor in Python, placing it at the specified (x, y) position, and optionally setting its visibility.
Returns:
    None
*/
void create_cursor_func_py(FILE *python_file) {


    // Génère le code Python pour initialiser le curseur
    fprintf(python_file, "def handle_cursor(id, x, y, visible):\n");
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
    fprintf(python_file, "    if  (visible == 'TRUE') :\n");
    fprintf(python_file, "        cursors[id]['turtle'].showturtle()\n");
    fprintf(python_file, "    else :\n");
    fprintf(python_file, "        cursors[id]['turtle'].hideturtle()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    turtle.tracer(1)\n");
    fprintf(python_file, "    turtle.update()\n");
}

/*
create COLOR command: Changes the color of a cursor.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to changes the color of a cursor.
Returns:
    None
*/
void create_color_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_color(id, color):\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(color)\n");
    fprintf(python_file, "    cursors[id]['color'] = color\n\n");
}

/*
create THICKNESS command: Changes the thickness of the pen for a cursor.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command Changes the thickness of the pen for a cursor.
Returns:
    None
*/
void create_thickness_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_thickness(id, thickness):\n");
    fprintf(python_file, "    cursors[id]['turtle'].pensize(thickness)\n");
    fprintf(python_file, "    cursors[id]['thickness'] = thickness\n");
}

/*
create MOVE command: Move a cursor.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to move a cursor in a direction giving the distance.
Returns:
    None
*/
void create_move_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_move(id, distance):\n");
    fprintf(python_file, "    cursors[id]['turtle'].forward(distance)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
}

/*
create GOTO command: Teleport a cursor.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to teleport the cursor to a position.
Returns:
    None
*/
void create_goto_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_goto(id, x, y):\n");
    fprintf(python_file, "    cursors[id]['turtle'].goto(x, y)\n");
    fprintf(python_file, "    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()\n");
}

/*
create ROTATE command: Rotates the cursor by a specified angle.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function rotates a cursor by the given angle in degrees. Positive angles rotate counterclockwise, and negative angles rotate clockwise.
Returns:
    None
*/
void create_rotate_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_rotate(id, angle):\n");
    fprintf(python_file, "    cursors[id]['turtle'].left(angle)\n");
}

/*
create LINE command: Make a line.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to make a line with a given distance.
Returns:
    None
*/
void create_line_func_py(FILE *python_file) {
    fprintf(python_file, "def handle_line(id, position, distance, id_form, color='null'):\n");
    fprintf(python_file, "    if position == 'null':\n");
    fprintf(python_file, "        position = cursors[id]['turtle'].pos()\n");
    fprintf(python_file, "    initial_color = cursors[id]['turtle'].pencolor()\n");
    fprintf(python_file, "\n");
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
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}

/*
create CIRCLE command: Make a circle.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to make a circle.
Returns:
    None
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
}

/*
create SQUARE command: Make a square.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to make a square.
Returns:
    None
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
}


/*create RECTANGLE command: Make a rectangle.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to make a rectangle.
Returns:
    None
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
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), side_length, 'rectangle'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}


/*
create POINT command: Make a point.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to make a point.
Returns:
    None
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
    fprintf(python_file, "    cursors[id]['turtle'].goto(position)\n");
    fprintf(python_file, "    shapes.append((id_form, id, cursors[id]['turtle'].pos(), 0, 'point'))\n");
    fprintf(python_file, "    cursors[id]['turtle'].color(initial_color)\n");
}


/*
create ARC command: Make a arc.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to make a arc.
Returns:
    None
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
}


/*
create ANIMATION command: Animate figure.
Args:
    python_file (FILE*): The Python file where the generated code will be written.
Purpose:
    This command create a function to animate figure.
Returns:
    None
*/
void create_animation_func_py(FILE *python_file) {
    fprintf(python_file, "def animation(ids, move_distance, angle_degrees, show):\n");
    fprintf(python_file, "    global animation_shapes\n");
    fprintf(python_file, "    global shapes\n");
    fprintf(python_file, "    final_animation = []\n");
    fprintf(python_file, "    angle_radians = math.radians(angle_degrees)\n");
    fprintf(python_file, "    dx = move_distance * math.cos(angle_radians)\n");
    fprintf(python_file, "    dy = move_distance * math.sin(angle_radians)\n");
    fprintf(python_file, "    for shape in shapes[:]:\n");
    fprintf(python_file, "        if shape[0] in ids:\n");
    fprintf(python_file, "            animation_shapes.append(shape)\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    if (show):\n");
    fprintf(python_file, "        for shape in animation_shapes:\n");
    fprintf(python_file, "            if shape[1] in ids:\n");
    fprintf(python_file, "                cursors[shape[1]]['turtle'].showturtle()\n");
    fprintf(python_file, "    else:\n");
    fprintf(python_file, "        for shape in animation_shapes:\n");
    fprintf(python_file, "            if shape[1] in ids:\n");
    fprintf(python_file, "                cursors[shape[1]]['turtle'].hideturtle()\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    for shape in animation_shapes[:]:\n");
    fprintf(python_file, "        if shape[0] in ids:\n");
    fprintf(python_file, "            if shape[4] == 'circle':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_circle(shape[1], shape[2], shape[3], shape[0], 'white')\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_circle(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'line':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_line(shape[1], shape[2], shape[3], shape[0], 'white')\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_line(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'square':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_square(shape[1], shape[2], shape[3], shape[0], 'white')\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_square(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'semi-circle':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_semi_circle(shape[1], shape[2], shape[3], shape[0], 'white')\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_semi_circle(shape[1], new_position, shape[3], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'point':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_point(shape[1], shape[2], shape[0], 'white')\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_point(shape[1], new_position, shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "            elif shape[4] == 'rectangle':\n");
    fprintf(python_file, "                new_position = (shape[2][0] + dx, shape[2][1] + dy)\n");
    fprintf(python_file, "                handle_rectangle(shape[1], shape[2], shape[3], shape[4], shape[0], 'white')\n");
    fprintf(python_file, "                shapes.pop()\n");
    fprintf(python_file, "                handle_point(shape[1], new_position, shape[3], shape[4], shape[0], 'null')\n");
    fprintf(python_file, "                final_animation.append(shapes.pop())\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    for shape in shapes:\n");
    fprintf(python_file, "        if shape[4] == 'circle':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_circle(shape[1], shape[2], shape[3], shape[0])\n");
    fprintf(python_file, "        elif shape[4] == 'line':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_line(shape[1], shape[2], shape[3], shape[0])\n");
    fprintf(python_file, "        elif shape[4] == 'square':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_line(shape[1], shape[2], shape[3], shape[0])\n");
    fprintf(python_file, "        elif shape[4] == 'semi-circle':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_semi_circle(shape[1], shape[2], shape[3], shape[0])\n");
    fprintf(python_file, "        elif shape[4] == 'point':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_point(shape[1], shape[2], shape[0])\n");
    fprintf(python_file, "        elif shape[4] == 'rectangle':\n");
    fprintf(python_file, "            shapes.remove(shape)\n");
    fprintf(python_file, "            handle_rectangle(shape[1], shape[2], shape[3], shape[4], shape[0])\n");
    fprintf(python_file, "\n");
    fprintf(python_file, "    shapes.extend(final_animation)\n");
    fprintf(python_file, "    animation_shapes = []\n");
}