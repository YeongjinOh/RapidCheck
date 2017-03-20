import cv2
import numpy as np

def drawing():
	img = np.zeros((512,512,3),np.uint8)

	# cv2.line(img,(0,0),(511,511),(255,100,100),10)
	# cv2.rectangle(img,(384,0),(510,128),(0,255,0),3)
	# cv2.circle(img,(447,63),63,(0,0,255),-1)
	# cv2.ellipse(img,(256,256),(100,50),0,0,360,(255,0,0),-1)

	font = cv2.FONT_HERSHEY_SIMPLEX
	cv2.putText(img,'Test',(100,500),font,4,(255,255,255),2)

	cv2.imshow('drawing',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	drawing()