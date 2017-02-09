import numpy as np
import cv2
from matplotlib import pyplot as plt

BLUE = (255,0,0)
GREEN = (0,255,0)
RED = (0,0,255)
BLACK = (0,0,0)
WHITE = (255,255,255)

DRAW_BG = {'color':BLACK, 'val':0}
DRAW_FG = {'color':WHITE, 'val':1}
DRAW_PR_BG = {'color':RED, 'val':2}
DRAW_PR_FG = {'color':GREEN, 'val':3}

# setting up flags
rect = (0, 0, 1, 1)
drawing = False
rectangle = False
rect_over = False
rect_or_mask = 100
value = DRAW_FG
thickness = 3

def onMouse(event, x, y, flags, param):
    global ix, iy, img, img2, drawing, value, mask, rectangle, rect, rect_or_mask, rect_over

    if event == cv2.EVENT_RBUTTONDOWN:
        rectangle = True
        ix, iy = x, y

    elif event == cv2.EVENT_MOUSEMOVE:
        if rectangle:
            img = img2.copy()
            cv2.rectangle(img, (ix, iy), (x,y), RED, 2)
            rect = (min(ix,x), min(iy,y), abs(ix-x), abs(iy-y))
            rect_or_mask = 0

    elif event == cv2.EVENT_RBUTTONUP:
        rectangle = False
        rect_over = True

        cv2.rectangle(img, (ix,iy), (x,y), RED, 2)
        rect = (min(ix,x), min(iy,y), abs(ix-x), abs(iy-y))
        rect_or_mask = 0

    if event == cv2.EVENT_LBUTTONDOWN:
        if not rect_over:
            print 'Select area for foreground first!'
        else:
            drawing = True
            cv2.circle(img, (x,y), thickness, value['color'],-1)
            cv2.circle(mask, (x,y), thickness, value['val'],-1)

    elif event == cv2.EVENT_MOUSEMOVE:
        if drawing:
            cv2.circle(img, (x,y), thickness, value['color'], -1)
            cv2.circle(mask, (x,y), thickness, value['val'], -1)

    elif event == cv2.EVENT_LBUTTONUP:
        if drawing:
            drawiing = False
            # cv2.circle(mask, (x,y), thickness, value['val'], -1)
            # cv2.circle(img, (x,y), thickness, value['color'], -1)

    return

if __name__ == '__main__':
    img = cv2.imread('../images/cctv.jpg')
    img2 = img.copy()

    mask = np.zeros(img.shape[:2], dtype=np.uint8)
    output = np.zeros(img.shape, np.uint8)

    cv2.namedWindow('input')
    cv2.namedWindow('output')
    cv2.setMouseCallback('input', onMouse, param=(img, img2))
    cv2.moveWindow('input', img.shape[1]+10, 90)

    while True:
        cv2.imshow('output',output)
        cv2.imshow('input',img)
        k = cv2.waitKey(1) & 0xFF

        if k == 27:
            break
        elif k == ord('0'):
            print 'mark background area with left mouse button'
            value = DRAW_BG
        elif k == ord('1'):
            print 'mark fourground area with left mouse button'
            value = DRAW_FG
        elif k == ord('r'):
            print 'reset'
            rect = (0,0,1,1)
            drawing = False
            rectangle = False
            rect_or_mask = 100
            rect_over = False
            value = DRAW_FG
            img = img2.copy()
            mask = np.zeros(img.shape[:2], dtype=np.uint8)
            output = np.zeros(img.shape, np.uint8)
        elif k == ord('n'):
            print 'segment...'
            bgdModel = np.zeros((1,65), np.float64)
            fgdModel = np.zeros((1,65), np.float64)

            if rect_or_mask == 0:
                cv2.grabCut(img2, mask, rect, bgdModel, fgdModel, 1, cv2.GC_INIT_WITH_RECT)
                rect_or_mask = 1
            elif rect_or_mask == 1:
                cv2.grabCut(img2, mask, rect, bgdModel, fgdModel, 1, cv2.GC_INIT_WITH_MASK)
        mask2 = np.where((mask==1)+(mask==3),255,0).astype('uint8')
        output = cv2.bitwise_and(img2,img2,mask=mask2)
    cv2.destroyAllWindows()





