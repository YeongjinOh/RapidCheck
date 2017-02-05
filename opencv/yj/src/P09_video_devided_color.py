import numpy as np
import cv2

def testRange():
    img = cv2.imread('../images/beach.jphg')
    print img.shape
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    # define range of HSV
    lower_blue = np.array([110,100,100])
    upper_blue = np.array([130,255,255])
    lower_green = np.array([50,100,100])
    upper_green = np.array([70,255,255])
    lower_red = np.array([-10,100,100])
    upper_red = np.array([10,255,255])

    # treshold the HSV image to get each colo
    mask_blue = cv2.inRange(hsv,lower_blue,upper_blue)
    mask_green = cv2.inRange(hsv,lower_green,upper_green)
    mask_red = cv2.inRange(hsv,lower_red,upper_red)

    print lower_blue
    print mask_blue



def tracking():
    try:
        print 'Start Camera..'
        cap = cv2.VideoCapture(0)
    except:
        print 'Cannot Load Camera...'
        return

    while True:
        ret, frame = cap.read()
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # define range of HSV
        lower_blue = np.array([110,100,100])
        upper_blue = np.array([130,255,255])
        lower_green = np.array([50,100,100])
        upper_green = np.array([70,255,255])
        lower_red = np.array([-10,100,100])
        upper_red = np.array([10,255,255])

        # treshold the HSV image to get each color
        mask_blue = cv2.inRange(hsv,lower_blue,upper_blue)
        mask_green = cv2.inRange(hsv,lower_green,upper_green)
        mask_red = cv2.inRange(hsv,lower_red,upper_red)

        res_blue = cv2.bitwise_and(frame, frame, mask=mask_blue);
        res_green = cv2.bitwise_and(frame, frame, mask=mask_green);
        res_red = cv2.bitwise_and(frame, frame, mask=mask_red);

        cv2.imshow('original', frame);
        cv2.imshow('blue', res_blue);
        cv2.imshow('green', res_green);
        cv2.imshow('red', res_red);

    cv2.destroyAllWindows()

if __name__ == '__main__':
    tracking()
#    testRange()

