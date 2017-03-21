import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    rows, cols = img.shape[:2]

    M1 = cv2.getRotationMatrix2D((cols/2, rows/2), 45, 1)
    M2 = cv2.getRotationMatrix2D((cols/2, rows/2), 90, 0.5)

    img2 = cv2.warpAffine(img, M1, (cols, rows))
    img3 = cv2.warpAffine(img, M2, (cols, rows))
    cv2.imshow('res1', img2)
    cv2.imshow('res2', img3)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
