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

if __name__ == '__main__':
    testImread()
