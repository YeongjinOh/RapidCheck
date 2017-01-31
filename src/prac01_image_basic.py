import numpy as np
import cv2
from matplotlib import pyplot as plt

def handle_image():
    imgfile = '../images/ironman.jpg'
    img = cv2.imread(imgfile, cv2.IMREAD_COLOR)

    cv2.namedWindow('IRONMAN', cv2.WINDOW_NORMAL)
    cv2.imshow('IRONMAN', img)
    k = cv2.waitKey(0)

    # ord : character to ASCII
    # chr : ASCII to character
    if k == ord('c'):
        cv2.imwrite('../images/ironman_copy.jpg', img)
    cv2.destroyAllWindows()


if __name__ == '__main__':
    handle_image()
