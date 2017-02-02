import cv2
from matplotlib import pyplot as plt


def handle_video():
    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame',gray)
        if cv2.waitKey(1) == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

def test_get():
    cap = cv2.VideoCapture(0)
    for i in range(18):
        print cap.get(i)

if __name__ == '__main__':
    #handle_video()
    test_get()
