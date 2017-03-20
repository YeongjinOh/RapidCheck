import numpy as np
import cv2
from matplotlib import pyplot as plt
import argparse

# set video path
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--videos", required=False, help="path to images directory")
args = vars(ap.parse_args())

def main():
    videoNum = args['videos']
    if videoNum is None:
        videoNum = 5
    cap = cv2.VideoCapture("../videos/cctv%s.mp4"%videoNum)

    width, height = int(cap.get(3)), int(cap.get(4))
    termination = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03)
    feature_params = dict(maxCorners=500, qualityLevel=0.01, minDistance=7)
    lk_params = dict(winSize=(15,15), maxLevel=2, criteria=termination)

    color = (0,255,0)
    ret, frame = cap.read()
    oldGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    p0 = cv2.goodFeaturesToTrack(oldGray, mask=None, **feature_params)
    blackscreen = False
    while True:
        ret, frame = cap.read()

        frameGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        p1, st, err = cv2.calcOpticalFlowPyrLK(oldGray, frameGray, p0, None, **lk_params)

        if p0 is not None and p1 is not None:
            goodNew = p1[st==1]
            goodOld = p0[st==1]
        if blackscreen:
            frame = np.zeros((height, width, 3), np.uint8)
        for i, (new, old) in enumerate(zip(goodNew, goodOld)):
            a,b = new.ravel()
            c,d = old.ravel()
            cv2.circle(frame,(a,b),3,color,-1)
            cv2.line(frame,(a,b),(c,d),(255,0,0),1)
        cv2.imshow('frame',frame)

        k = cv2.waitKey(3) & 0xFF
        if k == 27:
            break;
        if k == ord('b'):
            blackscreen = not blackscreen
        oldGray = frameGray.copy()
        p0 = goodNew.reshape(-1,1,2)
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
