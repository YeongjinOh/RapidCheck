import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    rows, cols = img.shape[:2]

    pts1 = np.float32([[0,0],[300,0],[0,300],[300,300]])
    pts2 = np.float32([[56,65],[368,52],[28,387],[200,150]])

    ## find affine transform matrix
    ## which transforms 3 points pts1 into pts2
    M = cv2.getPerspectiveTransform(pts1, pts2)

    img2 = cv2.warpPerspective(img, M, (cols*2, rows*2))
    cv2.imshow('original', img)
    cv2.imshow('Perspective Transform', img2)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
