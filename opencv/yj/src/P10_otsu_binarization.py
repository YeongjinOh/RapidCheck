import cv2
import numpy as np
from matplotlib import pyplot as plt

def thresholding():
    img = cv2.imread('../images/beach.jpg',0)

    # Global Thresholding
    ret, th1 = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)

    # Otsu's Binarization
    ret2, th2 = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)

    # Otsu's Thresholding after Gaussian filtering
    blur = cv2.GaussianBlur(img, (5, 5), 0)
    ret3, th3 = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)

    titles = ['original noisy', 'Histogram', 'G-Thresholding', 'original noisy',
              'Histogra', 'Otsu Thresholding', 'Gaussian-filtered', 'Histogram', 'Otsu Thresholding']
    images = [img, 0, th1, img, 0, th2, blur, 0,  th3]

    for i in xrange(3):
        plt.subplot(3,3,i*3+1), plt.imshow(images[i*3], 'gray')
        plt.title(titles[i*3]), plt.xticks([]), plt.yticks([])

        # plt.hist(img.ravel())
        plt.subplot(3,3,i*3+2), plt.hist(images[i*3].ravel(), 256)
        plt.title(titles[i*3+1]), plt.xticks([]), plt.yticks([])

        plt.subplot(3,3,i*3+3), plt.imshow(images[i*3+2], 'gray')
        plt.title(titles[i*3+2]), plt.xticks([]), plt.yticks([])
    plt.show()

if __name__ == '__main__':
    thresholding()
