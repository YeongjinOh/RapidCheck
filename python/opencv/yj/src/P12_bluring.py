import numpy as np
import cv2
from matplotlib import pyplot as plt

def onMouse(x):
    pass

def main():
    img = cv2.imread('../images/cctv.jpg')

    # create trackbar
    cv2.namedWindow('BlurPane')
    cv2.createTrackbar('BLUR_MODE', "BlurPane", 0, 3, onMouse)
    cv2.createTrackbar('BLUR', "BlurPane", 0, 5, onMouse)


    while True:
        mode = cv2.getTrackbarPos('BLUR_MODE', 'BlurPane')
        val = cv2.getTrackbarPos('BLUR', 'BlurPane')
        val = val * 2 + 1
        try:
            # Averaging Blur
            if mode == 0:
                blur = cv2.blur(img, (val, val))
            # Gaussian Filter
            elif mode == 1:
                blur = cv2.GaussianBlur(img, (val, val), 0)
            # Median Filter
            elif mode == 2:
                blur = cv2.medianBlur(img, val)
            # Bilateral Filter
            # preserve edge but takes long time
            elif mode == 3:
                blur = cv2.bilateralFilter(img, val, 75, 75)
            else:
                break
            cv2.imshow('BlurPane', blur)
        except:
            break
        k = cv2.waitKey(1)
        if k == 27:
            break
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
