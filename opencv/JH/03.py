import cv2

def handle_video():
    cap = cv2.VideoCapture(0)

    fps = 20.0
    width = int(cap.get(3))
    height = int(cap.get(4))
    fcc = cv2.VideoWriter_fourcc('D','I','V','X')

    out = cv2.VideoWriter('project.avi',fcc,fps,(width,height))

    while (cap.isOpened()):
        ret, frame = cap.read()

        if ret == 1:
            out.write(frame)
            cv2.imshow('frame',frame)

            if cv2.waitKey(1) & 0xff == ord('q'):
                break
        else:
            break
        # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # cv2.imshow("tesT",gray)

        # if cv2.waitKey(1) & 0xff == ord('q'):
        #     break

    cap.release()
    out.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    handle_video()