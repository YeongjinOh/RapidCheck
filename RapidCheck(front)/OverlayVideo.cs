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
    public partial class OverlayVideo
    {
        //private List<string> colors; //검색 조건들 이후에 추가..
        private List<Obj> ObjList; //tracking table data
        private List<int> objectidList; //분석에 사용된 id
        private Dictionary<int, List<int>> objidByFrame; //key : frame, value : Objid //해당 frame에 등장한 object //crop할 위치 확인용
        private List<Bitmap> overlayFrames; //result Frame
        private List<List<int>> overlayOrders; //overlayOrders[i] : i번째 output frame에 등장해야 할 Object id들의 리스트

        private List<int> trackingTableFrameNum;
        private List<int> trackingTableObjid;
        private List<Rectangle> trackingTableRectangle;
        private string videoPath;
        private int videoWidth;
        private int videoHeight;
        private int frameStep;
        private int videoid;
        private int outputFrameNum;
        private string strConn = "Server=localhost;Database=rapidcheck;Uid=root;Pwd=1234;";
        private int maxFrameNum;
        //overlayOrders의 길이와 overlayFrames의 길이는 같아야한다??? 디펜던시가 있다
        public OverlayVideo() { }
        public OverlayVideo(string path)
        {
            ObjList = new List<Obj>(); //DB Table
            objectidList = new List<int>();
            objidByFrame = new Dictionary<int, List<int>>();
            overlayFrames = new List<Bitmap>();
            overlayOrders = new List<List<int>>();
            
            trackingTableObjid = new List<int>();
            trackingTableFrameNum = new List<int>();
            trackingTableRectangle = new List<Rectangle>();
            videoPath = path;
            videoHeight = 0;
            videoWidth = 0;
            frameStep = 5;
            outputFrameNum = 48;
            maxFrameNum = 10000;

            //****************************************width height test***********************************
            //Stopwatch sw = new Stopwatch();
            //sw.Start();
            Accord.Video.FFMPEG.VideoFileReader reader = new Accord.Video.FFMPEG.VideoFileReader();
            reader.Open(videoPath);
            videoWidth = reader.Width;
            videoHeight = reader.Height;
            reader.Close();
            //sw.Stop();
            //MessageBox.Show(sw.ElapsedMilliseconds.ToString() + "ms");
            //****************************************width height test***********************************
            // step1. db에서 obj정보를 가져오고 OBJ 필드값을 세팅
            getMysqlObjList();  //set (trackingTableFrameNum, trackingTableObjid, trackingTableRectangle, objectidList)
            addObj();           //set (ObjList)
            kMeasFunc(5);

            return;
            imageCrop(videoPath);//set Obj cropImage
            
            //step2. 오버레이 할 id를 선별한다
            setObjidList(); //set #frame >=48인 id추출 & max값 저장

            //step3. 오버레이 할 id를 기준으로 어떤 id가 어떤 구간에 그릴지를 설정한다
            buildOverlayOrder(); //set overlayOrders

            //step4. overlay
            overlay(); //set overlayFrames

            //step5. save .Avi file
            saveAviFile();
        }
    }
}