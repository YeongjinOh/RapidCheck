import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    height, widt = img.shape[:2]
    print cv2.INTER_AREA
    print cv2.INTER_CUBIC + cv2.INTER_LINEAR

    # INTER_NEAREST, INTER_LINEAR, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4
    img2 = cv2.resize(img, None, fx=0.5, fy=1, interpolation=cv2.INTER_AREA)
    img3 = cv2.resize(img, None, fx=1, fy=0.5, interpolation=cv2.INTER_AREA)
    img4 = cv2.resize(img, None, fx=0.5, fy=0.5, interpolation=cv2.INTER_AREA)
    img5 = cv2.resize(img, None, fx=2, fy=2, interpolation=(cv2.INTER_CUBIC+cv2.INTER_LINEAR))

    cv2.imshow('original', img)
#    cv2.imshow('res2', img2)
#    cv2.imshow('res3', img3)
    cv2.imshow('res4', img4)
    cv2.imshow('res5', img5)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
