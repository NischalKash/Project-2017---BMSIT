import sys
import json
import requests
from serial import Serial
from datetime import datetime
import cv2
import numpy as np
import time

server = "http://" + sys.argv[1] + ":5000/" #server ip address passed as an argument to the program
ser = Serial("/dev/ttyACM0", 9600) #connecting to the arduino

def face_detect(): #function to recognoze a fcae
	faceDetect=cv2.CascadeClassifier('haarcascade_frontalface_default.xml') #xml template for  face recognition
	cam=cv2.VideoCapture(0) #start face recognition
	cam.set(3,320)
	cam.set(4,240)
	rec=cv2.face.createLBPHFaceRecognizer()
	rec.load('recognizer/trainingData.yml') #loading the training data
	uid = 0
	count =0
	font = cv2.FONT_HERSHEY_COMPLEX_SMALL
	while True:
		while(count!=10):
			ret,img=cam.read() #getting image from the video capture
			gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY) #converting image to greyscale
			faces=faceDetect.detectMultiScale(gray,1.3,5) #detecting a face
			print("Face Detected")
			for (x,y,w,h) in faces:
				cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2) #create a face box to highlight the face detected
				id=rec.predict(gray[y:y+h,x:x+w]) #recognizing a face
				if(id == 123):
					id = 123
					return id
                elif(id == 456):
                    id = 456
                    return id
                elif(id == 789):
                    id = 789
                    return id
			time.sleep(1)
			count = count+1
				
		if(count == 10):
			id = 999
			return id
			 
			#cv2.putText(img,str(id),(x,y+h), font, 1, (200,255,155), 2, cv2.LINE_AA) #display uid in the face box
		#cv2.imshow("Face",img)
		
		#if(cv2.waitKey(1) == ord('q')): #wait for the q key to be pressed to stop the recognition
			#break;
	cam.release()
	cv2.destroyAllWindows()

while True:
    uid_tag = ser.readline().strip().decode("utf-8") #scan uid from rfid reader
    t = datetime.now().strftime('%Y-%m-%d %H:%M:%S') #time of authentication
    conv = {'pino': 'x01', 'uid': uid_tag, 'time':t}
    s = json.dumps(conv)
    res = requests.post(server, json = s).json() #send to server and get acknoledegment stored in res
    if res['result'] == 'Authorized': #rfid confirmed
        print('the rfid verified, facial recognition will commence')
        face_uid = face_detect() #call face recognition function and get the uid of the person
        dat = json.dumps({'fid' : face_uid})
        result = requests.post(server + 'facerec',json = dat).json() #send the uid to the server for verification
        print('the person is {}'.format(result['val']))
        ser.write(b'10')
    else:
        print('invalid rfid, person Unauthorized')
        ser.write(b'10')

