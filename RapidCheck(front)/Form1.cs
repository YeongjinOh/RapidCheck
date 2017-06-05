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
using AxWMPLib; //player
using Shell32;
namespace RapidCheck
{
    public partial class Form1 : MaterialForm
    {
        private Size formSize;
        private Size pnlSize;
        //검색 조건 (방향, 색상)
        private int inputDirection = -1;
        private int color = -1;
        RapidCheck.OverlayVideo rapidCheck;
        public Form1()
        {
            InitializeComponent();
            var skinManager = MaterialSkinManager.Instance;
            skinManager.AddFormToManage(this);
            //skinManager.Theme = MaterialSkinManager.Themes.DARK;
            skinManager.Theme = MaterialSkinManager.Themes.LIGHT;
            skinManager.ColorScheme = new ColorScheme(Primary.BlueGrey800, Primary.BlueGrey900, Primary.BlueGrey500, Accent.LightBlue200, TextShade.WHITE);
            
            tabPage1.Text = "분석";
            tabPage2.Text = "영상";
            tabPage3.Text = "";

            
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            formSize = new Size(this.Width, this.Height);
            pnlSize = new Size(tabPage3.Width, tabPage3.Height);

            //color button
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

            //progress bar
            progressBar1.Minimum = 0;
            progressBar1.Maximum = 100;
            progressBar1.Step = progressBar1.Maximum / 7;
            progressBar1.Style = ProgressBarStyle.Continuous;
            progressBar1.Enabled = true;
            
            //video player
            MaximizeBox = false;

            //radioBtn speed
            radioButtonX1.Enabled = false;
            radioButtonX2.Enabled = false;
            radioButtonX4.Enabled = false;

            //trackBar
            trackBar1.Enabled = false;

            //dataGridView
            dataGridView1.Columns[0].Width = panelObject.Width / 2;
            dataGridView1.Columns[1].Width = panelObject.Width / 2;
            dataGridView1.Columns[1].DefaultCellStyle.WrapMode = DataGridViewTriState.True; //support multiline text
            dataGridView1.ColumnHeadersVisible = false;

            //draw time on/off
            radioButtonTimeOn.Enabled = false;
            radioButtonTimeOff.Enabled = false;
        }

        string videoPath = null;
        int outputFrameNum = 0;
        OpenFileDialog videoFilePath;
        private void VideoBtn_Click(object sender, EventArgs e)
        {
            videoFilePath = new OpenFileDialog();
            videoFilePath.Filter = "All Files (*.*)|*.*";
            videoFilePath.FilterIndex = 1;
            videoFilePath.Multiselect = true;
            videoFilePath.InitialDirectory = @"C:\videos";
            if (videoFilePath.ShowDialog() == DialogResult.OK)
            {
                videoPath = videoFilePath.FileName;
            }
            string createTime = setCreateTime(System.IO.Path.GetDirectoryName(videoFilePath.FileName), System.IO.Path.GetFileName(videoFilePath.FileName));
            int maxFrameNum = 10000;
            int frameStep = 3;
            int minTrackingLength = 17;
            int clusterNum = 6;
            outputFrameNum = 500;
            rapidCheck = new RapidCheck.OverlayVideo(dataGridView1, startBtn, trackBar1, pictureBoxVideo, videoPath, createTime, maxFrameNum, frameStep, minTrackingLength, clusterNum, outputFrameNum); //ObjList setting

            //trackbar
            trackBar1.Minimum = 0;
            trackBar1.Maximum = outputFrameNum - 1;
            new Thread(() => rapidRun(ref rapidCheck)).Start();
            
        }
        delegate void rapidModule();
        delegate void rapidChain(ref System.Diagnostics.Stopwatch sw, string state, rapidModule dele);
        private void basicFlow(ref System.Diagnostics.Stopwatch sw, string state, rapidModule dele)
        {
            sw.Start();
            dele();
            sw.Stop();
            //timeLabel.Text + "\noverlay : " + sw.ElapsedMilliseconds.ToString() + "ms";
            sw.Reset();
            progressBar1.PerformStep();
        }
        private void rapidRun(ref RapidCheck.OverlayVideo rapid)
        {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
           
            List<string> states = new List<string>();
            states.Add("MySQL");
            states.Add("Obj List");
            states.Add("Clustering");
            states.Add("Image cropping");
            states.Add("Id filtering");
            states.Add("Id ordering");
            states.Add("play");

            List<rapidModule> myRapidModule = new List<rapidModule>();
            myRapidModule.Add(rapid.getMysqlObjList);
            myRapidModule.Add(rapid.addObj);
            myRapidModule.Add(rapid.kMeasFunc);
            myRapidModule.Add(rapid.imageCrop);
//            myRapidModule.Add(rapid.setObjidList);
            myRapidModule.Add(rapid.buildOverlayOrderUsingCluster);
            //myRapidModule.Add(rapid.overlay);
            trackBar1.Enabled = true;
            radioButtonX1.Enabled = true;
            radioButtonX2.Enabled = true;
            radioButtonX4.Enabled = true;
            radioButtonX1.Checked = true;
            radioButtonTimeOn.Enabled = true;
            radioButtonTimeOff.Enabled = true;
            myRapidModule.Add(rapid.overlayLive);
            rapidChain myRapidChain = new rapidChain(basicFlow);
            for (int idx = 0; idx < myRapidModule.Count; idx++)
            {
                myRapidChain(ref sw, states[idx], myRapidModule[idx]);
            }
            progressBar1.Value = 100;
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            //    if(worker != null)
            //    {
            //        worker.Abort();
            //        worker = new Thread(Run);
            //        worker.Start();
            //    }
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            rapidCheck.resFrame = trackBar1.Value;
            rapidCheck.overlayObjIdx = 0;
        }
        private void VideoStartBtn_Click(object sender, EventArgs e)
        {
            if(startBtn.Text == "Start" )
            {
                startBtn.Text = "Pause";
            }
            else
            {
                startBtn.Text = "Start";
            }
        }

