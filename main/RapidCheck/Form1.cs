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
//using LiveCharts;
//using LiveCharts.Wpf;
//using LiveCharts.WinForms;

using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;
using System.Threading;
using AxWMPLib; //player
using Shell32;
using CefSharp;
using CefSharp.WinForms;
using LiveCharts;
using LiveCharts.Wpf;
using LiveCharts.WinForms;

namespace RapidCheck
{
    public partial class Form1 : MaterialForm
    {
        //******************************전역변수******************************
        /*****************/string videoPath = null;                     /*****************/
        /*****************/OpenFileDialog videoFilePath;                /*****************/
        /*****************/int outputFrameNum = 0;                      /*****************/
        /*****************/RapidCheck.OverlayVideo rapidCheck;          /*****************/
        /*****************/delegate void rapidModule();                 /*****************/
        /*****************/delegate void rapidChain(rapidModule dele);  /*****************/
        /*****************/Thread overlayModule;                        /*****************/
        /*****************/List<rapidModule> myRapidModule = new List<rapidModule>();/*****************/
        string createTime;
        //******************************Form******************************
        public Form1()
        {
            InitializeComponent();
            var skinManager = MaterialSkinManager.Instance;
            skinManager.AddFormToManage(this);
            //skinManager.Theme = MaterialSkinManager.Themes.DARK;
            skinManager.Theme = MaterialSkinManager.Themes.LIGHT;
            skinManager.ColorScheme = new ColorScheme(
                Primary.Grey50, // tab contorol
                Primary.Grey50, //최상단 
                Primary.BlueGrey700,
                Accent.LightBlue400,
                TextShade.BLACK);
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            Cef.Initialize(new CefSettings()); // chrome initialize
            setUI();
        }

