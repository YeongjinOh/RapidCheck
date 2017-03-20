import numpy as np
import cv2
from matplotlib import pyplot as plt
def fourier():
    img = cv2.imread('../images/cctv.jpg',0)
    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)
    m_spectrum = 20*np.log(np.abs(fshift))

    plt.subplot(121),plt.imshow(img,cmap='gray')
    plt.title('Input Image')
    plt.subplot(122),plt.imshow(m_spectrum, cmap = 'gray')
    plt.title('Magnitude Spectrum')
    plt.show()

if __name__ == '__main__':
    fourier()
