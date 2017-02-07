import cv2
import numpy as np

def explain(obj):
    print 'type:%s, shape:%s' %(type(obj), obj.shape)

def backSub():
    cap = cv2.VideoCapture('../videos/cctv5.mp4')
    mog = cv2.createBackgroundSubtractorMOG2()
    kernel = np.ones((3,3), np.uint8)

    while True:
        ret, frame = cap.read()
        fgmask = mog.apply(frame)
        erosion = cv2.erode(fgmask, kernel, iterations=1)
        dilation = cv2.dilate(erosion, kernel, iterations=1)

        ret, thresh = cv2.threshold(dilation, 127, 255, 0)
        _, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        cv2.drawContours(frame, contours, -1, (0,255,0),1)
#        cv2.drawContours(fgmask, contours, -1, (0,255,0),1)
#        cv2.drawContours(erosion, contours, -1, (0,255,0),1)
        cv2.imshow('origin',frame)
        cv2.imshow('fgmask',fgmask)
        cv2.imshow('erosion', dilation)

        k = cv2.waitKey(5)
        if k == 27:
            break;
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    backSub()
