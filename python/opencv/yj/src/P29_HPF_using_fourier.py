import numpy as np
import cv2
from matplotlib import pyplot as plt
def fourier():
    img = cv2.imread('../images/cctv.jpg',0)
    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)

    rows, cols =  fshift.shape
    crow, ccol = int(rows/2), int(cols/2)
    fshift[crow-30:crow+30,ccol-30:ccol+30] = 0
    # inverse
    f_ishift = np.fft.ifftshift(fshift)
    img_back = np.abs(np.fft.ifft2(f_ishift))

    plt.subplot(121),plt.imshow(img,cmap='gray')
    plt.title('Input Image')
    plt.subplot(122),plt.imshow(img_back, cmap = 'gray')
    plt.title('Magnitude Spectrum')
    plt.show()

if __name__ == '__main__':
    fourier()
