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

    # Set up tracker.
    # Instead of MIL, you can also use
    # BOOSTING, KCF, TLD, MEDIANFLOW or GOTURN
    tracker1 = cv2.Tracker_create("MIL")
    tracker2 = cv2.Tracker_create("BOOSTING")
    tracker3 = cv2.Tracker_create("KCF")
    tracker4 = cv2.Tracker_create("TLD")
    tracker5 = cv2.Tracker_create("MEDIANFLOW")
    trackers = [tracker1, tracker2, tracker3, tracker4, tracker5]
    colors = [(0,0,255), (0,255,0), (255,0,0), (0,255,255),(255,0,255),(255,255,0)]
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
                    for i in range(5):
                        ok = trackers[i].init(frame, trackWindow)
                    break
            cv2.drawContours(frame, contours, -1, (0,255,0),1)

        # start tracking
        else:
            for i in range(5):
                ok, bbox = trackers[i].update(frame)

                # Draw bounding box
                if ok:
                    p1 = (int(bbox[0]), int(bbox[1]))
                    p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
                    cv2.rectangle(frame, p1, p2, colors[i])

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
