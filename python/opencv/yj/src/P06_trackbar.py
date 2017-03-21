import numpy as np
import cv2

def onChange(x):
    pass

def trackbar():
    img = np.zeros((300,512,3),np.uint8)
    name = 'color_palette'
    cv2.namedWindow(name)

    # cv2.createTrackbar(trackbarname, windowname, start, end, onChange)
    cv2.createTrackbar('B', name, 0, 255, onChange)
    cv2.createTrackbar('G', name, 0, 255, onChange)
    cv2.createTrackbar('R', name, 0, 255, onChange)

    switch = '0 : OFF \n1 : ON'
    cv2.createTrackbar(switch, name, 0, 1, onChange)

    while(1):
        cv2.imshow(name, img)
        k = cv2.waitKey(1)
        if k == 27:
            break
        # cv2.getTrackbarPos(trackbarname, windowname)
        b = cv2.getTrackbarPos('B', name)
        g = cv2.getTrackbarPos('G', name)
        r = cv2.getTrackbarPos('R', name)
        s = cv2.getTrackbarPos(switch, name)

        if s == 0:
            img[:] = 0
        else:
            img[:] = [b, g, r]

    cv2.destroyAllWindows()

if __name__ == '__main__':
    trackbar()
