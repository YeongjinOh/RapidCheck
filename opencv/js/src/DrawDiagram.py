import numpy as np
import cv2

def drawing_diagram():
	img = np.zeros((512, 512, 3), np.uint8) # (0,0,0) 은 BGR 에서 검정색, 검정색 바탕의 파텟트를 만듬

	"""
	OpenCV 는 BGR 모드를 사용하기 대문에, (255, 0, 0) 이 청색을 의미한다
	"""
	cv2.line(img, (0, 0), (511, 511), (255, 0, 0), 5) # 그림판, 시작위치, 끝위치, 색깔, 굵기
	cv2.rectangle(img, (384, 0), (510, 128), (0, 255, 0), 3) # 그림판, 좌측상단, 우측 하단, 색깔, 굵기
	cv2.circle(img, (447, 63), 63, (0,0,255), -1) # 그림판, 원의 중심, 원의 반지름, 색깔, 도형 채움

	font = cv2.FONT_HERSHEY_SIMPLEX
	cv2.putText(img, 'OPENCV', (10, 500), font, 4, (255, 255, 255), 2) # 그림판, 글자, 시작위치, 폰트, 폰트 크기, 색깔, 굵기

	cv2.imshow('drawing', img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	drawing_diagram()