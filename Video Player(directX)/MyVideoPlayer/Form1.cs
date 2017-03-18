using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.DirectX.AudioVideoPlayback;
using System.IO;
//using OpenCvSharp;

namespace MyVideoPlayer
{
    public partial class Form1 : Form
    {
        //for your info, this only works on x86 projects
        //due to the library itself

        private Video video;
        private string[] videoPaths;
        private string folderPath = @"C:\Users\trevor\Desktop\Videos\";
        private int selectedIndex = 0;
        private Size formSize;
        private Size pnlSize;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            formSize = new Size(this.Width, this.Height);
            pnlSize = new Size(pnlVideo.Width, pnlVideo.Height);

            videoPaths = Directory.GetFiles(folderPath, "*.avi");

            if (videoPaths != null)
            {
                foreach (string path in videoPaths)
                {
                    string vid = path.Replace(folderPath, string.Empty);
                    vid = vid.Replace(".avi", string.Empty);
                    lstVideos.Items.Add(vid);
                }
            }
            lstVideos.SelectedIndex = selectedIndex;
        }

        private void lstVideos_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                video.Stop();
                video.Dispose();
            }
            catch { }

            int index = lstVideos.SelectedIndex;
            selectedIndex = index;
            video = new Video(videoPaths[index], false);
            video.Owner = pnlVideo;
            pnlVideo.Size = pnlSize;
            //video.Play();
            tmrVideo.Enabled = true;
            btnPlayPause.Text = "Pause";
            video.Ending += Video_Ending;
            lblVideo.Text = lstVideos.Text;
        }

        private void Video_Ending(object sender, EventArgs e)
        {
            Task.Factory.StartNew(() =>
            {
                System.Threading.Thread.Sleep(2000);

                if (InvokeRequired)
                {
                    this.Invoke(new Action(() =>
                    {
                        NextVideo();
                    }));
                }
            });
        }

        private void NextVideo()
        {
            int index = lstVideos.SelectedIndex;
            index++;
            if (index > videoPaths.Length - 1)
                index = 0;
            selectedIndex = index;
            lstVideos.SelectedIndex = index;
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            NextVideo();
        }

        private void btnPrevious_Click(object sender, EventArgs e)
        {
            PreviousVideo();
        }

        private void PreviousVideo()
        {
            int index = lstVideos.SelectedIndex;
            index--;
            if (index == -1)
                index = videoPaths.Length - 1;
            selectedIndex = index;
            lstVideos.SelectedIndex = index;
        }

        private void btnPlayPause_Click(object sender, EventArgs e)
        {
            if (!video.Playing)
            {
                //test
                OpenFileDialog dlogOpen = new OpenFileDialog();
                dlogOpen.Filter = "all|*.*";

                if (dlogOpen.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                    return;

                var capture = new OpenCvSharp.VideoCapture(dlogOpen.FileName);
                int sleepTime = (int)Math.Round(1000 / capture.Fps);
                
                using (var window = new OpenCvSharp.Window("capture"))
                {
                    OpenCvSharp.Mat image = new OpenCvSharp.Mat();
                    while(true)
                    {
                        capture.Read(image);
                        if (image.Empty())
                            break;
                        
                        OpenCvSharp.Cv2.ImShow("ttt", image);
                        OpenCvSharp.Cv2.WaitKey(sleepTime);
                        //OpenCvSharp.Window.ShowImages(image);
                        //OpenCvSharp.Window.WaitKey(sleepTime);
                    }
                }

                // Save the bitmap
                //bitimg.Save(@"C:\Users\trevor\Desktop\Videos\bitmap.png");


                //test
                
                
                //video.Play();
                //tmrVideo.Enabled = true;
                //btnPlayPause.Text = "Pause";
            }
            else if (video.Playing)
            {
                video.Pause();
                tmrVideo.Enabled = false;
                btnPlayPause.Text = "Play";
            }
        }

        public static Bitmap MatToBitmap(OpenCvSharp.Mat image)
        {
            return OpenCvSharp.Extensions.BitmapConverter.ToBitmap(image);
        } 

        private void btnFullscreen_Click(object sender, EventArgs e)
        {
            FormBorderStyle = FormBorderStyle.None;
            WindowState = FormWindowState.Maximized;
            video.Owner = this;
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                //exit full screen when escape is pressed
                FormBorderStyle = FormBorderStyle.Sizable;
                WindowState = FormWindowState.Normal;
                this.Size = formSize;
                video.Owner = pnlVideo;
                pnlVideo.Size = pnlSize;
            }
        }

        private void trackVolume_Scroll(object sender, EventArgs e)
        {
            video.Audio.Volume = trackVolume.Value;
        }

        private void btnVolume_Click(object sender, EventArgs e)
        {
            trackVolume.Visible = !trackVolume.Visible;
        }

        private void tmrVideo_Tick(object sender, EventArgs e)
        {
            int currentTime = Convert.ToInt32(video.CurrentPosition);
            int maxTime = Convert.ToInt32(video.Duration);

            lblVideoPosition.Text = string.Format("{0:00}:{1:00}:{2:00}", currentTime / 3600, (currentTime / 60) % 60, currentTime % 60)
                                    + " / " +
                                    string.Format("{0:00}:{1:00}:{2:00}", maxTime / 3600, (maxTime / 60) % 60, maxTime % 60);
        }
    }
}
