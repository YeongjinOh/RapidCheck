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
//using AxWMPLib; //player
using Shell32;
using CefSharp;
using CefSharp.WinForms;

namespace RapidCheck
{
    public partial class Form1 : MaterialForm
    {
        //***************전역변수****************************************/
        /**/string videoPath = null;                                  /**/
        /**/OpenFileDialog videoFilePath;                             /**/
        /**/int outputFrameNum = 0;                                   /**/
        /**/RapidCheck.OverlayVideo rapidCheck;                       /**/
        /**/delegate void rapidModule();                              /**/
        /**/delegate void rapidChain(rapidModule dele);               /**/
        /**/Thread overlayModule;                                     /**/
        /**/List<rapidModule> myRapidModule = new List<rapidModule>();/**/
        /**/string createTime;                                        /**/
        //***************전역변수****************************************/

        //------------------------------Form------------------------------
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
            setUI(); //ui enable false
            defaultColor();
        }
        //------------------------------Overlay Module------------------------------
        private void startOverlayModule()
        {
            createTime = setCreateTime(System.IO.Path.GetDirectoryName(videoFilePath.FileName), System.IO.Path.GetFileName(videoFilePath.FileName));
            int maxFrameNum = 1000;
            int analysisFPS = 5; //default
            int minTrackingLength = 21;
            int clusterNum = 10;
            outputFrameNum = 500;
            rapidCheck = new RapidCheck.OverlayVideo(labelProgress, dataGridView1, dataGridView2, startBtn, trackBar1, pictureBoxVideo, videoPath, createTime, maxFrameNum, analysisFPS, minTrackingLength, clusterNum, outputFrameNum); //ObjList setting
            rapidFunc();
            overlayModule = new Thread(() => rapidRun());
            overlayModule.Start();
        }
        private void basicFlow(rapidModule dele) { dele(); }
        private void rapidFunc()
        {
            myRapidModule.Add(rapidCheck.getMysqlObjList);
            myRapidModule.Add(rapidCheck.addObj);
            myRapidModule.Add(rapidCheck.imageCrop);
            myRapidModule.Add(rapidCheck.objectClustering);
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
            else if (Int32.Parse(hour) == 12)
            {
                hour = "0";
            }
            return hour + ":" + min;
        }
        //------------------------------UI SETTING------------------------------
        private void defaultColor()
        {
            Color background = Color.FromArgb(35, 144, 182);
            panelVideo.BackColor = background;
            panelCondition.BackColor = background;
            panelObject.BackColor = background;

            Color module = Color.FromArgb(171, 219, 248);
            panelConditionModule.BackColor = module;
            panelVideoControl.BackColor = module;
            trackBar1.BackColor = module;
            panelLog.BackColor = module;
            dataGridView1.BackgroundColor = module;
            dataGridView2.BackgroundColor = module;

            Color conditionModule = Color.FromArgb(64, 127, 149);
            panelColor.BackColor = conditionModule;
            panelDensity.BackColor = conditionModule;
            panelDirection.BackColor = conditionModule;
            panelFile.BackColor = conditionModule;
            panelTarget.BackColor = conditionModule;

            Color conditionModuleTable = Color.FromArgb(105, 180, 203);
            panelDensityTable.BackColor = conditionModuleTable;
            panelDirectionTable.BackColor = conditionModuleTable;
            panelColorTable.BackColor = conditionModuleTable;
            pictureBoxTargetCar.BackColor = conditionModuleTable;
            pictureBoxTargetPeople.BackColor = conditionModuleTable;
        }
        private void setUI() //default UI setting
        {
            //set tabkpage
            tabPage1.Text = "분석";
            tabPage2.Text = "영상";
            tabPage3.Text = "통계";
            tabPage4.Text = "학습";
            //video player
            MaximizeBox = false; //최대화 단추... 표시 여부
            //speed Btn
            radioButtonX1.Enabled = false;
            radioButtonX2.Enabled = false;
            radioButtonX4.Enabled = false;
            //trackBar
            trackBar1.Enabled = false;
            //start Btn
            startBtn.Enabled = false;
            //Target Btn
            pictureBoxTargetPeople.Enabled = false;
            pictureBoxTargetCar.Enabled = false;
            //Direction Btn
            pictureBoxDirection1.Enabled = false;
            pictureBoxDirection2.Enabled = false;
            pictureBoxDirection3.Enabled = false;
            pictureBoxDirection4.Enabled = false;
            pictureBoxDirection5.Enabled = false;
            pictureBoxDirection6.Enabled = false;
            pictureBoxDirection7.Enabled = false;
            pictureBoxDirection8.Enabled = false;
            pictureBoxDirection9.Enabled = false;
            //Color Btn
            buttonColor0.Enabled = false;
            buttonColor1.Enabled = false;
            buttonColor2.Enabled = false;
            buttonColor3.Enabled = false;
            buttonColor4.Enabled = false;
            buttonColor5.Enabled = false;
            buttonColor6.Enabled = false;
            buttonColor7.Enabled = false;
            buttonColor8.Enabled = false;
            buttonColor9.Enabled = false;
        }
        private void setOverlayUI() //UI enable = True
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
            //speed Btn
            radioButtonX1.Enabled = true;
            radioButtonX2.Enabled = true;
            radioButtonX4.Enabled = true;
            //trackBar
            trackBar1.Enabled = true;
            //start Btn
            startBtn.Enabled = true;
            //Target Btn
            pictureBoxTargetPeople.Enabled = true;
            pictureBoxTargetCar.Enabled = true;
            //Direction Btn
            pictureBoxDirection1.Enabled = true;
            pictureBoxDirection2.Enabled = true;
            pictureBoxDirection3.Enabled = true;
            pictureBoxDirection4.Enabled = true;
            pictureBoxDirection5.Enabled = true;
            pictureBoxDirection6.Enabled = true;
            pictureBoxDirection7.Enabled = true;
            pictureBoxDirection8.Enabled = true;
            pictureBoxDirection9.Enabled = true;
            //Color Btn
            buttonColor0.Enabled = true;
            buttonColor1.Enabled = true;
            buttonColor2.Enabled = true;
            buttonColor3.Enabled = true;
            buttonColor4.Enabled = true;
            buttonColor5.Enabled = true;
            buttonColor6.Enabled = true;
            buttonColor7.Enabled = true;
            buttonColor8.Enabled = true;
            buttonColor9.Enabled = true;
        }
        //------------------------------Read video EVENT------------------------------
        private void buttonReadFile_Click(object sender, EventArgs e)
        {
            //if (overlayModule.IsAlive == true)
            //{
            //    overlayModule.Abort();
            //}
            if(buttonReadFile.Text == "파일")
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
                    labelProgress.Text = "Loading...";

                    //read video file -> search로 변경
                    buttonReadFile.Text = "검색";
                }
            }
            else if(buttonReadFile.Text == "검색")
            {
                //------------------------------검색조건으로 바꾸장 바꿔------------------------------
            }
            

            
        }
        //------------------------------Video controler------------------------------
        private void trackBar1_MouseDown(object sender, MouseEventArgs e)
        {
            trackBar1.Value = Convert.ToInt32(1.0 * outputFrameNum * e.Location.X / trackBar1.Width);
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
        //------------------------------Video Click EVENT------------------------------
        private void pictureBoxVideo_MouseDown(object sender, MouseEventArgs e) //비디오 클릭하면 원본 영상 틀어주는 함수
        {
            if (trackBar1.Enabled == true)
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
        //------------------------------tab page control------------------------------
        public ChromiumWebBrowser browser;
        private void materialTabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = materialTabControl1.SelectedIndex;
            if (idx ==3) // 라벨링
            {
                browser = new ChromiumWebBrowser("http://www.rapidcheck.co.kr:5000")
                {
                    Dock = DockStyle.Fill,
                    Size = Size,
                };
                panelWebbrowser.Controls.Add(browser);
                browser.Size = new Size(panelWebbrowser.Size.Width, panelWebbrowser.Size.Height);
            }
            else if( idx == 2) //통계
            {
                //chart
            }
        }
        //------------------------------DataGridView Click EVENT------------------------------
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
        //------------------------------speed Btn Click ENVET------------------------------
        private void radioButtonX1_CheckedChanged(object sender, EventArgs e) { rapidCheck.speed = 1; }
        private void radioButtonX2_CheckedChanged(object sender, EventArgs e) { rapidCheck.speed = 2; }
        private void radioButtonX4_CheckedChanged(object sender, EventArgs e) { rapidCheck.speed = 4; }
        //------------------------------Object type Click EVENT------------------------------
        private void pictureBoxTargetPeople_Click(object sender, EventArgs e) 
        {
            if (pictureBoxTargetPeople.BackColor == Color.White) //click on
            {
                pictureBoxTargetPeople.BackColor = Color.Gray;
            }
            else //click off
            {
                pictureBoxTargetPeople.BackColor = Color.White;
            }
            //rapidCheck.objType = 0; 
        } // 0 = people
        private void pictureBoxTargetCar_Click(object sender, EventArgs e) { rapidCheck.objType = 1; } // 1 = car
        //------------------------------Direction Btn Click EVENT------------------------------
        private void pictureBoxDirection1_Click(object sender, EventArgs e) { }
        private void pictureBoxDirection2_Click(object sender, EventArgs e) { replay("and direction5 + direction6 + direction7 > 0.7"); }
        private void pictureBoxDirection3_Click(object sender, EventArgs e) { }
        private void pictureBoxDirection4_Click(object sender, EventArgs e) { }
        private void pictureBoxDirection5_Click(object sender, EventArgs e) { ObjReset(); } //검색 설정 초기화
        private void pictureBoxDirection6_Click(object sender, EventArgs e) { }
        private void pictureBoxDirection7_Click(object sender, EventArgs e) { }
        private void pictureBoxDirection8_Click(object sender, EventArgs e) { replay("and direction0 + direction1 + direction2  > 0.7"); }
        private void pictureBoxDirection9_Click(object sender, EventArgs e) { }
        //------------------------------Color Btn Click EVENT------------------------------
        private void buttonColor0_Click(object sender, EventArgs e) {replay("and color1 > 0.1"); }
        private void buttonColor1_Click(object sender, EventArgs e) {replay("and color0 + color15 > 0.1"); }
        private void buttonColor2_Click(object sender, EventArgs e) {replay("and color2 + color3 > 0.1"); }
        private void buttonColor3_Click(object sender, EventArgs e) {replay("and color3 + color4 + color5 > 0.1"); }
        private void buttonColor4_Click(object sender, EventArgs e) {replay("and color6 + color7 > 0.1"); }
        private void buttonColor5_Click(object sender, EventArgs e) {replay("and color7 + color8 + color9 + color10 + color11> 0.2"); }
        private void buttonColor6_Click(object sender, EventArgs e) {replay("and color12 + color13 > 0.1"); }
        private void buttonColor7_Click(object sender, EventArgs e) {replay("and color13 + color14 > 0.1"); }
        private void buttonColor8_Click(object sender, EventArgs e) {replay("and color16 > 0.6 and color17 > 0.15"); }
        private void buttonColor9_Click(object sender, EventArgs e) {replay("and color16 > 0.6 and color18 > 0.3"); }
        //------------------------------Search Function------------------------------
        private void replay(string condition)
        {
            //rapidCheck.condition = condition;
            overlayModule.Abort();
            Thread.Sleep(1);
            myRapidModule.Clear();
            myRapidModule.Add(rapidCheck.setFileterObjectidList); //filetering test
            myRapidModule.Add(rapidCheck.objectClustering);
            myRapidModule.Add(rapidCheck.buildOverlayOrderUsingCluster);
            myRapidModule.Add(rapidCheck.overlayLive);

            overlayModule = new Thread(() => rapidRun());
            overlayModule.Start();
        }
        //------------------------------초기화 EVENT------------------------------
        private void ObjReset()
        {
            overlayModule.Abort();
            Thread.Sleep(1);
            myRapidModule.Clear();
            myRapidModule.Add(rapidCheck.resetObjectidList);
            myRapidModule.Add(rapidCheck.objectClustering);
            myRapidModule.Add(rapidCheck.buildOverlayOrderUsingCluster);
            myRapidModule.Add(rapidCheck.overlayLive);
            overlayModule = new Thread(() => rapidRun());
            overlayModule.Start();
        }
    }
}
