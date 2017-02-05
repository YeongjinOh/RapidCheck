import cv2

def testImread():
    img0 = cv2.imread('../images/beach.jpg',False)
    img1 = cv2.imread('../images/beach.jpg',True)
    img2 = cv2.imread('../images/beach.jpg',0)
    img3 = cv2.imread('../images/beach.jpg',1)

    cv2.imshow('beach0',img0);
    cv2.imshow('beach1',img1);
    cv2.imshow('beach2',img2);
    cv2.imshow('beach3',img3);

    cv2.waitKey(0)
    cv2.destroyAllWindows()

def testThreshold():
    img = cv2.imread('../images/beach.jpg', 0)

    ret, thresh1 = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
    ret, thresh2 = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY_INV)
    ret, thresh3 = cv2.threshold(img, 127, 255, cv2.THRESH_TRUNC)
    ret, thresh4 = cv2.threshold(img, 127, 255, cv2.THRESH_TOZERO)
    ret, thresh5 = cv2.threshold(img, 127, 255, cv2.THRESH_TOZERO_INV)
    ret, thresh6 = cv2.threshold(img, 127, 255, cv2.THRESH_TRIANGLE)

    cv2.imshow('original', img)
    cv2.imshow('res1', thresh1)
    cv2.imshow('res2', thresh2)
    cv2.imshow('res3', thresh3)
    cv2.imshow('res4', thresh4)
    cv2.imshow('res5', thresh5)
    cv2.imshow('res6', thresh6)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
#    testImread()
    testThreshold()
