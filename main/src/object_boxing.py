import cv2
import numpy as np

def explain(obj):
    print('type:%s, shape:%s' %(type(obj), obj.shape))

def backSub():
    cap = cv2.VideoCapture('../videos/cctv5.mp4')
    mog = cv2.createBackgroundSubtractorMOG2()
    kernel = np.ones((3,3), np.uint8)

    while True:
        ret, frame = cap.read()
        fgmask = mog.apply(frame)
        # erosion = cv2.erode(fgmask, kernel, iterations=1)
        # dilation = cv2.dilate(erosion, kernel, iterations=1)
        opening = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel, iterations=2)
        closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel, iterations=3)

        # ret, thresh = cv2.threshold(dilation, 127, 255, 0)
        _, contours, hierarchy = cv2.findContours(closing, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        cv2.drawContours(frame, contours, -1, (0,255,0),1)
#        cv2.drawContours(fgmask, contours, -1, (0,255,0),1)
#        cv2.drawContours(erosion, contours, -1, (0,255,0),1)
        cv2.imshow('origin',frame)
        cv2.imshow('mog',fgmask)
        cv2.imshow('mog+opening+closing', closing)

        if cv2.waitKey(3) & 0xFF == 27:
            break;
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    backSub()
