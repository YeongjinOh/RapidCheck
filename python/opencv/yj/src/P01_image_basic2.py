import cv2
from matplotlib import pyplot as plt

def handle_image():
    imgfile = '../images/ironman.jpg'
    img = cv2.imread(imgfile, cv2.IMREAD_GRAYSCALE)

    plt.imshow(img, cmap='gray', interpolation = 'bicubic')
    # remove x,y-axis scale
    plt.xticks([]), plt.yticks([])
    plt.show()


if __name__ == '__main__':
    handle_image()