        //******************************Overlay Module******************************
        private void startOverlayModule()
        {
            createTime = setCreateTime(System.IO.Path.GetDirectoryName(videoFilePath.FileName), System.IO.Path.GetFileName(videoFilePath.FileName));
            int maxFrameNum = 1750;
            //int frameStep = 3;
            int analysisFPS = 5; //default
            int minTrackingLength = 21;
            int clusterNum = 10;
            outputFrameNum = 500;
            rapidCheck = new RapidCheck.OverlayVideo(labelProgress, dataGridView1, dataGridView2, startBtn, trackBar1, pictureBoxVideo, videoPath, createTime, maxFrameNum, analysisFPS, minTrackingLength, clusterNum, outputFrameNum); //ObjList setting
            
            rapidFunc();
            overlayModule = new Thread(() => rapidRun());
            overlayModule.Start();
        }
        private void basicFlow(rapidModule dele)
        {
            dele();

        }
        private void rapidFunc()
        {
            myRapidModule.Add(rapidCheck.getMysqlObjList);
            myRapidModule.Add(rapidCheck.addObj);
            myRapidModule.Add(rapidCheck.imageCrop);
            myRapidModule.Add(rapidCheck.kMeasFunc);
            myRapidModule.Add(rapidCheck.buildOverlayOrderUsingCluster);
            myRapidModule.Add(rapidCheck.setPictureBoxSize);
            myRapidModule.Add(rapidCheck.overlayLive);
        }
        private void rapidRun()
        {
            rapidChain myRapidChain = new rapidChain(basicFlow);
            for (int idx = 0; idx < myRapidModule.Count; idx++)
            {
                if (myRapidModule[idx].Method.ToString() == "Void overlayLive()" )
                {
                    setOverlayUI();
                }
                myRapidChain(myRapidModule[idx]);
            }
        }
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
                startOverlayModule();
                labelProgress.Text = "Analyzing 0%";
            }
        }
        //******************************UI SETTING******************************
        private void setUI()
        {
            //set tabkpage
            tabPage1.Text = "분석";
            tabPage2.Text = "영상";
            tabPage3.Text = "요약";
            tabPage4.Text = "라벨링";

            //video player
            MaximizeBox = false;

            //radioBtn speed
            radioButtonX1.Enabled = false;
            radioButtonX2.Enabled = false;
            radioButtonX4.Enabled = false;

            //trackBar
            trackBar1.Enabled = false;

            //dataGridView
            //dataGridView1.Columns[0].Width = panelObject.Width;
            //dataGridView1.Columns[1].Width = panelObject.Width / 2;
            //dataGridView1.Columns[1].DefaultCellStyle.WrapMode = DataGridViewTriState.True; //support multiline text

        } //default UI setting
        private void setOverlayUI()
        {
            //trackBar
            trackBar1.Minimum = 0;
            trackBar1.Maximum = outputFrameNum - 1;
            trackBar1.Value = 0;
            //enable
            trackBar1.Enabled = true;
            radioButtonX1.Enabled = true;
            radioButtonX2.Enabled = true;
            radioButtonX4.Enabled = true;
            radioButtonX1.Checked = true;
            startBtn.Enabled = true;
        } //UI enable = True
        //******************************UI EVENT******************************
        private void pictureBoxVideo_MouseDown(object sender, MouseEventArgs e) //비디오 클릭하면 원본 영상 틀어주는 함수
        {
            if(trackBar1.Enabled == true)
            {
                int fps = rapidCheck.fps;
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
        private string setCreateTime(string folderPath, string fileName) //파일에서 시간 가져오는 함수
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
        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            rapidCheck.resFrame = trackBar1.Value;
            rapidCheck.overlayObjIdx = 0;
        }
        
        private void startBtn_Click(object sender, EventArgs e)
        {
            if (startBtn.Text == "Start") { startBtn.Text = "Pause"; }
            else { startBtn.Text = "Start"; }
        }
        private void trackBar1_MouseDown(object sender, MouseEventArgs e)
        {
            trackBar1.Value = Convert.ToInt32(1.0 * outputFrameNum * e.Location.X / trackBar1.Width);
        }
        //******************************BUTTON EVENT******************************
       
        private void radioButtonBoth_CheckedChanged(object sender, EventArgs e) { rapidCheck.objType = 1; }
        private void radioButtonPeople_CheckedChanged(object sender, EventArgs e) { rapidCheck.objType = 2; }
        private void radioButtonCar_CheckedChanged(object sender, EventArgs e) { rapidCheck.objType = 3; }
        private void radioButtonX1_CheckedChanged(object sender, EventArgs e) { rapidCheck.speed = 1; }
        private void radioButtonX2_CheckedChanged(object sender, EventArgs e) { rapidCheck.speed = 2; }
        private void radioButtonX4_CheckedChanged(object sender, EventArgs e) { rapidCheck.speed = 4; }

        private void onToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.drawTime = true;
        }
        private void offToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.drawTime = false;
        }

        //------------------------------검색 조건------------------------------
        //방향
        private void DownToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and direction0 + direction1 + direction2  > 0.7";
            replay();
        }

        private void UpToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and direction5 + direction6 + direction7 > 0.7";
            replay();
        }

        private void replay()
        {
            overlayModule.Abort();
            Thread.Sleep(1);
            myRapidModule.Clear();
            myRapidModule.Add(rapidCheck.setFileterObjectidList); //filetering test
            myRapidModule.Add(rapidCheck.kMeasFunc);
            myRapidModule.Add(rapidCheck.buildOverlayOrderUsingCluster);
            myRapidModule.Add(rapidCheck.overlayLive);

            overlayModule = new Thread(() => rapidRun());
            overlayModule.Start();
        }
        //색상
        private void BlackToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color9 > 0.3";
            replay();
        }
        
        //초기화
        private void ResetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            overlayModule.Abort();
            Thread.Sleep(1);
            myRapidModule.Clear();
            myRapidModule.Add(rapidCheck.resetObjectidList);
            myRapidModule.Add(rapidCheck.kMeasFunc);
            myRapidModule.Add(rapidCheck.buildOverlayOrderUsingCluster);
            myRapidModule.Add(rapidCheck.overlayLive);
            overlayModule = new Thread(() => rapidRun());
            overlayModule.Start();
        }
        

        private void color0ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color0 + color15 > 0.1";
            replay();
        }
        
        private void color1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color1 > 0.1";
            replay();
        }
        
        private void color2ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color2 + color3 > 0.1";
            replay();
        }
        
        private void color3ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color3 + color4 + color5 > 0.1";
            replay();
        }
        
        private void color4ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color6 + color7 > 0.1";
            replay();
        }
        
        private void color5ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color7 + color8 + color9 + color10 + color11> 0.2";
            replay();
        }
        
        private void color6ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color12 + color13 > 0.1";
            replay();
        }
        
        private void color7ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color13 + color14 > 0.1";
            replay();
        }
        
        private void color8ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color16 > 0.6 and color17 > 0.15";
            replay();
        }
        
        private void color9ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color16 > 0.6 and color18 > 0.3";
            replay();
        }


        //------------------------------tab page control------------------------------
        public ChromiumWebBrowser browser;
        private void materialTabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = materialTabControl1.SelectedIndex;
            if (idx ==3)
            {
                browser = new ChromiumWebBrowser("http://www.rapidcheck.co.kr:5000")
                {
                    Dock = DockStyle.Fill,
                    Size = Size,
                };
                panelWebbrowser.Controls.Add(browser);
                browser.Size = new Size(panelWebbrowser.Size.Width, panelWebbrowser.Size.Height);
            }
            else if( idx == 2)
            {
                //chart
            }
        }

        private void dataGridView2_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            //e.RowIndex;
            int id = rapidCheck.gridViewList2[e.RowIndex];
            DateTime startTime = rapidCheck.getClickedGridViewOriginalDataTime(id);

            int hour = Convert.ToInt32(createTime.Split(':')[0]);
            int min = Convert.ToInt32(createTime.Split(':')[1]);
            hour = startTime.Hour - hour;
            min = startTime.Minute - min;
            int sec = startTime.Second;
            int position = (hour * 60 + min)*60 + sec;

            if(startTime != null)
            {
                materialTabControl1.SelectedTab = tabPage3;
                axWindowsMediaPlayer1.URL = videoFilePath.FileName;
                axWindowsMediaPlayer1.Ctlcontrols.play();
                axWindowsMediaPlayer1.Ctlcontrols.currentPosition = (double)position;
            }
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //e.RowIndex;
            int id = rapidCheck.gridViewList1[e.RowIndex];
            DateTime startTime = rapidCheck.getClickedGridViewOriginalDataTime(id);

            int hour = Convert.ToInt32(createTime.Split(':')[0]);
            int min = Convert.ToInt32(createTime.Split(':')[1]);
            hour = startTime.Hour - hour;
            min = startTime.Minute - min;
            int sec = startTime.Second;
            int position = (hour * 60 + min) * 60 + sec;

            if (startTime != null)
            {
                materialTabControl1.SelectedTab = tabPage3;
                axWindowsMediaPlayer1.URL = videoFilePath.FileName;
                axWindowsMediaPlayer1.Ctlcontrols.play();
                axWindowsMediaPlayer1.Ctlcontrols.currentPosition = (double)position;
            }
        }

        private void panelFile_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panelTarget_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBoxCar_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void materialLabelTrackingbar_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void panelDensity_Paint(object sender, PaintEventArgs e)
        {

        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {

        }

        private void panelDirection_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox8_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox11_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox10_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox9_Click(object sender, EventArgs e)
        {

        }

        private void panelColor_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panelCondition_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {

        }

        private void panelVideo_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
