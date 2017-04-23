using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;//DialogResult
//DirectX
using Microsoft.DirectX.AudioVideoPlayback;
using Microsoft.DirectX;

namespace RapidCheck
{
    public partial class Form1
    {
        //DirectX
        private Video video;

        private void VideoBtn_Click(object sender, EventArgs e)
        {
            openFileDialog1.InitialDirectory = @"C:\Users\trevor\Desktop\Videos";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                // open the video
                // remember the original dimensions of the panel
                int height = videoPanel.Height;
                int width = videoPanel.Width;
                // dispose of the old video to clean up resources
                if (video != null)
                {
                    video.Dispose();
                }
                try
                {
                    // open a new video
                    video = new Video(openFileDialog1.FileName);
                    // assign the win form control that will contain the video
                    video.Owner = videoPanel;
                    // resize to fit in the panel
                    videoPanel.Width = width;
                    videoPanel.Height = height;
                    // play the first frame of the video so we can identify it
                    video.Play();
                    video.CurrentPosition = 200 / 60;
                    //video.Pause();
                }
                catch (Exception ex)
                {
                    VideoBtn.Text = "error";
                }
            }
            // enable video buttons
            //ControlLogic();
        }
    }
}
