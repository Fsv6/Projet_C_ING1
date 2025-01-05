import turtle
import math
if 'cursors' not in globals():
    cursors = {}
turtle.setup(800, 600)
turtle.title('Test cursors')

turtle.tracer(n=1, delay=0)
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
        cursors[id]['turtle'].speed(0)

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
handle_cursor('1', 10, 15, 'TRUE')

turtle.done()
