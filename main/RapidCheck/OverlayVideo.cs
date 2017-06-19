using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing; //Bitmap
using System.Drawing.Imaging;
using MySql.Data.MySqlClient;
using System.Data;
using System.Windows.Forms;
using Accord.Video.VFW;
using Accord.Video.FFMPEG;

using System.Diagnostics;
using OxyPlot;
using OxyPlot.Series;
using OxyPlot.Axes;
namespace RapidCheck
{
    public class objDataGridView
    {
        public DataGridViewImageColumn img;
        public string contents;
        public objDataGridView(DataGridViewImageColumn img, string contents)
        {
            this.img = img;
            this.contents = contents;
        }
    }
    public class objIdAndOrderingCnt
    {
        public int id {get; set;}
        public int orderingCnt {get;set;}
        public objIdAndOrderingCnt(int id, int orderingCnt)
        {
            this.id = id;
            this.orderingCnt = orderingCnt;
        }
    }
    public partial class OverlayVideo
    {
        //private List<string> colors; //검색 조건들 이후에 추가..
        private List<Obj> ObjList; //tracking table data
        private List<int> objectidList; //분석에 사용된 id
        private List<int> originObjectidList; // copy objectidList
        private Dictionary<int, List<int>> objidByFrame; //key : frame, value : Objid //해당 frame에 등장한 object //crop할 위치 확인용
        private List<Bitmap> overlayFrames; //result Frame
        private List<List<objIdAndOrderingCnt>> overlayOrders; //overlayOrders[i] : i번째 output frame에 등장해야 할 Object id들의 리스트
        private List<int> idxbyObjid; //objid로 인덱스 접근

        private List<int> trackingTableFrameNum;
        private List<int> trackingTableObjid;
        private Dictionary<int,int> trackingTableClassid;
        private Dictionary<int, int> trackingTableDirectionid;
        private List<Rectangle> trackingTableRectangle;
        public List<int> gridViewList1;
        public List<int> gridViewList2;
        private string videoPath;
        public int videoWidth { get; set; }
        public int videoHeight { get; set; }
        private int frameStep;
        private int videoid;
        private int outputFrameNum;
        private string strConn = "Server=localhost;Database=rapidcheck;Uid=root;Pwd=1234;";
        public string conditionTarget { set; get; }
        public string conditionDensity { set; get; }
        public string conditionDirection { set; get; }
        public string conditionColor { set; get; }
        private int maxFrameNum;
        private int minTrackingLength;
        private List<StartingGroup> startingGroup; //kmeans test
        private int clusterNum;
        public int speed{get;set;}
        public int fps { get; set; }
        private int maxObjectid;
        private System.DateTime createTime;
        private int frameRate;
        public bool drawTime { get; set; }
        public int objType; //1 = both, 2 = people, 3 = car
        //drawing style
        System.Drawing.Font drawFont;
        System.Drawing.SolidBrush drawBrush;
        //background
        Bitmap background;
        Bitmap backgroundCar;
        Bitmap backgroundPeople;
        Bitmap Direction0;
        Bitmap Direction1;
        Bitmap Direction2;
        Bitmap Direction3;
        Bitmap Direction4;
        Bitmap Direction5;
        Bitmap Direction6;
        Bitmap Direction7;

        //UI
        PictureBox pictureBoxVideo;
        TrackBar trackingBar;
        Button startBtn;
        DataGridView dataGridView1;
        DataGridView dataGridView2;
        Label labelProgress;
        Label labelVideoInfo1;
        Label labelVideoInfo2;
        Label labelVideoInfo3;
        string strVideoInfo1;
        string strVideoInfo2;
        string strVideoInfo3;
        public int resFrame { get; set; }
        public int overlayObjIdx { set; get; }
        public int clickFramePosition { set; get; } // mouse click frame position

        //tabpage3
        List<int> directionCntPeople;
        List<int> directionCntCar;

