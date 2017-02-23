import numpy as np
import cv2
from matplotlib import pyplot as plt
import argparse

# set video path
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--videos", required=False, help="path to images directory")
args = vars(ap.parse_args())

def explain(str, val):
    print str
    print type(val)
    print val, '\n'

def main():
    videoNum = args['videos']
    if videoNum is None:
        videoNum = 5
    cap = cv2.VideoCapture("../videos/cctv%s.mp4"%videoNum)

    ret, frame = cap.read()
    oldGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    while True:
        ret, frame = cap.read()
        frameGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        flow = cv2.calcOpticalFlowFarneback(oldGray, frameGray, None, 0.5, 3, 15, 3, 5, 1.2, 0)
        rows, cols = flow.shape[:2]
#        for row in xrange(rows):
#            for col in xrange(cols):
#                [y,x] =  flow[row,col]
#                cv2.line(frame, (col,row), (int(col+x),int(row+y)), (255,0,0),1)
        cv2.imshow('frame',frame)

        k = cv2.waitKey(3) & 0xFF
        if k == 27:
            break;
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
