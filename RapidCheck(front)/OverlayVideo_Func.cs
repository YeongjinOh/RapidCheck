using System;
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
using Accord.MachineLearning; //knn test
namespace RapidCheck
{
    public partial class OverlayVideo
    {
        public void addObj()
        {
            int row = 0;
            for (int objid = 0; objid < trackingTableObjid.Distinct().Count(); objid++) //0~1196
            {
                Obj temp = new Obj(objid);
                for (; row < trackingTableObjid.Count && objid == trackingTableObjid[row]; row++)
                {
                    Rectangle rect = trackingTableRectangle[row];
                    modifyCropArea(ref rect);
                    temp.addCropArea(rect);
                    temp.addCropPositionNum(trackingTableFrameNum[row]);
                }
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
            int offset = 10;
            int group = 3;
            for (int overlayFrameNum = 0; overlayFrameNum < outputFrameNum; overlayFrameNum++)
            {
                List<int> temp = new List<int>();
                for (int objectidListIdx = 0; (objectidListIdx < objectidList.Count) && (overlayFrameNum >=(objectidListIdx/group)*offset); objectidListIdx++)
                {
                    int id = objectidList[objectidListIdx];
                    temp.Add(id);
                }
                overlayOrders.Add(temp);
            }
        }
        public void overlay()
        {
            overlayFrames = new List<Bitmap>();
            //drawing
            Bitmap background = new Bitmap(@"C:\videos\Background\0.bmp"); //*****Background는....0번째 프레임?
            for (int resFrame = 0; resFrame < overlayOrders.Count; resFrame++)
            {
                Bitmap BitCopy = (Bitmap)background.Clone();
                for (int idx = 0; idx < overlayOrders[resFrame].Count; idx++)
                {
                    int id = overlayOrders[resFrame][idx];
                    BitCopy = combinedImage(BitCopy, ObjList[id].getNextCropImage(), ObjList[id].getCropArea());
                }
                overlayFrames.Add(BitCopy);
            }
        }
        public void setObjidList()
        {
            objectidList.Clear();
            int idCnt = trackingTableObjid.Distinct().Count();
            for (int idIdx = 0; idIdx < idCnt; idIdx++)
            {
                if (ObjList[idIdx].getFrameCnt() >= outputFrameNum) //48frame 이상인 아이들 저장
                {
                    ObjList[idIdx].currentAreaPositionIdx = 0;
                    ObjList[idIdx].currentImagePositionIdx = 0;
                    objectidList.Add(idIdx);
                }
            }
            List<int> temp = new List<int>();
            using (MySqlConnection conn = new MySqlConnection(strConn))
            {
                DataSet ds = new DataSet();
                MySqlDataAdapter adapter = new MySqlDataAdapter();
                DataTable dt = new DataTable();

                //string SQL = String.Format("SELECT objectId FROM rapidcheck.objectinfo where direction1 + direction2 > 0.7 and videoId={0};", videoid);
                string SQL = String.Format("SELECT * FROM rapidcheck.objectinfo where videoId={0} AND objectId <= {1};", videoid, ObjList.Count);

                adapter.SelectCommand = new MySqlCommand(SQL, conn);
                adapter.Fill(ds, "directionid");
                dt = ds.Tables["directionid"];
                foreach (DataRow dr in dt.Rows)
                {
                    temp.Add(Convert.ToInt32(dr["objectId"]));
                }
            }
            List<int> store = new List<int>();
            for (int i = 0; i < objectidList.Count; i++)
            {
                for (int j = 0; j < temp.Count; j++)
                {
                    if (objectidList[i] == temp[j])
                    {
                        store.Add(objectidList[i]);
                        break;
                    }
                }
            }
            objectidList.Clear();
            foreach (int id in store)
            {
                objectidList.Add(id);
                //MessageBox.Show(id.ToString());
            }
        }
        public Bitmap combinedImage(Bitmap back, Bitmap front, Rectangle position)
        {
            try
            {
                if ((back != null) | (front != null))
                {
                    
                    using (Graphics gr = Graphics.FromImage(back))
                    {
                        ColorMatrix matrix = new ColorMatrix();
                        matrix.Matrix33 = 0.75f; //0.7~0.75
                        ImageAttributes att = new ImageAttributes();
                        att.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
                        gr.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                        gr.DrawImage(front, position, 0, 0, front.Width, front.Height, GraphicsUnit.Pixel, att);
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
                    //INSERT
                    conn.Open();
                    videoPath = videoPath.Replace(@"\", @"\\");
                    string insertCMD = String.Format("INSERT INTO rapidcheck.file(path, frameStep) values ('{0}',{1});", videoPath, frameStep);
                    MySqlCommand cmd = new MySqlCommand(insertCMD, conn);
                    cmd.ExecuteNonQuery();

                    //SELECT
                    DataSet ds = new DataSet();
                    MySqlDataAdapter adapter = new MySqlDataAdapter();
                    DataTable dt = new DataTable();

                    string SQL = String.Format("Select videoId from rapidcheck.file where path=\"{0}\"", videoPath);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "videoid");
                    dt = ds.Tables["videoid"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        videoid = Convert.ToInt32(dr["videoid"]);
                    }
                    videoid = 1; //test

                    SQL = string.Format("SELECT max(objectId) as maxid FROM rapidcheck.tracking where videoId={0} AND frameNum < {1};", videoid, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "maxid");

                    SQL = string.Format("SELECT objectId, frameNum, x, y, width, height FROM tracking where videoId={0} AND frameNum < {1} ORDER BY objectId ASC, frameNum ASC", videoid, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "data");

                    SQL = string.Format("SELECT objectId, frameNum FROM tracking where videoId={0} ORDER BY frameNum ASC", videoid);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "objidByframe");


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
                    foreach (DataRow dr in dt.Rows)
                    {
                        int tempFrame = Convert.ToInt32(dr["frameNum"]);
                        int tempObjid = Convert.ToInt32(dr["objectId"]);
                        if (objidByFrame.ContainsKey(tempFrame)) //Dictionary에 key가 있다면
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

            for (int frameNum = 0; frameNum < maxFrameNum/*reader.FrameCount*/; frameNum++)
            {
                Bitmap videoFrame = reader.ReadVideoFrame();
                if (objidByFrame.ContainsKey(frameNum))
                {
                    foreach (int objid in objidByFrame[frameNum])
                    {
                        Rectangle temp = ObjList[objid].getCropArea();
                        ObjList[objid].addCropImage(videoFrame.Clone(temp, videoFrame.PixelFormat));
                    }
                }
                videoFrame.Dispose();
            }
            reader.Close();
        }
        public void saveAviFile()
        {
            VideoFileWriter writer = new VideoFileWriter();
            string outputPath = string.Format(@"C:\videos\output\{0}.avi", videoid);
            writer.Open(outputPath, videoWidth, videoHeight, 5, VideoCodec.H264);
            for (int i = 0; i < outputFrameNum; i++)
            {
                writer.WriteVideoFrame(overlayFrames[i]);
            }
            writer.Close();
        }
        public void modifyCropArea(ref Rectangle cropArea)
        {
            cropArea.X -= 10;
            cropArea.Y -= 20;
            cropArea.Width += 20;
            cropArea.Height += 20;

            //일단은... width x height
            if (cropArea.X > videoWidth) cropArea.X /= 2;
            if (cropArea.Y > videoHeight) cropArea.Y /= 2;
            //if (cropArea.X < 0) cropArea.X = 0;
            //if (cropArea.Y < 0) cropArea.Y = 0;
            if (cropArea.X + cropArea.Width > videoWidth) cropArea.Width = videoWidth - cropArea.X;
            if (cropArea.Y + cropArea.Height > videoHeight) cropArea.Height = videoHeight - cropArea.Y;
        }
        
        
        public void kMeasFunc(int k )
        {
            var kmeas = new KMeans(k:k);
            //List<List<int>> points = new List<List<int>>();
            double[][] points = new double[ObjList.Count][];
            //List<Point> points = new List<Point>();
            for (int i = 0; i < ObjList.Count; i++)
            {
                points[i] = ObjList[i].getStartingPoint();
            }
            KMeansClusterCollection clusters = kmeas.Learn(points);
            int[] output = clusters.Decide(points);
            for (int i = 0; i < ObjList.Count; i++)
            {
                points[i] = ObjList[i].getStartingPoint();
                string temp = string.Format("x:{0} y:{1} class:{2}\n", points[i][0], points[i][1], output[i]);
                Console.WriteLine(temp);
            }
        }
    }
}
