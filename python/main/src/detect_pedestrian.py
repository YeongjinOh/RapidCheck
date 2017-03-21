# import the necessary packages
from __future__ import print_function
from imutils.object_detection import non_max_suppression
from imutils import paths
import numpy as np
import argparse
import imutils
import cv2

def detect_pedestrian():
    cap = cv2.VideoCapture('../videos/cctv5.mp4')

     # initialize the HOG descriptor/person detector
    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

    while True:
        ret, image = cap.read()
        #image = imutils.resize(frame, width=min(400, frame.shape[1]))

        # detect people in the image
        (rects, weights) = hog.detectMultiScale(image, winStride=(4, 4),
                                                padding=(8, 8), scale=1.05)

        # apply non-maxima suppression to the bounding boxes using a
        # fairly large overlap threshold to try to maintain overlapping
        # boxes that are still people
        rects = np.array([[x, y, x + w, y + h] for (x, y, w, h) in rects])
        pick = non_max_suppression(rects, probs=None, overlapThresh=0.65)
        for (x1, y1, x2, y2) in rects:
            cv2.rectangle(image, (x1,y1), (x2,y2), (0,0,255), 2)
        for (x1, y1, x2, y2) in pick:
            cv2.rectangle(image, (x1,y1), (x2,y2), (0,255,0), 1)
        cv2.imshow('pedestrian', image)
        k = cv2.waitKey(3) & 0xFF
        if k == 27:
            break;

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    detect_pedestrian()
