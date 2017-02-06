import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    edge1 = cv2.Canny(img, 50, 200)
    edge2 = cv2.Canny(img, 150, 200)
    edge3 = cv2.Canny(img, 170, 250)

    cv2.imshow('Original', img)
    cv2.imshow('Canny Edge1', edge1)
    cv2.imshow('Canny Edge2', edge2)
    cv2.imshow('Canny Edge3', edge3)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
