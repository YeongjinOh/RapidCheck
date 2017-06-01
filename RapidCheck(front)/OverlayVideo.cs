using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Collections.Generic; //List
using System.Drawing; //Bitmap
using System.Drawing.Imaging;
using MySql.Data.MySqlClient;
using System.Data;
using System.Windows.Forms;
using Accord.Video.VFW;
using Accord.Video.FFMPEG;

using System.Diagnostics;
//time
namespace RapidCheck
{
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
        private Dictionary<int, List<int>> objidByFrame; //key : frame, value : Objid //해당 frame에 등장한 object //crop할 위치 확인용
        private List<Bitmap> overlayFrames; //result Frame
        private List<List<objIdAndOrderingCnt>> overlayOrders; //overlayOrders[i] : i번째 output frame에 등장해야 할 Object id들의 리스트

        private List<int> trackingTableFrameNum;
        private List<int> trackingTableObjid;
        private List<Rectangle> trackingTableRectangle;
        private string videoPath;
        public int videoWidth { get; set; }
        public int videoHeight { get; set; }
        private int frameStep;
        private int videoid;
        private int outputFrameNum;
        private string strConn = "Server=localhost;Database=rapidcheck;Uid=root;Pwd=1234;";
        private int maxFrameNum;
        private int minTrackingLength;
        private List<StartingGroup> startingGroup; //kmeans test
        private int clusterNum;
        private int speed;
        
        //UI
        PictureBox pictureBoxVideo;
        TrackBar trackingBar;
        Button startBtn;
        public int resFrame { get; set; }
        public int overlayObjIdx { set; get; }
        public int clickFramePosition { set; get; } // mouse click frame position

        public OverlayVideo() { }
        public OverlayVideo(Button startBtn, TrackBar TrackingBar, PictureBox pictureBoxVideo, string path, int maxFrameNum, int frameStep = 5, int minTrackingLength = 29, int clusterNum = 20, int outputFrameNum = 1000)
        {
            this.trackingBar = TrackingBar;
            this.pictureBoxVideo = pictureBoxVideo;
            this.startBtn = startBtn;
            this.resFrame = 0;
            this.overlayObjIdx = 0;
            this.clickFramePosition = 0;
            //------------------------------변수 초기화-----------------------------
            ObjList = new List<Obj>(); //DB Table
            objectidList = new List<int>();
            objidByFrame = new Dictionary<int, List<int>>();
            overlayFrames = new List<Bitmap>();
            overlayOrders = new List<List<objIdAndOrderingCnt>>();
            
            trackingTableObjid = new List<int>();
            trackingTableFrameNum = new List<int>();
            trackingTableRectangle = new List<Rectangle>();
            videoPath = path;
            this.outputFrameNum = outputFrameNum;
            this.maxFrameNum = maxFrameNum;
            this.minTrackingLength = minTrackingLength;
            this.frameStep = frameStep;
            this.clusterNum = clusterNum;
            this.speed = 1;
            startingGroup = new List<StartingGroup>(clusterNum);
            for (int i = 0; i < clusterNum; i++)
            {
                startingGroup.Add(new StartingGroup());
            }
            
            //read video info
            Accord.Video.FFMPEG.VideoFileReader reader = new Accord.Video.FFMPEG.VideoFileReader();
            reader.Open(videoPath);
            videoWidth = reader.Width;
            videoHeight = reader.Height;
            if(maxFrameNum == 0)
            {
                this.maxFrameNum = (int)reader.FrameCount;
            }
            reader.Close();
            //------------------------------/변수 초기화-----------------------------
        }
    }
}