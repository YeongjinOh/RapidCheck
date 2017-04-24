using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;

//directShow
using DirectShowLib;

namespace RapidCheck
{
    public partial class Form1
    {
        DirectShowLib.IGraphBuilder pGraphBuilder;
        DirectShowLib.IMediaControl pMediaControl;
        DirectShowLib.IVideoWindow pVideoWindow;

        private void Video2Btn_Click(object sender, EventArgs e)
        {
            pGraphBuilder = new FilterGraph() as DirectShowLib.IGraphBuilder;
            pMediaControl = pGraphBuilder as DirectShowLib.IMediaControl;

            pVideoWindow = pGraphBuilder as DirectShowLib.IVideoWindow;

            string filePath = @"C:\Users\trevor\Desktop\Videos\cctv2.wmv";
            pGraphBuilder.RenderFile(filePath, null);

            pVideoWindow.put_Owner(tabPage2.Handle);
            pVideoWindow.put_WindowStyle(WindowStyle.Child | WindowStyle.ClipSiblings);
            pVideoWindow.SetWindowPosition(0, 0, tabPage2.Width, tabPage2.Height);
            pVideoWindow.put_MessageDrain(tabPage2.Handle);
            pVideoWindow.put_Visible(OABool.True);

            pMediaControl.Run();
            pMediaControl.RenderFile(@"C:\Users\trevor\Desktop\Videos\overlay.png");
        }
        protected override void OnClosed(EventArgs e)
        {
            if (pGraphBuilder != null)
            {
                Marshal.ReleaseComObject(pGraphBuilder);
            }

            if (pMediaControl != null)
            {
                Marshal.ReleaseComObject(pMediaControl);
            }

            base.OnClosed(e);
        }
    }
}
