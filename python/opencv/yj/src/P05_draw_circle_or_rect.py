import numpy as np
import cv2
from random import shuffle
import math

mode, drawing = True, False
ix, iy = -1, -1
B, G, R = [i for i in range(256)], [i for i in range(256)], [i for i in range(256)]

def getRandomBGR():
    global B, G, R
    return (B[0], G[0], R[0])


def onMouse(event, x, y, flags, param):
    global ix, iy, drawing, mode, B, G, R

    color = getRandomBGR()

    if event == cv2.EVENT_LBUTTONDOWN:
        drawing = True
        ix, iy = x, y
        shuffle(B), shuffle(G), shuffle(R)

    elif event == cv2.EVENT_MOUSEMOVE:
        if drawing == True:
            if mode == True:
                cv2.rectangle(param, (ix, iy), (x, y), color, -1)
            else:
                r = (ix-x)**2 + (iy-y)**2
                r = int(math.sqrt(r))
                cv2.circle(param, (ix, iy), r, color, -1)
    elif event == cv2.EVENT_LBUTTONUP:
        drawing = False
        if mode == True:
            cv2.rectangle(param, (ix, iy), (x, y), color, -1)
        else:
            r = (ix-x)**2 + (iy-y)**2
            r = int(math.sqrt(r))
            cv2.circle(param, (ix, iy), r, color, -1)

def mouseBrush():
    global mode

    img = np.zeros((512, 512, 3), np.uint8)
    cv2.namedWindow('paint')
    cv2.setMouseCallback('paint', onMouse, param=img)

    while True:
        cv2.imshow('paint', img)
        k = cv2.waitKey(1)
        if k == ord('m'):
            mode = not mode
        elif k == 27:
            break
    cv2.destroyAllWindows()

if __name__ == '__main__':
    mouseBrush()
