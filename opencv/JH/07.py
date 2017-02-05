import cv2

if __name__ == '__main__':
	img = cv2.imread('test.png')

	px = img[50,30]
	print(px)

	B = img.item(50,30,0)
	G = img.item(50,30,1)
	R = img.item(50,30,2)

	BGR = [B,G,R]
	print(BGR)

	img.itemset((50,30,0),100)
	print(px)

	print('='*10)

	print(img.shape) 
	print(img.size)
	print(img.dtype)
	# (142, 355, 3)
	# 151230
	# uint8

	print("----ROL (Region Of Image)----")
	cv2.imshow('original',img)
	chair = img[30:40,60:90]
	img[0:10,0:30] = chair

	print(img.shape)
	print(chair.shape)

	cv2.imshow('modfying',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

	print("----BGR split----")
	b, g, r = cv2.split(img)
	print(img[50, 70])	
	print(b[50, 70],g[50, 70],r[50, 70])

	cv2.imshow("b",b)
	cv2.imshow("g",g)
	cv2.imshow("r",r)

	cv2.waitKey(0)
	cv2.destroyAllWindows()

	print("----BGR merge----")

	merged_img = cv2.merge((b,g,r))
	cv2.imshow('merge',merged_img)

	cv2.waitKey(0)
	cv2.destroyAllWindows()