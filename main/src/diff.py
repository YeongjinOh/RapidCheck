import cv2
import numpy as np
import argparse

ap = argparse.ArgumentParser()
ap.add_argument("-v", "--videos", required=False, help="path to images directory")
args = vars(ap.parse_args())

def isContained(pt, rect, padding=10):
    x1, y1 = pt
    x, y, w, h = rect
    return x<=x1+padding and x1 <= x+w+padding and y <= y1+padding and y1 <= y+h+padding

# check rect1 overlapped with rect2
def isOverlapped (rect1, rect2):
    x1, y1, w1, h1 = rect1
    pts = [(x1,y1),(x1,y1+h1),(x1+w1,h1),(x1+w1,y1+h1)]
    for pt in pts:
        if isContained(pt, rect2):
            return True
    return False

def merge(rect1, rect2):
    x1, y1, w1, h1 = rect1
    x2, y2, w2, h2 = rect2
    x3 = min(x1,x2)
    y3 = min(y1,y2)
    x4 = max(x1+w1,x2+w2)
    y4 = max(y1+h1,y2+h2)
    return (x3,y3,x4-x3,y4-y3)

def mergeOnce (rects):
    for i in range(len(rects)):
        for j in range(i+1,len(rects)):
            if isOverlapped(rects[i], rects[j]) or isOverlapped(rects[j], rects[i]):
                rects.append(merge(rects.pop(j),rects.pop(i)))
                return True
    return False

def mergeAll(rects):
    while mergeOnce(rects):
        pass

def test():
    videoNum = args['videos']
    if videoNum is None:
        videoNum = 5
    cap = cv2.VideoCapture('../videos/cctv%s.mp4'%videoNum)
    ret, frame = cap.read()
    oldGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    kernel = np.ones((3,3), np.uint8)

    while True:
        ret, frame = cap.read()
        newGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        diffGray = cv2.absdiff(oldGray,newGray)
        ret, thresh = cv2.threshold(diffGray, 20,255,0)
        oldGray = newGray
        _, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        rects = []
        for contour in contours:
            if contour.shape[0] < 50:
                continue
            x, y, w, h = cv2.boundingRect(contour)
            rects.append((x, y, w, h))
            cv2.rectangle(frame, (x,y), (x+w,y+h), (0,0,255),3)
        mergeAll(rects)
        for (x, y, w, h) in rects:
            cv2.rectangle(frame, (x,y), (x+w,y+h), (0,255,0),2)
        cv2.drawContours(frame, contours, -1, (0,255,0),1)
        cv2.imshow('origin',frame)
        cv2.imshow('thresh',thresh)

        k = cv2.waitKey(3) & 0xFF
        if k == 27:
            break;

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    test()
