import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/star.png')
    imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, thresh = cv2.threshold(imgray, 127,255,0)
    _, contours, hierarchy = cv2.findContours(thresh,1,2)
    contour = contours[1]
    cv2.drawContours(img, [contour], 0, (0,255,0),3)

    x, y, w, h = cv2.boundingRect(contour)
    cv2.rectangle(img, (x,y), (x+w, y+h), (0,0,255),3)

    rect = cv2.minAreaRect(contour)
    #print type(rect)
    #print rect

    box = cv2.boxPoints(rect)
    print type(box)
    print box

    # type int
    box = np.int0(box)
    print type(box)
    print box
    cv2.drawContours(img, [box], 0, (0, 255, 0), 3)

    cv2.imshow('res',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
