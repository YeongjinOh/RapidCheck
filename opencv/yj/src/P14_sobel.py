import numpy as np
import cv2
from matplotlib import pyplot as plt

def grad():
    img = cv2.imread('../images/cctv.jpg',0)

    # OpenCV provides 3 gradient filters : Sobel, Scharr, Laplacian
    # NOTE : Sobel can have negative values,
    # so be careful when using CV_8U type
    laplacian = cv2.Laplacian(img, cv2.CV_64F)
    sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=3)
    sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=3)

    plt.subplot(2,2,1),plt.imshow(img,cmap='gray')
    plt.subplot(2,2,2),plt.imshow(laplacian,cmap='gray')
    plt.subplot(2,2,3),plt.imshow(sobelx,cmap='gray')
    plt.subplot(2,2,4),plt.imshow(sobely,cmap='gray')
    plt.show()

grad()
