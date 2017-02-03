import cv2

def handle_video():
    cap = cv2.VideoCapture(0)

    fps = 20.0
    width = int(cap.get(3))
    height = int(cap.get(4))

    # available code list : DIVS, SVID, MJPG, X264, WMV1, WMV2
    #    FourCC is a 4-byte code used to specify the video codec. The list of available codes can be found in fourcc.org. It is platform dependent. Following codecs works fine for me.
    #    In Fedora: DIVX, XVID, MJPG, X264, WMV1, WMV2. (XVID is more preferable. MJPG results in high size video. X264 gives very small size video)
    #    In Windows: DIVX (More to be tested and added)
    #    In OSX : *(I don't have access to OSX. Can some one fill this?)*
    fcc = cv2.VideoWriter_fourcc(*'DIVX')
    out = cv2.VideoWriter('mycam.avi', fcc, fps, (width, height))

    while (cap.isOpened()):
        ret, frame = cap.read()

        if ret == True:
            out.write(frame)
            cv2.imshow('frame', frame)

            if cv2.waitKey(1) == ord('q'):
                break
        else:
            break

    cap.release()
    out.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    handle_video()
