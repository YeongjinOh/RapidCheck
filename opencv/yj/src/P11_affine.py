import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    rows, cols = img.shape[:2]

    M = np.float32([[1,0,100],[0,1,50]])

    img2 = cv2.warpAffine(img, M, (cols+200, rows+100))
    cv2.imshow('res', img2)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
