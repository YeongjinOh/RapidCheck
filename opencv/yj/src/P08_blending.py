import numpy as np
import cv2

def onMouse(x):
    pass

def blending():
    fname1 = '../images/man1.jpeg'
    fname2 = '../images/man2.jpeg'

    img1 = cv2.imread(fname1)
    img2 = cv2.imread(fname2)

    cv2.namedWindow('ImgPane')
    cv2.createTrackbar('Blending', 'ImgPane', 0, 100, onMouse)
    mix = cv2.getTrackbarPos('Blending', 'ImgPane')

    while(1):
        img = cv2.addWeighted(img1, float(100-mix)/100, img2, float(mix)/100,0)
        cv2.imshow('ImgPane', img)

        k = cv2.waitKey(1)
        if k == 27:
            break

        mix = cv2.getTrackbarPos('Blending', 'ImgPane')

    cv2.destroyAllWindows()

if __name__ == '__main__':
    blending()
