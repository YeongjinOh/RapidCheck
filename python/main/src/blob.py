import cv2
import numpy as np
import argparse

ap = argparse.ArgumentParser()
ap.add_argument("-v", "--videos", required=False, help="path to images directory")
args = vars(ap.parse_args())

def test():
    videoNum = args['videos']
    if videoNum is None:
        videoNum = 5
        cap = cv2.VideoCapture('../videos/cctv%s.mp4'%videoNum)
        ret, frame = cap.read()
        oldGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        kernel = np.ones((3,3), np.uint8)

    while True:
        ret, frame = cap.read()
        frameGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Set up the detector with default parameters.
        detector = cv2.SimpleBlobDetector_create()

        # Detect blobs.
        keypoints = detector.detect(frameGray)

        # Draw detected blobs as red circles.
        # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle
        # corresponds to the size of blob
        im_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
        cv2.imshow("Keypoints", im_with_keypoints)
        k = cv2.waitKey(3) & 0xFF
        if k == 27:
            break;

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    test()
