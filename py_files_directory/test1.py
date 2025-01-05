import turtle

if 'cursors' not in globals():
    cursors = {}

def draw_axes():
    screen = turtle.Screen()
    screen.setworldcoordinates(-300, -300, 300, 300)
    axis_turtle = turtle.Turtle()
    axis_turtle.speed(0)
    axis_turtle.hideturtle()
    axis_turtle.penup()

    axis_turtle.goto(-300, 0)
    axis_turtle.pendown()
    axis_turtle.goto(300, 0)
    axis_turtle.penup()
    for x in range(-300, 301, 50):
        axis_turtle.goto(x, -10)
        axis_turtle.write(f"{x}", align="center")
        axis_turtle.goto(x, 0)
        axis_turtle.pendown()
        axis_turtle.goto(x, 5)
        axis_turtle.penup()

    axis_turtle.goto(0, -300)
    axis_turtle.pendown()
    axis_turtle.goto(0, 300)
    axis_turtle.penup()
    for y in range(-300, 301, 50):
        axis_turtle.goto(-10, y)
        axis_turtle.write(f"{y}", align="right")
        axis_turtle.goto(0, y)
        axis_turtle.pendown()
        axis_turtle.goto(5, y)
        axis_turtle.penup()

draw_axes()
x = 42
y = x
z = x * 2 + 3
w = 15
turtle.done()
