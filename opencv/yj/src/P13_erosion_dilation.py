import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')

    kernel = np.ones((3,3), np.uint8)

    rows, cols = img.shape[:2]
    edge = cv2.Canny(img,rows,cols)
    erosion = cv2.erode(edge, kernel, iterations=1)
    dilation = cv2.dilate(edge, kernel, iterations=1)
#    opening = cv2.dilate(erosion, kernel,iterations=1)
#    closing = cv2.erode(dilation, kernel,iterations=1)

    opening = cv2.morphologyEx(edge, cv2.MORPH_OPEN, kernel)
    closing = cv2.morphologyEx(edge, cv2.MORPH_CLOSE, kernel)

    cv2.imshow('original',edge)
    cv2.imshow('erosion',erosion)
    cv2.imshow('delation',dilation)
    cv2.imshow('opening',opening)
    cv2.imshow('closing',closing)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
