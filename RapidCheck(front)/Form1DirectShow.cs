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

namespace RapidCheck
{
    public partial class Form1
    {
        DirectShowLib.IGraphBuilder pGraphBuilder = null; //DirectShow Graph
        DirectShowLib.IMediaControl pMediaControl = null; //DirectShow Control
        DirectShowLib.IMediaEvent pMediaEvent = null; //DirectShow Event
        EventCode eventCode;
        DirectShowLib.IVideoWindow pVideoWindow = null;

        private void Video2Btn_Click(object sender, EventArgs e)
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
            
            switch(eventCode)
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
    }
}
