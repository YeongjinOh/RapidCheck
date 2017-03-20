import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/star.png')
    imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    rows, cols = img.shape[:2]

    ret, thresh = cv2.threshold(imgray, 127,255,0)
    _, contours, hierarchy = cv2.findContours(thresh,1,2)
    contour = contours[1]

    (x,y), r = cv2.minEnclosingCircle(contour)
    center = (int(x), int(y))
    r = int(r)
    blue = (255,0,0)
    green = (0,255,0)
    red = (0,0,255)
    cv2.circle(img,center,r,blue,3)
    ellipse = cv2.fitEllipse(contour)
    cv2.ellipse(img,ellipse,green,3)
    print 'ellipse type : ', type(ellipse)
    print ellipse

    [vx,vy,x,y] = cv2.fitLine(contour,1,0,0.01,0.01)
    ly = int((-x*vy/vx)+y)
    ry = int(((cols-x)*vy/vx)+y)

    cv2.line(img, (cols-1,ry),(0,ly),red,2)

    cv2.imshow('res',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
