using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;
using System.Windows.Forms;

//DirectShowLib;
using DirectShowLib;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;

//ffmpeg
using AForge;
using Accord.Video.FFMPEG;

namespace RapidCheck
{
    public partial class Form1
    {
        private void button1_Click(object sender, EventArgs e) //Frame btn
        {
            FFMPEG_TEST();
        }
        public void FFMPEG_TEST()
        {
            // create instance of video reader
            Accord.Video.FFMPEG.VideoFileReader reader = new Accord.Video.FFMPEG.VideoFileReader();
            // open video file
            reader.Open(@"C:\Users\trevor\Desktop\Videos\cctv2.wmv");

            Bitmap[] objBitmap = new Bitmap[100];
            // read 100 video frames out of it
            for (int i = 0; i < 100; i++)
            {
                Bitmap videoFrame = reader.ReadVideoFrame();

                //corp
                objBitmap[i] = cropImage(videoFrame, new Rectangle(10, 10, 200, 200));

                //videoFrame.Save(i + ".bmp");
                objBitmap[i].Save(@"C:\Users\trevor\Desktop\Videos\obj\" + i + ".bmp");
                // dispose the frame when it is no longer required
                videoFrame.Dispose();
            }
            reader.Close();
        }
        private static Bitmap cropImage(Bitmap img, Rectangle cropArea)
        {
            return img.Clone(cropArea, img.PixelFormat);
        }
    }
}
