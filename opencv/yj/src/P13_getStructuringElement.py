import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    M1 = cv2.getStructuringElement(cv2.MORPH_RECT, (5,5))
    M2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5,5))
    M3 = cv2.getStructuringElement(cv2.MORPH_CROSS, (5,5))

    print M1, '\n'
    print M2, '\n'
    print M3

if __name__ == '__main__':
    main()
