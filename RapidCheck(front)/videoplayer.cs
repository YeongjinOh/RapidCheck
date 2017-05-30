using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.DirectX.AudioVideoPlayback;
using System.ComponentModel;

namespace RapidCheck
{
    public partial class Form1
    {
        Video vdo;
        public string mode = "play";
        public string PlayingPosition, Duration;

        private void timer1_Tick(object sender, EventArgs e)
        {
            PlayingPosition = CalculateTime(vdo.CurrentPosition);
            txtStatus.Text = PlayingPosition + "/" + Duration;

            if (vdo.CurrentPosition >= vdo.Duration)
            {
                timer1.Stop();
                Duration = CalculateTime(vdo.Duration);
                PlayingPosition = "0:00:00";
                txtStatus.Text = PlayingPosition + "/" + Duration;
                vdo.Stop();
                //btnPlay.BackgroundImage = RapidCheck.Properties.Resources.btnplay;
                vdoTrackBar.Value = 0;
            }
            else
                vdoTrackBar.Value += 1;
        }
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (vdo != null)
            {
                vdo.Stop();
                timer1.Stop();
                //btnPlay.BackgroundImage = RapidCheck.Properties.Resources.btnplay;
                vdoTrackBar.Value = 0;
            }

            System.Windows.Forms.OpenFileDialog openFileDialog = new System.Windows.Forms.OpenFileDialog();
            openFileDialog.ShowDialog();
            openFileDialog1.Title = "Select video file..";
            openFileDialog1.InitialDirectory = System.Windows.Forms.Application.StartupPath;
            openFileDialog1.DefaultExt = ".avi";
            openFileDialog.Filter = "Media Files|*.mpg;*.avi;*.wma;*.mov;*.wav;*.mp2;*.mp3|All Files|*.*";
            if (openFileDialog1.FileName != "")
            {
                Form1.ActiveForm.Text = openFileDialog.FileName + " - Anand Media Player";
                vdo = new Video(openFileDialog.FileName);

                vdo.Owner = pictureBox2;
                Duration = CalculateTime(vdo.Duration);
                PlayingPosition = "0:00:00";
                txtStatus.Text = PlayingPosition + "/" + Duration;

                vdoTrackBar.Minimum = 0;
                vdoTrackBar.Maximum = Convert.ToInt32(vdo.Duration);
            }
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {

            if (vdo != null)
            {
                if (vdo.Playing)
                {
                    vdo.Pause();
                    timer1.Stop();
                    //btnPlay.BackgroundImage = RapidCheck.Properties.Resources.btnplay;
                }
                else
                {
                    vdo.Play();
                    timer1.Start();

                    //btnPlay.BackgroundImage = RapidCheck.Properties.Resources.pause;
                }
            }

        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            vdo.Stop();
            timer1.Stop();
            //btnPlay.BackgroundImage = RapidCheck.Properties.Resources.btnplay;
            vdoTrackBar.Value = 0;
        }
        public string CalculateTime(double Time)
        {
            string mm, ss, CalculatedTime;
            int h, m, s, T;

            Time = Math.Round(Time);
            T = Convert.ToInt32(Time);

            h = (T / 3600);
            T = T % 3600;
            m = (T / 60);
            s = T % 60;

            if (m < 10)
                mm = string.Format("0{0}", m);
            else
                mm = m.ToString();
            if (s < 10)
                ss = string.Format("0{0}", s);
            else
                ss = s.ToString();

            CalculatedTime = string.Format("{0}:{1}:{2}", h, mm, ss);

            return CalculatedTime;
        }
        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            Duration = CalculateTime(vdo.Duration);
            PlayingPosition = "0:00:00";
            txtStatus.Text = PlayingPosition + "/" + Duration;
        }

        private void vdoTrackBar_Scroll(object sender, EventArgs e)
        {
            if (vdo != null)
            {
                vdo.CurrentPosition = vdoTrackBar.Value;
            }
        }
        //private void exitToolItem_Click(object sender, EventArgs e)
        //{
        //    Application.Exit();
        //}

    }
}
