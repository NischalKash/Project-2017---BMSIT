from threading import Thread
from rpi_cs import app
import socket
from serial import Serial

#ser = Serial("/dev/ttyACM0", 9600)

#if __name__ == "__main__":
#    app.run(host='0.0.0.0')

def flask_app():
    app.run(host='0.0.0.0',port = '8000',debug = True)
    
def rfidclient():
    client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    host = '192.168.0.107'  
    port = 12002
    client_socket.connect((host, port))
    while True:
        uid_tag = ser.readline().strip().decode("utf-8")                 
        client_socket.send(uid_tag.encode("utf-8"))
    client_socket.close()

if __name__ == '__main__':
    t1 = Thread(target=flask_app)
    t2 = Thread(target=rfidclient) 
    t1.start()
    t2.start()