        private void trackBar1_MouseDown(object sender, MouseEventArgs e)
        {
            trackBar1.Value = Convert.ToInt32(1.0 * outputFrameNum * e.Location.X / trackBar1.Width);
        }

        private void pictureBoxVideo_MouseDown(object sender, MouseEventArgs e)
        {
            if(trackBar1.Enabled == true)
            {
                int fps = 20;
                startBtn.Text = "Start";
                Point clickPosition = e.Location;
                double clickPositionOriginX = (double)clickPosition.X / pictureBoxVideo.Width * rapidCheck.videoWidth;
                double clickPositionOriginY = (double)clickPosition.Y / pictureBoxVideo.Height * rapidCheck.videoHeight;
                int frameNum = rapidCheck.getClickedObjectOriginalFrameNum(clickPositionOriginX, clickPositionOriginY);
                if (frameNum >= 0)
                {
                    materialTabControl1.SelectedTab = tabPage3;
                    axWindowsMediaPlayer1.URL = videoFilePath.FileName;
                    axWindowsMediaPlayer1.Ctlcontrols.play();
                    axWindowsMediaPlayer1.Ctlcontrols.currentPosition = (double)frameNum / fps;
                }
            }
        }
        private void radioButtonX1_CheckedChanged(object sender, EventArgs e) { rapidCheck.setSpeed(1); }
        private void radioButtonX2_CheckedChanged(object sender, EventArgs e) { rapidCheck.setSpeed(2); }
        private void radioButtonX4_CheckedChanged(object sender, EventArgs e) { rapidCheck.setSpeed(4); }
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
        private void direction1_Click(object sender, EventArgs e) { inputDirection = 1; }
        private void direction2_Click(object sender, EventArgs e) { inputDirection = 2; }
        private void direction3_Click(object sender, EventArgs e) { inputDirection = 3; }
        private void direction4_Click(object sender, EventArgs e) { inputDirection = 4; }
        private void direction5_Click(object sender, EventArgs e) { inputDirection = 5; }
        private void direction6_Click(object sender, EventArgs e) { inputDirection = 6; }
        private void direction7_Click(object sender, EventArgs e) { inputDirection = 7; }
        private void direction8_Click(object sender, EventArgs e) { inputDirection = 8; }
        private string setCreateTime(string folderPath, string fileName)
        {
            Shell32.Shell shell = new Shell32.Shell();
            Shell32.Folder objFolder;
            objFolder = shell.NameSpace(folderPath);
            Shell32.FolderItem fi;
            fi = objFolder.ParseName(fileName);
            string temp = objFolder.GetDetailsOf(fi, 4);
            temp = temp.Split('오')[1];
            string AmPm = temp.Split(' ')[0];
            string hour = temp.Split(' ')[1].Split(':')[0];
            string min = temp.Split(' ')[1].Split(':')[1];
            if (AmPm == "후")
            {
                int h = Int32.Parse(hour) + 12;
                if (h == 24) h = 12;
                hour = h.ToString();
            }
            else if(Int32.Parse(hour) == 12)
            {
                hour = "0";
            }
            return hour + ":" + min;
        }

        private void radioButtonTimeOn_CheckedChanged(object sender, EventArgs e)
        {
            rapidCheck.drawTime = true;
        }
        private void radioButtonTimeOff_CheckedChanged(object sender, EventArgs e)
        {
            rapidCheck.drawTime = false;
        }

        private void radioButtonBoth_CheckedChanged(object sender, EventArgs e)
        {
            rapidCheck.objType = 1;
        }
        private void radioButtonPeople_CheckedChanged(object sender, EventArgs e)
        {
            rapidCheck.objType = 2;
        }
        private void radioButtonCar_CheckedChanged(object sender, EventArgs e)
        {
            rapidCheck.objType = 3;
        }
    }
}
