import numpy as np
import cv2
from matplotlib import pyplot as plt

def HSVmap():
    hsv_map = np.zeros((180,256,3), np.uint8)
    h, s = np.indices(hsv_map.shape[:2])

    hsv_map[:,:,0] = h
    hsv_map[:,:,1] = s
    hsv_map[:,:,2] = 255
    hsv_map = cv2.cvtColor(hsv_map, cv2.COLOR_HSV2BGR)

    return hsv_map
#    plt.imshow(hsv_map)
#    plt.show()

def main():
    img = cv2.imread('../images/landscape.png')
    hsv_map = HSVmap()
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    hist = cv2.calcHist([hsv], [0,1], None, [180,256], [0,180,0,256])
    hist = np.clip(hist,0,100)
    plt.subplot(1,2,1)
    plt.imshow(cv2.cvtColor(img,cv2.COLOR_BGR2RGB))
    plt.subplot(1,2,2)
    plt.imshow(hsv_map*hist[:,:,np.newaxis], interpolation='nearest')
    plt.show()

if __name__ == '__main__':
    main()
