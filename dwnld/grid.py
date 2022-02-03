from turtle import *

def grid(width_len, height_len, margin_w, margin_h, width_pen):
	# initialisation turtle
	setup()
	setheading(0)
	up()
	goto(0, 0)
	pen(speed=0)
	width(width_pen)
	color("red")

	n = width_len / margin_w
	m = height_len / margin_h

	#Traces horizontales
	for i in range(0, n):
		print(i, margin_w*i)
		goto(0, margin_w * i)
		down()
		forward(height_len)
		up()

	left(90)

	for j in range(1, m):
		goto(margin_h * j, 0)
		down()
		forward(width_len)
		up()

	ht()