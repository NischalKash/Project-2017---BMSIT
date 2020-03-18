import cv2
import numpy as np 

faceDetect=cv2.CascadeClassifier('haarcascade_frontalface_default.xml') #Cascase fronatal face
cam=cv2.VideoCapture(0) #Capture video stream
cam.set(3,320)  #Set camera resoultion
cam.set(4,240)

uid=input("Enter the User ID matching with the RFID of the person")
sampleNum=0

while True:
	ret,img=cam.read() #Read image from the camera object
	gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY) #Convert the image to grey scale
	faces=faceDetect.detectMultiScale(gray,1.3,5)  #Detect a face from the image
	for (x,y,w,h) in faces:
		sampleNum = sampleNum + 1
		cv2.imwrite("dataSet/User."+str(uid)+"."+str(sampleNum)+".jpg",gray[y:y+h,x:x+w]) #Store the image in the specified path
		cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2)    #Add a rectangle box to the face detected
		cv2.waitKey(100)      #Wait for 100ms and detect the face again
	cv2.imshow("Face",img)     #open a window to display the face
	cv2.waitKey(1)
	if(sampleNum>100):          #Train 100 samples of the face
		break
cam.release()
cv2.destroyAllWindows()

 
