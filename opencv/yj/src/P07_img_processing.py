import cv2

if __name__ == '__main__':
    img = cv2.imread('../images/beach.jpg')
    px = img[10,50]
    b = img.item(10,50,0)
    g = img.item(10,50,1)
    r = img.item(10,50,2)
    print px
    print [b, g, r]

    print img.shape
    print img.size
    print img.dtype

    ''' crop
    part = img[40:80,30:100]
    img[10:50,200:270] = part
    cv2.imshow('part',img)
    '''

    b, g, r = cv2.split(img)
    cv2.imshow('blue', b)
    cv2.imshow('green', g)
    cv2.imshow('red', r)

    merged = cv2.merge((b, g, r))
    cv2.waitKey(0)
    cv2.destroyAllWindows()
