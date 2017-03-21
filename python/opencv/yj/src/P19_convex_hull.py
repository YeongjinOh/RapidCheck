import numpy as np
import cv2

def moment():
    img = cv2.imread('../images/cctv.jpg')
    imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    ret, thresh = cv2.threshold(imgray, 127, 255, 0)
    _, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

    for contour in contours:
        if contour.shape[0] > 50:
            break;

    check = cv2.isContourConvex(contour)
    if check == False:
        img1 = img.copy()
        hull = cv2.convexHull(contour)
        cv2.drawContours(img1, [hull], 0, (0, 255, 0), 3)
        cv2.imshow('convexhull',img1)
    cv2.drawContours(img, [contour], 0, (0, 255, 0), 1)
    cv2.imshow('contour',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
moment()
