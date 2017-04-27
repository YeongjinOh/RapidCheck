using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices; //Marshal
using System.Windows.Forms;

//directShow
using DirectShowLib;
using System.Drawing;
using System.Drawing.Imaging; //img capture

namespace RapidCheck
{
    public partial class Form1
    {
        DirectShowLib.IGraphBuilder pGraphBuilder = null; //DirectShow Graph
        DirectShowLib.IMediaControl pMediaControl = null; //DirectShow Control
        DirectShowLib.IMediaEvent pMediaEvent = null; //DirectShow Event
        EventCode eventCode;
        DirectShowLib.IVideoWindow pVideoWindow = null;

        DirectShowLib.IMediaPosition pMediaPosition = null; // play time...

        DirectShowLib.IVideoFrameStep pVideoFrameStep = null; // video stream을 진행시킨다...

        //capture
        //DirectShowLib.IBasicVideo pBasicVideo = null;
        //sampleGrabber
        DirectShowLib.IBaseFilter pSampleGrabberFilter = null;
        DirectShowLib.ISampleGrabber pSampleGrabber = null;
        int Video_Width, Video_Height;

        private void Video2Btn_Click(object sender, EventArgs e)
        {
            //Videotest1();
            Videotest2();
        }
        private void captureBtn_Click(object sender, EventArgs e)
        {
            pCapture();
        }
        private void Videotest1()
        {
            pGraphBuilder = (DirectShowLib.IGraphBuilder)new FilterGraph();
            pMediaControl = (DirectShowLib.IMediaControl)pGraphBuilder; //controler
            pMediaEvent = (DirectShowLib.IMediaEvent)pGraphBuilder; //envent

            pMediaControl.RenderFile(@"C:\Users\trevor\Desktop\Videos\cctv2.wmv");
            pVideoWindow = (DirectShowLib.IVideoWindow)pGraphBuilder; // renderFile을 실행 한 후 연결해줘야한다. 출력처를 알기위해서.
            //pVideoWindow.put_Caption("test"); // setTitle
            //pVideoWindow.put_FullScreenMode(OABool.True); //Video full Screen
            pVideoWindow.put_Owner(tabPage2.Handle); // tabPage2에 그리기.
            pVideoWindow.put_WindowStyle(WindowStyle.Child | WindowStyle.ClipSiblings);
            Rectangle rect = tabPage2.ClientRectangle;
            pVideoWindow.SetWindowPosition(0, 0, rect.Right, rect.Bottom);
            pMediaControl.Run(); // Video play

            pMediaEvent.WaitForCompletion(-1, out eventCode); //블로킹을 실시하는 메소드

            switch (eventCode)
            {
                case 0:
                    textBox1.Text = "TimeOut";
                    break;
                case EventCode.UserAbort: //ex) alt+f4
                    textBox1.Text = "User Abort";
                    break;
                case EventCode.Complete:
                    textBox1.Text = "complete";
                    break;
                case EventCode.ErrorAbort:
                    textBox1.Text = "Error Abort";

                    break;
            }

            //pVideoWindow.put_FullScreenMode(OABool.False); // !full Screen

            //free
            Marshal.ReleaseComObject(pGraphBuilder);
            pGraphBuilder = null;
            Marshal.ReleaseComObject(pMediaControl);
            pMediaControl = null;
            Marshal.ReleaseComObject(pMediaEvent);
            pMediaEvent = null;
        }
        private void Videotest2()
        {
            if (DirectShowBtn.Text == "DiShow")
            {
                play();
                DirectShowBtn.Text = "close";
            }
            else if (DirectShowBtn.Text == "close")
            {
                CloseInterface();
                DirectShowBtn.Text = "DiShow";
            }
        }
        private void play()
        {
            SetupGraph(tabPage2, @"C:\Users\trevor\Desktop\Videos\cctv2.wmv");
            pMediaControl.Run();
        }
        private void SetupGraph(Control hWin, string filename)
        {
            pGraphBuilder = (DirectShowLib.IGraphBuilder)new FilterGraph();
            pMediaControl = (DirectShowLib.IMediaControl)pGraphBuilder;
            pVideoWindow = (DirectShowLib.IVideoWindow)pGraphBuilder;
            pVideoFrameStep = (DirectShowLib.IVideoFrameStep)pGraphBuilder; // video frame...
            //DirectShowLib.IBaseFilter pBaseFilter = (DirectShowLib.IBaseFilter)pGraphBuilder;

            //test
            DirectShowLib.ICaptureGraphBuilder2 pCaptureGraphBuilder2;
            DirectShowLib.IBaseFilter pRenderer;
            DirectShowLib.IVMRFilterConfig9 pIVMRFilterConfig9;
            DirectShowLib.IVMRWindowlessControl9 pVMRWC9;

            pCaptureGraphBuilder2 = (DirectShowLib.ICaptureGraphBuilder2)new CaptureGraphBuilder2();
            pCaptureGraphBuilder2.SetFiltergraph(pGraphBuilder);     // CaptureGraph를  GraphBuilder에 붙인다.
            
            //pGraphBuilder.AddFilter(pMediaControl "SDZ 375 Source");  // GraphBuilder에 영상장치필터를 추가한다.
            pRenderer = (DirectShowLib.IBaseFilter)new DirectShowLib.VideoMixingRenderer9();       // 믹서 필터를 생성 한다.
            pIVMRFilterConfig9 = (DirectShowLib.IVMRFilterConfig9)pRenderer;         // 믹서 필터의 속성을 설정한다.
            pIVMRFilterConfig9.SetRenderingMode(VMR9Mode.Windowless);
            //pIVMRFilterConfig9.SetRenderingMode(VMR9Mode.Windowed);
            pIVMRFilterConfig9.SetNumberOfStreams(2);

            pVMRWC9 = (DirectShowLib.IVMRWindowlessControl9)pRenderer;              // 오버레이 평면의 속성을 설정한다.
            pVMRWC9.SetVideoClippingWindow(hWin.Handle);
            pVMRWC9.SetBorderColor(0);
            pVMRWC9.SetVideoPosition(null, hWin.ClientRectangle);
            pGraphBuilder.AddFilter(pRenderer, "Video Mixing Renderer"); // GraphBuilder에 믹스 필터를 추가한다.
            pCaptureGraphBuilder2.RenderStream(null, MediaType.Video, pGraphBuilder , null, pRenderer);   // 영상표시를 위한 필터를 설정한다.
            ///test

            //sampleGrabber
            AMMediaType am_media_type = new AMMediaType();
            pSampleGrabber = (DirectShowLib.ISampleGrabber)new SampleGrabber();
            pSampleGrabberFilter = (DirectShowLib.IBaseFilter)pSampleGrabber;
            am_media_type.majorType = MediaType.Video;
            am_media_type.subType = MediaSubType.RGB24;
            am_media_type.formatType = FormatType.VideoInfo;
            pSampleGrabber.SetMediaType(am_media_type);
            //Graph에 sampleGrabber filter를 추가
            pGraphBuilder.AddFilter(pSampleGrabberFilter, "Sample Grabber");

            pMediaControl.RenderFile(filename);
            

            pVideoWindow.put_Owner(hWin.Handle);
            pVideoWindow.put_WindowStyle(WindowStyle.Child | WindowStyle.ClipSiblings);
            Rectangle rect = hWin.ClientRectangle;
            pVideoWindow.SetWindowPosition(0, 0, rect.Right, rect.Bottom);
            
            //sampleGrabber2
            pSampleGrabber.GetConnectedMediaType(am_media_type);
            DirectShowLib.VideoInfoHeader pVideoInfoHeader = (DirectShowLib.VideoInfoHeader)Marshal.PtrToStructure(am_media_type.formatPtr, typeof(VideoInfoHeader));
            String str = string.Format("size = {0} x {1}", pVideoInfoHeader.BmiHeader.Width, pVideoInfoHeader.BmiHeader.Height);
            Video_Width = pVideoInfoHeader.BmiHeader.Width;
            Video_Height = pVideoInfoHeader.BmiHeader.Height;
            str += string.Format("sample size = {0}", am_media_type.sampleSize);
            textBox1.Text = str;
            DsUtils.FreeAMMediaType(am_media_type);
            //SetBufferSamples를 실행하지 않으면 버퍼로부터 데이터를 얻을 수 없다.
            //불필요하게 부하를 주고싶지 않은경우 false, 데이터를 얻고싶으면 true
            pSampleGrabber.SetBufferSamples(true);

            //play time
            pMediaPosition = (DirectShowLib.IMediaPosition)pGraphBuilder;
            double Length;
            pMediaPosition.get_Duration(out Length);
            String str2 = string.Format("play time: {0}", Length);
            textBox1.Text = str2;
        }
        #region 그래프와 프로그램 종료
        private void CloseInterface()
        {
            //그래프와 콘트롤 해제
            if (pMediaControl != null)
            {
                pMediaControl.StopWhenReady();
            }
            if (pVideoWindow != null)
            {
                pVideoWindow.put_Visible(OABool.False);
                pVideoWindow.put_Owner(IntPtr.Zero);
            }

            Marshal.ReleaseComObject(pSampleGrabber);
            pSampleGrabber = null;
            Marshal.ReleaseComObject(pSampleGrabberFilter);
            pSampleGrabberFilter = null;
            Marshal.ReleaseComObject(pGraphBuilder);
            pGraphBuilder = null;
            Marshal.ReleaseComObject(pMediaControl);
            pMediaControl = null;
            Marshal.ReleaseComObject(pVideoWindow);
            pVideoWindow = null;
            Marshal.ReleaseComObject(pMediaPosition);
            pMediaPosition = null;
            //this.Close(); //close this program
        }
        #endregion
        private void pCapture()
        {
            //IVideoFrameStep test;
            int bufSize = 0;
            IntPtr imgData;
            pSampleGrabber.GetCurrentBuffer(ref bufSize, IntPtr.Zero);
            if(bufSize < 1)
            {
                textBox1.Text = "Get(bufSize) Error";
                return;
            }
            imgData = Marshal.AllocCoTaskMem(bufSize);
            pSampleGrabber.GetCurrentBuffer(ref bufSize, imgData);

            saveToJpg(imgData, bufSize, Video_Height, Video_Width);
            Marshal.FreeCoTaskMem(imgData);
        }
        private void saveToJpg(IntPtr Source, int Size, int height, int width)
        {
            int stride = -3 * width;
            IntPtr Scan0 = (IntPtr)(((int)Source) + (Size - (3 * width)));
            Bitmap img = new Bitmap(width, height, stride, System.Drawing.Imaging.PixelFormat.Format24bppRgb, Scan0);
            Bitmap croppedImage = img.Clone(new Rectangle(30,40,100,200), img.PixelFormat);
            croppedImage.Save("test.jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
            img.Dispose();
        }
    }
}
