import turtle
import time
import math
if 'cursors' not in globals():
    cursors = {}
    turtle.setup(800, 600)
    turtle.title('Projects Cursors')
    turtle.tracer(n=2, delay=10)

def set_all_cursors_speed(speed):
    for cursor in cursors.values():
        cursor['turtle'].speed(speed)

shapes = []
animation_shapes = []

def handle_cursor(id, x, y, visible='TRUE'):
    if id not in cursors:
        cursors[id] = {
            'turtle': turtle.Turtle(),
            'x': x,
            'y': y,
            'visible': visible,
            'color': 'black',
            'thickness': 1
        }
        cursors[id]['turtle'].color('black')
        cursors[id]['turtle'].pensize(1)
        cursors[id]['turtle'].penup()
        cursors[id]['turtle'].speed(1)

    turtle.tracer(0)
    cursors[id]['x'] = x
    cursors[id]['y'] = y
    cursors[id]['turtle'].setpos(x, y)

    if visible == 'TRUE':
        cursors[id]['turtle'].showturtle()
    else:
        cursors[id]['turtle'].hideturtle()

    turtle.tracer(1)
    turtle.update()

def handle_color(id, color='black'):
    cursors[id]['turtle'].color(color)
    cursors[id]['color'] = color

def handle_circle(id, position, radius, id_form, color='null'):
    if position == 'null':
        position = cursors[id]['turtle'].pos()

    initial_color = cursors[id]['turtle'].pencolor()
    if color != 'null':
        cursors[id]['turtle'].color(color)

    cursors[id]['turtle'].penup()
    cursors[id]['turtle'].goto(position)
    cursors[id]['turtle'].pendown()
    cursors[id]['turtle'].circle(radius)
    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()
    cursors[id]['turtle'].penup()
    shapes.append((id_form, id, cursors[id]['turtle'].pos(), radius, 'circle'))
    cursors[id]['turtle'].color(initial_color)

def handle_move(id, distance):
    cursors[id]['turtle'].forward(distance)
    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()

x = 42
y = x
z = x * 2 + 3
handle_cursor(1, y, z, 'TRUE')
if cursors[1]['x'] > 10 :
    handle_color(1, 'red')
    handle_cursor(2, -100, -100, 'TRUE')
    handle_cursor(3, 100, 100, 'TRUE')
    handle_color(2, 'green')
else:
    handle_color(1, 'green')
handle_circle(1, 'null', 100, 2)
for I in range(1,10):
    handle_move(1, I*10)
    pass
turtle.done()

