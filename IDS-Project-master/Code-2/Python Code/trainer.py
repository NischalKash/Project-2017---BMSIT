import os
import cv2
import numpy as np
from PIL import Image 

recognizer = cv2.face.createLBPHFaceRecognizer() #create recognizer object
path = 'dataSet'    #Path for the images

def getImageWithId(path):
	imagePaths = [os.path.join(path,f) for f in os.listdir(path)] #Absolute path for the images stored
	faces = []
	Ids = []
	for imagePath in imagePaths:
		faceImg=Image.open(imagePath).convert('L')
		faceNp=np.array(faceImg,'uint8')  #Add the images to a numpy array
		ID=int(os.path.split(imagePath)[-1].split('.')[1])
		faces.append(faceNp)
		Ids.append(ID)
		cv2.imshow("training",faceNp) #open a windows for training faces
		cv2.waitKey(10)
	return np.array(Ids),faces

Ids,faces = getImageWithId(path)
recognizer.train(faces,Ids)
recognizer.save('recognizer/trainingData.yml')  #Save the trained faces in a yml file in a specified path
cv2.destroyAllWindows()


