using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinFormProject
{
    public partial class Form1 : Form
    {
        
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //open jpg file as Bitmap
            Bitmap img = (Bitmap)Bitmap.FromFile(@"C:\test.jpg");

            ClassLibrary1.MyOpenCvWrapper obj = new ClassLibrary1.MyOpenCvWrapper();
            Bitmap output = obj.ApplyFilter(img);//call opencv functions and get filterred image

            output.Save("test1.jpg");//save processed image
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //allow user to open jpg file 
            OpenFileDialog dlogOpen = new OpenFileDialog();
            dlogOpen.Filter = "Jpg Files|*.jpg|MP4 Files|*.mp4|AVI Files|*.avi";

            if (dlogOpen.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                return;

            //open jpg file as Bitmap
            Bitmap img = (Bitmap)Bitmap.FromFile(dlogOpen.FileName);    

            pbSrcImg.Image = img;//set picture box image to UI

            ClassLibrary1.MyOpenCvWrapper processor = new ClassLibrary1.MyOpenCvWrapper();
            Bitmap processedImg = processor.ApplyFilter(img);//call opencv functions and get filterred image

            pbDstImage.Image = processedImg;//set processed image to picture box
        }

        private void button3_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlogOpen = new OpenFileDialog(); //avi
            if (dlogOpen.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                return;
            
            var capture = new OpenCvSharp.VideoCapture(dlogOpen.FileName);
            int sleepTime = (int)Math.Round(1000 / capture.Fps);

            using (var window = new OpenCvSharp.Window("capture"))
            {
                OpenCvSharp.Mat image = new OpenCvSharp.Mat();
                ClassLibrary1.MyOpenCvWrapper processor = new ClassLibrary1.MyOpenCvWrapper();
                while (true)
                {
                    capture.Read(image);
                    if (image.Empty() | (OpenCvSharp.Cv2.WaitKey(33)==27))
                        break;
                    Bitmap img = MatToBitmap(image);
                    
                    Bitmap processedImg = processor.ApplyFilter(img);
                    OpenCvSharp.Extensions.BitmapConverter.ToMat(processedImg, image);
                    //pictureBoxIpl.Image = processedImg;
                    OpenCvSharp.Cv2.ImShow("capture", image);
                    OpenCvSharp.Cv2.WaitKey(sleepTime);
                }
            }
        }
        public static Bitmap MatToBitmap(OpenCvSharp.Mat image)
        {
            return OpenCvSharp.Extensions.BitmapConverter.ToBitmap(image);
        }
    }
}
