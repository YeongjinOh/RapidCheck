import numpy as np
import cv2

#TODO complete code

def onMouse (event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        print 'ldown'
        print x, y, flags, param
    elif event == cv2.EVENT_MOUSEMOVE:
        print 'move'
        print x, y, flags, param
    elif event == cv2.EVENT_LBUTTONUP:
        print 'lup'
        print x, y, flags, param

def meanShift():
    cap = cv2.VideoCapture(0)
    ret, frame = cap.read()

    cv2.namedWindow('frame')
    cv2.setMouseCallback('frame', onMouse, param=(frame, frame2))

    termination = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1)

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        if trackingWindow is not None:
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            dst = cv2.calcBackProject([hsv], [0], roi_hist, [0,180], 1)
            ret, trackingWindow = cv2.meanShift(dst, trackingWindow, termination)
            x, y, w, h = trackingWindow

            cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0), 2)
        cv2.imshow('frame',frame)

        k = cv2.waitKey(60) & 0xFF
        if k == 27:
            break
        if k == ord('i'):
            cv2.waitKey(0)
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    meanShift()
