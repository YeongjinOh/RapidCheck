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
    print contour
    moments = cv2.moments(contour)

    keys = moments.keys()
    '''
    print type(moments), '\n' # print dict
    print moments, '\n'
    print type(keys), '\n' # print list
    print keys
    '''

    for key in keys:
        print '%s:\t %f' %(key, moments[key])

    # print center of weight
    cx = int(moments['m10']/moments['m00'])
    cy = int(moments['m01']/moments['m00'])
    print cx, cy

    # print area, perimeter
    area = cv2.contourArea(contour)
    perimeter = cv2.arcLength(contour,True)
    print area, ' ', perimeter

    cv2.drawContours(img, [contour], 0, (0, 255, 0), 1)
    cv2.imshow('contour',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
moment()
