from collections import deque
import numpy as np
import imutils
import cv2

greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)
pts = deque(maxlen=64)

camera = cv2.VideoCapture(0)

kernels = np.ones((5,5), np.uint8)

while True:
    (grabbed, frame) = camera.read()
    
    frame = imutils.resize(frame, width=600)
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    cv2.imshow('hsv', hsv_frame)
    
    mask = cv2.inRange(hsv_frame, greenLower, greenUpper)
    cv2.imshow('mask', mask)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernels, iterations=2)
    cv2.imshow('opening', mask)

    th, contours_external, hierarchy1 = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    center = None

    if len(contours_external) > 0:
    	c = max(contours_external, key=cv2.contourArea)

    	((x,y), radius) = cv2.minEnclosingCircle(c)
    	M = cv2.moments(c)
    	center = (int(M['m10'] / M['m00']), int(M['m01']/M['m00']))

    	if radius > 10:
    		cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 2)
    		cv2.circle(frame, center, 5, (0,0,255), -1)

    pts.appendleft(center)
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
camera.release()
cv2.destroyAllWindows()