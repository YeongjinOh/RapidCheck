import numpy as np
import cv2
from matplotlib import pyplot as plt

def harris():
    img = cv2.imread('../images/cctv.jpg')
    img2 = img.copy()
    imgray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

    # Harris Corner Detection
    imgray = np.float32(imgray)
    dst = cv2.cornerHarris(imgray, 2, 3, 0.04)

    dst = cv2.dilate(dst, None)
    img2[dst>0.01*dst.max()] = [0,0,255]
    cv2.imshow('Harris',img2)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

def shito():
    img = cv2.imread('../images/cctv.jpg')
    imgray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

    corners = cv2.goodFeaturesToTrack(imgray, 5, 0,01, 10)
    corners = np.int0(corners)

    for i in corners:
        print i
        x,y = i.ravel()
        cv2.circle(img, (x,y), 3, 255, -1)
    cv2.imshow('Shito',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    harris()
#    shito()
# shito error
