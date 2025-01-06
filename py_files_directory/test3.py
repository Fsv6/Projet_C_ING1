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
def handle_cursor(id, x, y, visible):
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
    if  (visible == 'TRUE') :
        cursors[id]['turtle'].showturtle()
    else :
        cursors[id]['turtle'].hideturtle()
    turtle.tracer(1)
    turtle.update()
def handle_square(id, position, side_length, id_form, color='null'):
    if position == 'null':
        position = cursors[id]['turtle'].pos()
    initial_color = cursors[id]['turtle'].pencolor()
    if color != 'null':
        cursors[id]['turtle'].color(color)
    cursors[id]['turtle'].penup()
    cursors[id]['turtle'].goto(position)
    cursors[id]['turtle'].pendown()
    for _ in range(4):
        cursors[id]['turtle'].forward(side_length)
        cursors[id]['turtle'].left(90)
    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()
    cursors[id]['turtle'].penup()
    shapes.append((id_form, id, cursors[id]['turtle'].pos(), side_length, 'square'))
    cursors[id]['turtle'].color(initial_color)
def animation(ids, move_distance, angle_degrees, show):
    global animation_shapes
    global shapes
    final_animation = []
    angle_radians = math.radians(angle_degrees)
    dx = move_distance * math.cos(angle_radians)
    dy = move_distance * math.sin(angle_radians)
    for shape in shapes[:]:  # On parcourt une copie de la liste shapes
        if shape[1] in ids:  # Si l'id est dans la liste d'animation
            animation_shapes.append(shape)
            shapes.remove(shape)  # Retirer l'élément de shapes
    if (show):
        for shape in animation_shapes:
            if shape[1] in ids:
                cursors[shape[1]]['turtle'].showturtle()
    else:
        for shape in animation_shapes:
            if shape[1] in ids:
                cursors[shape[1]]['turtle'].hideturtle()
    for shape in animation_shapes[:]:
        if shape[1] in ids:  # Si l'id est dans la liste d'animation
            if shape[4] == 'circle':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_circle(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le cercle en blanc
                shapes.pop()
                handle_circle(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'line':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_line(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le carré en blanc
                shapes.pop()
                handle_line(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'square':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_square(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le carré en blanc
                shapes.pop()
                handle_square(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'semi-circle':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_semi_circle(shape[1], shape[2], shape[3], shape[0], 'white')  # Effacer le carré en blanc
                shapes.pop()
                handle_semi_circle(shape[1], new_position, shape[3], shape[0], 'null')
                final_animation.append(shapes.pop())
            elif shape[4] == 'point':
                new_position = (shape[2][0] + dx, shape[2][1] + dy)
                handle_point(shape[1], shape[2], shape[0], 'white')  # Effacer le carré en blanc
                shapes.pop()
                handle_point(shape[1], new_position, shape[0], 'null')
                final_animation.append(shapes.pop())
    # Redessiner les formes restantes de shapes qui ne sont pas animées
    for shape in shapes:
        if shape[4] == 'circle':
            shapes.remove(shape)
            handle_circle(shape[1], shape[2], shape[3])  # Redessiner le cercle
        elif shape[4] == 'line':
            shapes.remove(shape)
            handle_line(shape[1], shape[2], shape[3])  # Redessiner le carré
        elif shape[4] == 'square':
            shapes.remove(shape)
            handle_line(shape[1], shape[2], shape[3])  # Redessiner le carré
        elif shape[4] == 'semi-circle':
            shapes.remove(shape)
            handle_semi_circle(shape[1], shape[2], shape[3])  # Redessiner le carré
        elif shape[4] == 'point':
            shapes.remove(shape)
            handle_point(shape[1], shape[2], shape[3])  # Redessiner le carré
    shapes.extend(final_animation)
    animation_shapes = []  # Reset animation list after the animation is done
handle_cursor(1, 100, 100, 'TRUE')
handle_square(1, 'null', 20, 1)
set_all_cursors_speed(0)
turtle.tracer(n=1, delay=0)
for i in range(20):
    time.sleep(0.1)
    if (i == 20 - 1):
        show = True
    else:
        show = False
    animation([1], 5, 15, show)
turtle.tracer(n=1, delay=10)
set_all_cursors_speed(1)
turtle.done()
