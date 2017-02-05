import cv2

def handle_img():
    img_path = "test.png"
    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    
    cv2.namedWindow("title",cv2.WINDOW_NORMAL) #이걸 실행하면 사진창의 크기를 조절 가능. (타이틀 이름은 같아야한다.)
    cv2.imshow("title", img)    
    # cv2.waitKey(2000) #windows frame is done after 2sec
    cv2.waitKey(0)
    cv2.destroyAllWindows() #생성한 모둔 윈도우를 제거

if __name__ == '__main__':1
    handle_img()
