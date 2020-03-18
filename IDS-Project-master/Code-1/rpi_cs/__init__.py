from flask import Flask, render_template, Response, request
from serial import Serial
from threading import Thread
from rpi_cs.camera import Camera


app = Flask(__name__)
#ser = Serial("/dev/ttyACM0", 9600)


@app.route('/')
def home():
    return render_template("home.html")


def gen(camera):
    """Video streaming generator function."""
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')


@app.route('/video_feed')
def video_feed():
    """Video streaming route. Put this in the src attribute of an img tag."""
    return Response(gen(Camera()),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


@app.route("/navigate/<direction>")
def navigation(direction):

    left = request.args.get('left')
    right = request.args.get('right')

    if (direction == "forward"):
        ser.write('{},0,{},0'.format(left, right).encode('utf-8'))

    elif(direction == "reverse"):
        ser.write('0,{},0,{}'.format(left, right).encode('utf-8'))

    elif (direction == "left"):
        ser.write('0,{},{},0'.format(left, right).encode('utf-8'))

    elif (direction == "right"):
        ser.write('{},0,0,{}'.format(left, right).encode('utf-8'))

    elif (direction == "stop"):
        ser.write(b'0,0,0,0')

    return ''
