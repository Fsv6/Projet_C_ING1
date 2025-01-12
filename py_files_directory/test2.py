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

def animation(ids, move_distance, angle_degrees, show):
    global animation_shapes
    global shapes
    final_animation = []
    angle_radians = math.radians(angle_degrees)
    dx = move_distance * math.cos(angle_radians)
    dy = move_distance * math.sin(angle_radians)
    for shape in shapes[:]:
        if shape[0] in ids:
            animation_shapes.append(shape)
            shapes.remove(shape)

    if (show):
        for shape in animation_shapes:
            if shape[1] in ids:
                cursors[shape[1]]['turtle'].showturtle()
    else:
        for shape in animation_shapes:
            if shape[1] in ids:
                cursors[shape[1]]['turtle'].hideturtle()

    for shape in animation_shapes[:]:
        if shape[0] in ids:
            if shape[4] == 'circle':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_circle(shape[1], shape[2], shape[3], shape[0], 'white')
                shapes.pop()
                handle_circle(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'line':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_line(shape[1], shape[2], shape[3], shape[0], 'white')
                shapes.pop()
                handle_line(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'square':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_square(shape[1], shape[2], shape[3], shape[0], 'white')
                shapes.pop()
                handle_square(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'semi-circle':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_semi_circle(shape[1], shape[2], shape[3], shape[0], 'white')
                shapes.pop()
                handle_semi_circle(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'point':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_point(shape[1], shape[2], shape[0], 'white')
                shapes.pop()
                handle_point(shape[1], new_position, shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'rectangle':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_rectangle(shape[1], shape[2], shape[3], shape[4], shape[0], 'white')
                shapes.pop()
                handle_point(shape[1], new_position, shape[3], shape[4], shape[0], 'null')
                final_animation.append(shapes.pop())

    for shape in shapes:
        if shape[4] == 'circle':
            shapes.remove(shape)
            handle_circle(shape[1], shape[2], shape[3], shape[0])
        elif shape[4] == 'line':
            shapes.remove(shape)
            handle_line(shape[1], shape[2], shape[3], shape[0])
        elif shape[4] == 'square':
            shapes.remove(shape)
            handle_square(shape[1], shape[2], shape[3], shape[0])
        elif shape[4] == 'semi-circle':
            shapes.remove(shape)
            handle_semi_circle(shape[1], shape[2], shape[3], shape[0])
        elif shape[4] == 'point':
            shapes.remove(shape)
            handle_point(shape[1], shape[2], shape[0])
        elif shape[4] == 'rectangle':
            shapes.remove(shape)
            handle_rectangle(shape[1], shape[2], shape[3], shape[4], shape[0])

    shapes.extend(final_animation)
    animation_shapes = []
centerX = 50
centerY = 50
radius = 30
moveStep = 15
handle_cursor(1, centerX, centerY, 'TRUE')
handle_color(1, 'yellow')
handle_circle(1, 'null', radius, 3)
for ANGLE in range(0, 1000):
    handle_cursor(2, centerX+math.cos(ANGLE)*radius, centerY+math.sin(ANGLE)*radius, 'TRUE')
if cursors[2]['x'] > 30 :
    handle_color(1, 'red')
    handle_move(1, centerX+moveStep)
    handle_move(2, centerX-moveStep)
else:
    handle_color(1, 'green')
    handle_color(2, 'orange')
set_all_cursors_speed(0)
turtle.tracer(n=10, delay=0)

for i in range(10):
    time.sleep(0.1)
    if (i == 10 - 1):
        show = True
    else:
        show = False
    animation([1,2,3], 4, 90, show)

turtle.tracer(n=1, delay=10)
set_all_cursors_speed(1)
turtle.done()

