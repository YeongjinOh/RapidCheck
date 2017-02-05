import cv2

def onMouse(x):
	pass

def blending():
	filename1 = '225by225.png'
	filename2 = '225by225.jpg'

	img1 = cv2.imread(filename1)
	img2 = cv2.imread(filename2)

	cv2.namedWindow("img page")
	cv2.createTrackbar('MIXING','img page',0,100,onMouse)
	mix = cv2.getTrackbarPos('MIXING','img page')

	while 1:
		img = cv2.addWeighted(img1, float(100-mix)/100, img2, float(mix)/100,0)
		cv2.imshow('img page',img)

		k = cv2.waitKey(1) & 0xFF
		if k == 27:
			break
		mix = cv2.getTrackbarPos('MIXING','img page')
	cv2.destroyAllWindows()

# if __name__ == '__main__':
# 	blending()


def bitoperation(hpos,vpos):
	imgname1 = 'aircraft.png'
	imgname2 = 'opencv.png'

	img1 = cv2.imread(imgname1)
	img2 = cv2.imread(imgname2)

	rows, cols, channels = img2.shape #(180,144,3)
	roi = img1[vpos:rows+vpos, hpos:cols+hpos]
	

	img2gray = cv2.cvtColor(img2,cv2.COLOR_BGR2GRAY)
	ret, mask = cv2.threshold(img2gray, 10, 255, cv2.THRESH_BINARY)
	mask_inv = cv2.bitwise_not(mask)

	img1_bg = cv2.bitwise_and(roi, roi, mask=mask_inv)

	img2_fg = cv2.bitwise_and(img2,img2,mask = mask)

	dst = cv2.add(img1_bg, img2_fg)
	img1[vpos:rows + vpos, hpos:cols+hpos] = dst

	cv2.imshow('res',img1)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	bitoperation(10,10)