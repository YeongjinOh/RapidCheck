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
        /*****************/private int inputDirection = -1;             /*****************/
        /*****************/private int color = -1;                      /*****************/
        /*****************/string videoPath = null;                     /*****************/
        /*****************/OpenFileDialog videoFilePath;                /*****************/
        /*****************/int outputFrameNum = 0;                      /*****************/
        /*****************/RapidCheck.OverlayVideo rapidCheck;          /*****************/
        /*****************/delegate void rapidModule();                 /*****************/
        /*****************/delegate void rapidChain(rapidModule dele);  /*****************/
        /*****************/Thread overlayModule;                        /*****************/
        /*****************/List<rapidModule> myRapidModule = new List<rapidModule>();/*****************/
        //******************************Form******************************
        public Form1()
        {
            InitializeComponent();
            var skinManager = MaterialSkinManager.Instance;
            skinManager.AddFormToManage(this);
            //skinManager.Theme = MaterialSkinManager.Themes.DARK;
            skinManager.Theme = MaterialSkinManager.Themes.LIGHT;
            skinManager.ColorScheme = new ColorScheme(
                Primary.BlueGrey100, // tab contorol
                Primary.Blue200, //최상단 
                Primary.BlueGrey100, 
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
            string createTime = setCreateTime(System.IO.Path.GetDirectoryName(videoFilePath.FileName), System.IO.Path.GetFileName(videoFilePath.FileName));
            int maxFrameNum = 1000;
            //int frameStep = 3;
            int analysisFPS = 5; //default
            int minTrackingLength = 29;
            int clusterNum = 6;
            outputFrameNum = 500;
            rapidCheck = new RapidCheck.OverlayVideo(dataGridView1, startBtn, trackBar1, pictureBoxVideo, videoPath, createTime, maxFrameNum, analysisFPS, minTrackingLength, clusterNum, outputFrameNum); //ObjList setting
            
            rapidFunc();
            overlayModule = new Thread(() => rapidRun());
            overlayModule.Start();
        }
        private void basicFlow(rapidModule dele)
        {
            dele();
            progressBar1.PerformStep();
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
            progressBar1.Value = 100;
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
        private void VideoStartBtn_Click(object sender, EventArgs e)
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

        private void onToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.drawTime = true;
        }
        private void offToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.drawTime = false;
        }

        private void DownToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and direction0 + direction1 + direction2  > 0.7";
            direction();
        }

        private void UpToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and direction5 + direction6 + direction7 > 0.7";
            direction();
        }

        private void direction()
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

        private void BlackToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rapidCheck.condition = "and color9 > 0.3";
            direction();
        }

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

        public ChromiumWebBrowser browser;
        private void materialTabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = materialTabControl1.SelectedIndex;
            if (idx ==3)
            {
                browser = new ChromiumWebBrowser("http://www.google.com")
                {
                    Dock = DockStyle.Fill,
                    Size = Size,
                };
                panelWebbrowser.Controls.Add(browser);
                browser.Size = new Size(panelWebbrowser.Size.Width, panelWebbrowser.Size.Height);
            }
            else if( idx == 2)
            {
                chartsTest1();
                chartsTest2();
                chartsTest3();
                chartsTest4();
            }
        }



        //요약 페이지 코드  -> 나중에 분할 예정
        private void chartsTest1()
        {
            cartesianChart1.Series = new SeriesCollection
            {
                new StackedColumnSeries
                {
                    Values = new ChartValues<double> {4, 5, 6, 8},
                    StackMode = StackMode.Values, // this is not necessary, values is the default stack mode
                    DataLabels = true
                },
                new StackedColumnSeries
                {
                    Values = new ChartValues<double> {2, 5, 6, 7},
                    StackMode = StackMode.Values,
                    DataLabels = true
                }
            };

            //adding series updates and animates the chart
            cartesianChart1.Series.Add(new StackedColumnSeries
            {
                Values = new ChartValues<double> { 6, 2, 7 },
                StackMode = StackMode.Values
            });

            //adding values also updates and animates
            cartesianChart1.Series[2].Values.Add(4d);

            cartesianChart1.AxisX.Add(new Axis
            {
                Title = "Browser",
                Labels = new[] { "Chrome", "Mozilla", "Opera", "IE" },
                Separator = DefaultAxes.CleanSeparator
            });

            cartesianChart1.AxisY.Add(new Axis
            {
                Title = "Usage",
                LabelFormatter = value => value + " Mill"
            });
        }
        private void chartsTest2()
        {
            cartesianChart2.Series = new SeriesCollection
            {
                new ColumnSeries
                {
                    Title = "2015",
                    Values = new ChartValues<double> { 10, 50, 39, 50 }
                }
            };

            //adding series will update and animate the chart automatically
            cartesianChart2.Series.Add(new ColumnSeries
            {
                Title = "2016",
                Values = new ChartValues<double> { 11, 56, 42 }
            });

            //also adding values updates and animates the chart automatically
            cartesianChart2.Series[1].Values.Add(48d);

            cartesianChart2.AxisX.Add(new Axis
            {
                Title = "Sales Man",
                Labels = new[] { "Maria", "Susan", "Charles", "Frida" }
            });

            cartesianChart2.AxisY.Add(new Axis
            {
                Title = "Sold Apps",
                LabelFormatter = value => value.ToString("N")
            });
        }
        private void chartsTest3()
        {
            cartesianChart3.Series = new SeriesCollection
            {
                new RowSeries
                {
                    Title = "2015",
                    Values = new ChartValues<double> { 10, 50, 39, 50 }
                }
            };

            //adding series will update and animate the chart automatically
            cartesianChart3.Series.Add(new RowSeries
            {
                Title = "2016",
                Values = new ChartValues<double> { 11, 56, 42 }
            });

            //also adding values updates and animates the chart automatically
            cartesianChart3.Series[1].Values.Add(48d);

            cartesianChart3.AxisY.Add(new Axis
            {
                Labels = new[] { "Maria", "Susan", "Charles", "Frida" }
            });

            cartesianChart3.AxisX.Add(new Axis
            {
                LabelFormatter = value => value.ToString("N")
            });

            var tooltip = new DefaultTooltip
            {
                SelectionMode = TooltipSelectionMode.SharedYValues
            };

            cartesianChart3.DataTooltip = tooltip;
        }
        private void chartsTest4()
        {
            pieChart1.InnerRadius = 100;
            pieChart1.LegendLocation = LegendLocation.Right;

            pieChart1.Series = new SeriesCollection
            {
                new PieSeries
                {
                    Title = "Chrome",
                    Values = new ChartValues<double> {8},
                    PushOut = 15,
                    DataLabels = true
                },
                new PieSeries
                {
                    Title = "Mozilla",
                    Values = new ChartValues<double> {6},
                    DataLabels = true
                },
                new PieSeries
                {
                    Title = "Opera",
                    Values = new ChartValues<double> {10},
                    DataLabels = true
                },
                new PieSeries
                {
                    Title = "Explorer",
                    Values = new ChartValues<double> {4},
                    DataLabels = true
                }
            };
        }
    }
}
