import turtle
if 'cursors' not in globals():
    cursors = {}
turtle.setup(800, 600)
turtle.title('Test cursors')

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
def handle_color(id, color):
    cursors[id]['turtle'].color(color)
    cursors[id]['color'] = color
def handle_move(id, distance):
    cursors[id]['turtle'].forward(distance)
    cursors[id]['x'], cursors[id]['y'] = cursors[id]['turtle'].pos()
handle_cursor('A', 10, 15, 'TRUE')
if cursors['A']['x'] > 10 :
    handle_color('A', 'red')
else:
    handle_color('A', 'green')
handle_cursor('B', 20, 25, 'TRUE')
for I in range(0, 5):
    handle_move('A', 10+I*2)
turtle.done()
