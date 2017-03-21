import cv2
import numpy as np

def thresholding():
    img = cv2.imread('../images/beach.jpg',0)

    ret, th1 = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
    th2 = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)
    th3 = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

    titles = ['original', 'global', 'adaptive MEAN', 'adaptive GAUSSIAN']
    images = [img, th1, th2, th3]

    for i in xrange(4):
        cv2.imshow(titles[i],images[i])

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    thresholding()
