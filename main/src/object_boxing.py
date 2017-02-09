import cv2
import numpy as np

def explain(obj):
    print('type:%s, shape:%s' %(type(obj), obj.shape))

def backSub():
    cap = cv2.VideoCapture('../videos/cctv5.mp4')
    mog = cv2.createBackgroundSubtractorMOG2(100,0)
    kernel = np.ones((3,3), np.uint8)
    trackWindow = None
    termination = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1)
    while True:
        ret, frame = cap.read()
        if trackWindow is None:
            fgmask = mog.apply(frame)
            opening = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel, iterations=2)
            # ret, thresh = cv2.threshold(dilation, 127, 255, 0)
            _, contours, hierarchy = cv2.findContours(opening, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            for contour in contours:
                if contour.shape[0] < 50:
                    continue
                x, y, w, h = cv2.boundingRect(contour)
                cv2.rectangle(frame, (x,y), (x+w,y+h), (0,0,255),3)
                if w*h > 50000:
                    trackWindow = (x, y, w, h)

                    # get roi hist
                    roi = frame[y:y+h,x:x+w]
                    roi = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)
                    roi_hist = cv2.calcHist([roi], [0], None, [180], [0,180])
                    cv2.normalize(roi_hist, roi_hist, 0, 255, cv2.NORM_MINMAX)

                    break
            cv2.drawContours(frame, contours, -1, (0,255,0),1)
        else:
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            dst = cv2.calcBackProject([hsv],[0],roi_hist,[0,180],1)
            ret, trackWindow = cv2.CamShift(dst, trackWindow,  termination)
            pts = cv2.boxPoints(ret)
            pts = np.int0(pts)
            cv2.polylines(frame, [pts], True, (0,255,0), 2)
        cv2.imshow('origin',frame)
        #cv2.imshow('mog',fgmask)
        #cv2.imshow('mog+opening', opening)

        k = cv2.waitKey(3) & 0xFF
        if k == 27:
            break;

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    backSub()
