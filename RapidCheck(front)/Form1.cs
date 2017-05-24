using MaterialSkin;
using MaterialSkin.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;
using System.Threading;
//using SharpDX.MediaFoundation;

namespace RapidCheck
{
    public partial class Form1 : MaterialForm
    {
        private Size formSize;
        private Size pnlSize;

        //검색 조건 (방향, 색상)
        private int inputDirection = -1;
        private int color = -1;
        public Form1()
        {
            InitializeComponent();
            var skinManager = MaterialSkinManager.Instance;
            skinManager.AddFormToManage(this);
            //skinManager.Theme = MaterialSkinManager.Themes.DARK;
            skinManager.Theme = MaterialSkinManager.Themes.LIGHT;
            skinManager.ColorScheme = new ColorScheme(Primary.BlueGrey800, Primary.BlueGrey900, Primary.BlueGrey500, Accent.LightBlue200, TextShade.WHITE);
            
            tabPage1.Text = "검색";
            tabPage2.Text = "요약";
            tabPage3.Text = "영상";
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            formSize = new Size(this.Width, this.Height);
            pnlSize = new Size(tabPage3.Width, tabPage3.Height);

            radioButton1.Text = "빨간색";
            radioButton2.Text = "노란색";
            radioButton3.Text = "초록색";
            radioButton4.Text = "하늘색";
            radioButton5.Text = "파란색";
            radioButton6.Text = "남색";
            radioButton7.Text = "보라색";
            radioButton8.Text = "자주색";
            radioButton9.Text = "검은색";
            radioButton10.Text = "흰색";
        }

        private void direction1_Click(object sender, EventArgs e) { inputDirection = 1; }
        private void direction2_Click(object sender, EventArgs e) { inputDirection = 2; }
        private void direction3_Click(object sender, EventArgs e) { inputDirection = 3; }
        private void direction4_Click(object sender, EventArgs e) { inputDirection = 4; }
        private void direction5_Click(object sender, EventArgs e) { inputDirection = 5; }
        private void direction6_Click(object sender, EventArgs e) { inputDirection = 6; }
        private void direction7_Click(object sender, EventArgs e) { inputDirection = 7; }
        private void direction8_Click(object sender, EventArgs e) { inputDirection = 8; }

        string videoPath = null;
        Thread worker;
        private void VideoBtn_Click(object sender, EventArgs e)
        {
            //OpenFileDialog choofdlog = new OpenFileDialog();
            //choofdlog.Filter = "All Files (*.*)|*.*";
            //choofdlog.FilterIndex = 1;
            //choofdlog.Multiselect = true;
            //choofdlog.InitialDirectory = @"C:\videos";

            //if (choofdlog.ShowDialog() == DialogResult.OK)
            //{
            //    videoPath = choofdlog.FileName;
            //}
            videoPath = @"C:\videos\tracking.mp4";
            RapidCheck.OverlayVideo test = new RapidCheck.OverlayVideo(videoPath);
            /////
            //worker = new Thread(Run);
            //worker.Start();
        }
        private void Run()
        {
            Graphics gs = tabPage2.CreateGraphics();
            for (int i = 0; i < 66; i++)
            {
                System.Threading.Thread.Sleep(500);
                //if (i >= 65) { i = 0; }
                string filePath = @"C:\videos\66장 여기\" + i + ".bmp";
                Bitmap back = new Bitmap(filePath);
                gs.DrawImage(back, new Point(0, 0));
            }
        }
        private void radioButton1_CheckedChanged(object sender, EventArgs e) { color = 0; }
        private void radioButton2_CheckedChanged(object sender, EventArgs e) { color = 1; }
        private void radioButton3_CheckedChanged(object sender, EventArgs e) { color = 2; }
        private void radioButton4_CheckedChanged(object sender, EventArgs e) { color = 3; }
        private void radioButton5_CheckedChanged(object sender, EventArgs e) { color = 4; }
        private void radioButton6_CheckedChanged(object sender, EventArgs e) { color = 5; }
        private void radioButton7_CheckedChanged(object sender, EventArgs e) { color = 6; }
        private void radioButton8_CheckedChanged(object sender, EventArgs e) { color = 7; }
        private void radioButton9_CheckedChanged(object sender, EventArgs e) { color = 8; }
        private void radioButton10_CheckedChanged(object sender, EventArgs e) { color = 9; }

        private void Form1_Resize(object sender, EventArgs e)
        {
            if(worker != null)
            {
                worker.Abort();
                worker = new Thread(Run);
                worker.Start();
            }
        }
    }
}
