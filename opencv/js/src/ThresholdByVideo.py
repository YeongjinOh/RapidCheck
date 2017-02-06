import numpy as np
import cv2
from my_cv_lib import show_pictures
import threading

def thresh_video(modes, dev=0):
	cap = cv2.VideoCapture(dev)
	mog = cv2.createBackgroundSubtractorMOG2(500, 0, False)

	for mode in modes:
		cv2.namedWindow(mode, cv2.WINDOW_NORMAL)
	
	while True:
		ret, frame = cap.read()
		gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

		for mode in modes:
			if mode == 'original':
				# ret2, th4 = cv2.threshold(frame, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, frame)

			elif mode == 'gray':
				# ret2, th4 = cv2.threshold(gray_frame, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, gray_frame)
			
			elif mode == 'diff':
				gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
				fgmask = mog.apply(gray_frame)
				# ret2, th4 = cv2.threshold(fgmask, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, fgmask)
			
			elif mode == 'threshold':
				fgmask = mog.apply(gray_frame)
				ret2, th4 = cv2.threshold(fgmask, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				cv2.imshow(mode, th4)

			elif mode == 'blur':
				fgmask = mog.apply(gray_frame)
				blur = cv2.GaussianBlur(fgmask, (5, 5), 0)
				# ret, th = cv2.threshold(blur, 200, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
				ret, th = cv2.threshold(blur, 160, 255, cv2.THRESH_BINARY)
				cv2.imshow(mode, th)
			else:
				print("unknown mode")
		
		if cv2.waitKey(3) & 0xFF == 27:
				break
		
	cap.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	# th = threading.Thread(target=thresh_video, args=('original', 'cctv2.mp4', 'window_original',))
	thresh_video(modes=['original', 'diff', 'threshold', 'blur'] ,dev='../videos/cctv2.mp4')
	# th.start()
	# th.join()
