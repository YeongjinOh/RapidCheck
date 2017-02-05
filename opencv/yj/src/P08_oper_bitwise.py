import numpy as np
import cv2

def bitoperation (hpos, vpos):
    imgname1 = '../images/beach.jpg'
    imgname2 = '../images/ironman.jpg'

    img1 = cv2.imread(imgname1)
    img2 = cv2.imread(imgname2)

    rows, cols, channels = img2.shape
    roi = img1[vpos:rows+vpos, hpos:cols+hpos]

    # create a mask
    img2gray = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    ret, mask = cv2.threshold(img2gray, 10, 255, cv2.THRESH_BINARY)
    mask_inv = cv2.bitwise_not(mask)

    img1_bg = cv2.bitwise_and(roi,roi,mask=mask_inv)
    img2_fg = cv2.bitwise_and(img2,img2,mask=mask)

dst = cv2.add(img1_bg,img2_fg)
    img1[vpos:rows+vpos, hpos:cols+hpos]=dst
    cv2.imshow('bg',img1_bg)
    cv2.imshow('fg',img2_fg)
    cv2.imshow('res', img1)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    bitoperation(10,10)
