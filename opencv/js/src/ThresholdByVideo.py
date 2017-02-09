import numpy as np
import cv2
from my_cv_lib import show_pictures
import threading

def thresh_video(modes, dev=0):
	cap = cv2.VideoCapture(dev)
	mog = cv2.createBackgroundSubtractorMOG2(100, 0, False)
	kernel = np.ones((2,2), np.uint8)

	for mode in modes:
		cv2.namedWindow(mode, cv2.WINDOW_NORMAL)
	
	while True:
		ret, frame = cap.read()
		gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

		for mode in modes:
			if mode == 'original':
				# ret2, th4 = cv2.threshold(frame, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, frame)
			
			elif mode == 'mog':
				fgmask = mog.apply(gray_frame)
				# ret2, th4 = cv2.threshold(fgmask, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, fgmask)
			
			elif mode == 'threshold':
				fgmask = mog.apply(gray_frame)
				ret2, th4 = cv2.threshold(fgmask, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, th4)

			elif mode == 'mog+blur':
				fgmask = mog.apply(gray_frame)
				blur = cv2.GaussianBlur(fgmask, (5, 5), 0)
				# ret, th = cv2.threshold(blur, 200, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				# ret, th = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, blur)

			elif mode == 'blur+mog':
				blur = cv2.GaussianBlur(gray_frame, (5, 5), 0)
				fgmask = mog.apply(blur)
				# ret, th = cv2.threshold(blur, 200, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				# ret, th = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, fgmask)
			elif mode == 'mog+opening':
				fgmask = mog.apply(gray_frame)
				opening = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel, iterations=2)
				cv2.imshow(mode, opening)
			
			elif mode == 'mog+opening+closing':
				fgmask = mog.apply(gray_frame)
				opening = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel, iterations=2)
				closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel, iterations=2)
				cv2.imshow(mode, closing)
			else:
				print("unknown mode")
		
		if cv2.waitKey(3) & 0xFF == 27:
				break
		
	cap.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	# th = threading.Thread(target=thresh_video, args=('original', 'cctv2.mp4', 'window_original',))
	thresh_video(modes=['mog', 'mog+opening', 'mog+opening+closing'] ,dev='../videos/cctv2.mp4')
	# th.start()
	# th.join()