        List<double> colorRatioPeople;
        List<double> colorRatioCar;
        public PlotModel modelBarChart;
        public PlotModel modelPieChartPeople;
        public PlotModel modelPieChartCar;
        public PlotModel modelLineChart;
        public int peopleTotal { get; set; }
        public int carTotal { get; set; }
        public OverlayVideo() { }
        public OverlayVideo(Label labelVideoInfo1, Label labelVideoInfo2, Label labelVideoInfo3, Label labelProgress, DataGridView dataGridView1, DataGridView dataGridView2, Button startBtn, TrackBar TrackingBar, PictureBox pictureBoxVideo, string path, string createTime, int maxFrameNum, int analysisFPS = 5, int minTrackingLength = 29, int clusterNum = 20, int outputFrameNum = 1000)
        {
            //drawing style
            drawFont = new System.Drawing.Font("Arial", 14);
            drawBrush = new System.Drawing.SolidBrush(System.Drawing.Color.White);
            backgroundPeople = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\back\human@1x.png");
            backgroundCar = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\back\Car@1x.png");
            Direction0 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir0.png");
            Direction1 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir1.png");
            Direction2 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir2.png");
            Direction3 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir3.png");
            Direction4 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir4.png");
            Direction5 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir5.png");
            Direction6 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir6.png");
            Direction7 = new Bitmap(@"C:\Users\SoMa\Desktop\RapidCheck\main\RapidCheck\asset\dir7.png");
            //UI
            this.labelVideoInfo1 = labelVideoInfo1;
            this.labelVideoInfo2 = labelVideoInfo2;
            this.labelVideoInfo3 = labelVideoInfo3;
            this.labelProgress = labelProgress;
            this.dataGridView1 = dataGridView1;
            this.dataGridView2 = dataGridView2;
            this.trackingBar = TrackingBar;
            this.pictureBoxVideo = pictureBoxVideo;
            this.startBtn = startBtn;
            this.resFrame = 0;
            this.overlayObjIdx = 0;
            this.clickFramePosition = 0;
            //------------------------------변수 초기화-----------------------------
            ObjList = new List<Obj>(); //DB Table
            objectidList = new List<int>();
            originObjectidList = new List<int>();
            objidByFrame = new Dictionary<int, List<int>>();
            overlayFrames = new List<Bitmap>();
            overlayOrders = new List<List<objIdAndOrderingCnt>>();

            trackingTableObjid = new List<int>();
            trackingTableClassid = new Dictionary<int, int>();
            trackingTableDirectionid = new Dictionary<int, int>();
            trackingTableFrameNum = new List<int>();
            trackingTableRectangle = new List<Rectangle>();
            gridViewList1 = new List<int>();
            gridViewList2 = new List<int>();
            videoPath = path;
            videoPath = videoPath.Replace(@"\", @"\\");
            this.conditionTarget = "";
            this.conditionDensity = "";
            this.conditionDirection = "";
            this.conditionColor = "";
            this.outputFrameNum = outputFrameNum;
            this.maxFrameNum = maxFrameNum;
            this.minTrackingLength = minTrackingLength;
            this.clusterNum = clusterNum;
            this.speed = 1;
            this.drawTime = true;
            this.createTime = new System.DateTime(2017, 1, 1, Int32.Parse(createTime.Split(':')[0]), Int32.Parse(createTime.Split(':')[1]), 0);
            this.objType = 1; //default는 둘 다 검색
            this.maxObjectid = 0;
            startingGroup = new List<StartingGroup>();
            for (int i = 0; i < clusterNum; i++)
            {
                startingGroup.Add(new StartingGroup());
            }
            
            //read video info
            Accord.Video.FFMPEG.VideoFileReader reader = new Accord.Video.FFMPEG.VideoFileReader();
            reader.Open(videoPath);
            frameRate = reader.FrameRate;
            videoWidth = reader.Width;
            videoHeight = reader.Height;
            background = reader.ReadVideoFrame(); // 첫번째 프레임을 백그라운드로
            fps = reader.FrameRate;
            //reader.FrameCount
            this.frameStep = fps / analysisFPS;
            if (maxFrameNum == 0 || maxFrameNum > (int)reader.FrameCount)
            {
                this.maxFrameNum = (int)reader.FrameCount;
            }
            strVideoInfo1 = "path: " + path + "\nSize: " + reader.Width + " x " + reader.Height + "\ncodec: " + reader.CodecName;
            labelVideoInfo1.Text = strVideoInfo1;
            strVideoInfo2 = "FPS: " + fps + "\nCluster value: " + clusterNum + "\nFrame step: " + frameStep;
            labelVideoInfo2.Text = strVideoInfo2;
            strVideoInfo3 = "create Time: " + createTime + "\nVideo Frame: " + reader.FrameCount;
            reader.Close();

            //tabpage3
            directionCntPeople = new List<int>();
            directionCntCar = new List<int>();
            colorRatioPeople = new List<double>();
            colorRatioCar = new List<double>();
            peopleTotal = 0;
            carTotal = 0;
            //------------------------------/변수 초기화-----------------------------
        }
    }
}