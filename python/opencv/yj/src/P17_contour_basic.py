import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    imgray = cv2.imread('../images/cctv.jpg',0)
    img = cv2.imread('../images/cctv.jpg')
    #imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    ret, thresh = cv2.threshold(imgray, 177, 200, 0)
    image, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    cv2.imshow('image', image)
    cv2.imshow('thresh', thresh)
    cv2.drawContours(img, contours, -1, (0, 255, 0), 1)
    cv2.imshow('Contour', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
