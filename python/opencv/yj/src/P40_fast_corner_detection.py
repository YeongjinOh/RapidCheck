import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    fast = cv2.FastFeatureDetector_create(30)
    kp = fast.detect(img,None)
    cv2.drawKeypoints(img, kp, color=(255,0,0), outImage= img)

    cv2.imshow('FAST1', img)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
