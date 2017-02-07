import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/landscape.png')
    print img.shape
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    hist = cv2.calcHist([hsv], [0,1], None, [180,256], [0,180,0,256])
    plt.imshow(hist, interpolation='nearest')
    plt.show()

if __name__ == '__main__':
    main()
