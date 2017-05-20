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

namespace RapidCheck
{
    public class OverlayVideo
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

        private string strConn = "Server=localhost;Database=rapidcheck;Uid=root;Pwd=1234;";
        //overlayOrders의 길이와 overlayFrames의 길이는 같아야한다??? 디펜던시가 있다
        public OverlayVideo()
        {
            ObjList = new List<Obj>(); //DB Table
            objectidList = new List<int>();
            objidByFrame = new Dictionary<int, List<int>>();
            overlayFrames = new List<Bitmap>();
            overlayOrders = new List<List<int>>();
            
            trackingTableObjid = new List<int>();
            trackingTableFrameNum = new List<int>();
            trackingTableRectangle = new List<Rectangle>();
            videoPath = @"C:\videos\tracking.mp4";

            //set (trackingTableFrameNum, trackingTableObjid, trackingTableRectangle, objectidList)
            MessageBox.Show("sqr");
            getMysqlObjList();
            //set (ObjList)
            MessageBox.Show("addObj");
            addObj();

            //set Obj cropImage
            MessageBox.Show("crop");
            imageCrop(videoPath);
            
            //set overlayFrames
            MessageBox.Show("overlay");
            overlay();
            MessageBox.Show("save\n" + overlayFrames.Count);

            for (int i = 0; i < overlayFrames.Count; i++)
            {
                string filepath = @"C:\videos\test\" + i + ".bmp";
                overlayFrames[i].Save(filepath);
            }

            /* 1. Db에서 가져온 데이터를 기준으로OBJ생성
             * 2. image crop (this class에서 처리해야함)
             * 3. background + cropimage
             * -----DONE-----
             * 4. Bitmap merge -> .avi
             */
        }
        public void addObj()
        {
            int row = 0;
            for (int objid = 0; objid < trackingTableObjid.Distinct().Count(); objid++)
            {
                Obj temp = new Obj(objid);
                try
                {
                    for (; objid == trackingTableObjid[row]; row++)
                    {
                        temp.addCropArea(trackingTableRectangle[row]);
                        temp.addCropPositionNum(trackingTableFrameNum[row]);
                    }
                }
                catch (System.ArgumentOutOfRangeException){  }
                ObjList.Add(temp);
            }
        }
        public void addOverlayFrames(Bitmap frame)
        {
            overlayFrames.Add(frame);
        }
        public void setDefaultObjectidList(int maxObjectid)
        {
            for (int idx = 0; idx < maxObjectid; idx++)
            {
                objectidList.Add(idx);
            }
        }
        public void buildOverlayOrder() // 애를 만든다. overlayOrders;
        {
            //일단은 모든 obj들을 0frame부터 합성
            for (int overlayFrameNum = 0; overlayFrameNum < 100; overlayFrameNum++)
            {
                List<int> temp = new List<int>();
                for (int objectidListIdx = 0; objectidListIdx < objectidList.Count; objectidListIdx++)
                {
                    int id = objectidList[objectidListIdx];
                    if( !ObjList[id].emptyImage()) // image가 남아있다면
                    {
                        temp.Add(id);
                    }
                }
                overlayOrders.Add(temp);
            }
        }
        public void overlay()
        {
            overlayFrames = new List<Bitmap>();
            //현재는 over algorithm에대한 방안이 없으니....frame수가 48 이상인 것들만 overlay
            //step1. #frame >=48인 id추출 & max값 저장
            objectidList.Clear();
            int idCnt = trackingTableObjid.Distinct().Count();
            for (int idIdx = 0; idIdx < idCnt; idIdx++)
            {
                if (ObjList[idIdx].getFrameCnt() >= 100)
                {
                    ObjList[idIdx].currentAreaPositionIdx = 0;
                    ObjList[idIdx].currentImagePositionIdx = 0;
                    objectidList.Add(idIdx);
                }
            }
            //step2. 해당 frame에...그릴 id 확인
            //set overlayOrders
            buildOverlayOrder();
            //step3. drawing
            Bitmap background = new Bitmap(@"C:\videos\Background\0.bmp"); //*****Background는....0번째 프레임?
            for (int resFrame = 0; resFrame < overlayOrders.Count; resFrame++ )
            {
                Bitmap BitCopy = (Bitmap)background.Clone();
                try
                {
                    for (int idx = 0; idx < overlayOrders[resFrame].Count; idx++)
                    {
                        int id = overlayOrders[resFrame][idx];
                        BitCopy = combinedImage(BitCopy, ObjList[id].getNextCropImage(), ObjList[id].getCropArea());
                    }
                    overlayFrames.Add(BitCopy);
                }
                catch (System.ArgumentOutOfRangeException) { }
            }
        }
        public Bitmap combinedImage(Bitmap back, Bitmap front,Rectangle position)
        {
            try
            {
                if ((back != null) | (front != null))
                {
                    using(Graphics gr = Graphics.FromImage(back))
                    {
                        ColorMatrix matrix = new ColorMatrix();
                        matrix.Matrix33 = 0.6f;
                        ImageAttributes att = new ImageAttributes();
                        att.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
                        gr.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                        gr.DrawImage(front, position, 0,0,front.Width,front.Height, GraphicsUnit.Pixel,att);
                    }
                }
                return back;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Image combined Error");
                return null;
            }
        }
        public void getMysqlObjList()
        {
            try
            {
                using (MySqlConnection conn = new MySqlConnection(strConn))
                {
                    DataSet ds = new DataSet();
                    MySqlDataAdapter adapter = new MySqlDataAdapter();
                    adapter.SelectCommand = new MySqlCommand("SELECT max(objectId) as maxid FROM rapidcheck.tracking;",conn);
                    adapter.Fill(ds, "maxid");
                    adapter.SelectCommand = new MySqlCommand("SELECT objectId, frameNum, x, y, width, height FROM tracking where videoId=1 ORDER BY objectId ASC, frameNum ASC", conn);
                    adapter.Fill(ds, "data");
                    adapter.SelectCommand = new MySqlCommand("SELECT objectId, frameNum FROM tracking where videoId=1 ORDER BY frameNum ASC", conn);
                    adapter.Fill(ds, "objidByframe");
                    
                    DataTable dt = new DataTable();
                    dt = ds.Tables["maxid"];
                    int maxObjectid = 0;
                    foreach (DataRow dr in dt.Rows)
                    {
                        maxObjectid = Convert.ToInt32(dr["maxid"]);
                    }
                    //set ObjectidList
                    setDefaultObjectidList(maxObjectid);
                    //set (trackingTableFrameNum, trackingTableObjid, trackingTableRectangle)
                    dt = ds.Tables["data"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        trackingTableFrameNum.Add(Convert.ToInt32(dr["frameNum"]));
                        trackingTableObjid.Add(Convert.ToInt32(dr["objectId"]));
                        trackingTableRectangle.Add(new Rectangle(Convert.ToInt32(dr["x"]), Convert.ToInt32(dr["y"]), Convert.ToInt32(dr["width"]), Convert.ToInt32(dr["height"])));
                    }
                    //set objidByframe
                    dt = ds.Tables["objidByframe"];
                    foreach(DataRow dr in dt.Rows)
                    {
                        int tempFrame = Convert.ToInt32(dr["frameNum"]);
                        int tempObjid = Convert.ToInt32(dr["objectId"]);
                        if(objidByFrame.ContainsKey(tempFrame)) //Dictionary에 key가 있다면
                        {
                            objidByFrame[tempFrame].Add(tempObjid);
                        }
                        else //Dictionary에 key가 없다면
                        {
                            List<int> tempList = new List<int>();
                            tempList.Add(tempObjid);
                            objidByFrame.Add(tempFrame, tempList);
                        }
                    }
                }
            }
            catch (Exception)
            {
                Console.WriteLine("getMysqlObjList() ERROR");
            }
        }
        public void imageCrop(string videoPath)
        {
            Accord.Video.FFMPEG.VideoFileReader reader = new Accord.Video.FFMPEG.VideoFileReader();
            reader.Open(videoPath);
            for (int frameNum = 0; frameNum < reader.FrameCount; frameNum++)
            {
                Bitmap videoFrame = reader.ReadVideoFrame();
                if (objidByFrame.ContainsKey(frameNum))
                {
                    foreach(int objid in objidByFrame[frameNum])
                    {
                        Rectangle temp = new Rectangle();
                        temp = ObjList[objid].getCropArea();

                        ObjList[objid].addCropImage(videoFrame.Clone(temp, videoFrame.PixelFormat));
                    }
                }
                videoFrame.Dispose();
            }
            reader.Close();
        }
    }
}


// video play
// 0, 1, 2, 3
// objectId = 0 ~ (numOfPedestrians-1)
//int numOfPedestrians = 
//Obj objects[numOfPedestrians];

//foreach (DataRow dr in dt.Rows)
//                    {
//            int objectId = Convert.ToInt32(dr["objectId"]);
//            Obj object= objects[objectId];
//            int x = Convert.ToInt32(dr["x"]), y = Convert.ToInt32(dr["y"]), width = Convert.ToInt32(dr["width"]), height = Convert.ToInt32(dr["height"]);
//            object.addCropArea(x, y, width, height);
//                        index += 1;
//                    }
//                }
