import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    rows, cols = img.shape[:2]

    pts1 = np.float32([[50,50],[200,50],[20,200]])
    pts2 = np.float32([[10,100],[200,50],[100,250]])

    ## find affine transform matrix
    ## which transforms 3 points pts1 into pts2
    M = cv2.getAffineTransform(pts1, pts2)

    img2 = cv2.warpAffine(img, M, (cols, rows))
    cv2.imshow('original', img)
    cv2.imshow('Affine Transform', img2)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
