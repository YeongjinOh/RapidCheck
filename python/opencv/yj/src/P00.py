import numpy as np
import cv2
from matplotlib import pyplot as plt
import argparse

# set video path
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--videos", required=False, help="path to images directory")
args = vars(ap.parse_args())

def main():
    videoNum = args['videos']
    if videoNum is None:
        videoNum = 5
    cap = cv2.VideoCapture('../videos/cctv%s.mp4'%videoNum)
    while True:
        ret, frame = cap.read()
        k = cv2.waitKey(3) & 0xFF
        if k == 27:
            break;

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
