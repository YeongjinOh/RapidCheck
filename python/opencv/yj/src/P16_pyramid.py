import numpy as np
import cv2
from matplotlib import pyplot as plt

def main():
    img = cv2.imread('../images/cctv.jpg')
    tmp = img.copy()

    win_names = ['org', 'lv1', 'lv2', 'lv3']
    g_down = []
    g_down.append(tmp)
    img_shape = []
    for i in range(3):
        tmp1 = cv2.pyrDown(tmp)
        g_down.append(tmp1)
        tmp = tmp1

#    for i in range(4):
#        cv2.imshow(win_names[i], g_down[i])
    g_up = []
    for i in range(3):
        tmp = g_down[i+1]
        tmp1 = cv2.pyrUp(tmp)
        # if images size is odd number, set dsize
        tmp1_resized = cv2.resize(tmp1, dsize=(g_down[i].shape[1], g_down[i].shape[0]),
                                  interpolation=cv2.INTER_CUBIC)
        g_up.append(tmp1_resized)
        laplacian = cv2.subtract(g_down[i],g_up[i])
        cv2.imshow(win_names[i+1], laplacian)

#    cv2.imshow('Original', img)
#    cv2.imshow('Down and then UpScaled', g_up[3])

    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
