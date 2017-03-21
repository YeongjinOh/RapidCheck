import numpy as np
import cv2

if __name__ == '__main__':
    blue = np.uint8([[[255,0,0]]])
    green = np.uint8([[[0,255,0]]])
    red = np.uint8([[[0,0,255]]])
    rg = np.uint8([[[0,255,255]]])
    rg_weak = np.uint8([[[0,125,125]]])

    hsv_blue = cv2.cvtColor(blue, cv2.COLOR_BGR2HSV)
    hsv_green = cv2.cvtColor(green, cv2.COLOR_BGR2HSV)
    hsv_red = cv2.cvtColor(red, cv2.COLOR_BGR2HSV)
    hsv_rg = cv2.cvtColor(rg, cv2.COLOR_BGR2HSV)
    hsv_rg_weak = cv2.cvtColor(rg_weak, cv2.COLOR_BGR2HSV)

    print 'Hue in [0,179] / Saturation in [0,255] / Value in [0,255]'
    print hsv_blue
    print hsv_green
    print hsv_red
    print hsv_rg
    print hsv_rg_weak
