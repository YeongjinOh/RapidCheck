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
                List<objIdAndOrderingCnt> temp = new List<objIdAndOrderingCnt>();
                for (int objectidListIdx = 0; (objectidListIdx < objectidList.Count) && (overlayFrameNum >=(objectidListIdx/group)*offset); objectidListIdx++)
                {
                    int id = objectidList[objectidListIdx];
                    int orderingCnt = ObjList[id].OrderingCnt;
                    objIdAndOrderingCnt newOrder = new objIdAndOrderingCnt(id, orderingCnt);
                    temp.Add(newOrder);
                    ObjList[id].OrderingCnt++;
                }
                overlayOrders.Add(temp);
            }
        }
        public void buildOverlayOrderUsingCluster() // 애를 만든다. overlayOrders;
        {
            for (int overlayFrameNum = 0; overlayFrameNum < outputFrameNum; overlayFrameNum++)
            {
                List<objIdAndOrderingCnt> currentObjid = new List<objIdAndOrderingCnt>();
                for (int groupIdx = 0; groupIdx < startingGroup.Count; groupIdx++)
                {
                    if(startingGroup[groupIdx].hasNext())
                    {
                        int id = startingGroup[groupIdx].getNextId(ref ObjList);
                        int orderingCnt = ObjList[id].OrderingCnt;
                        objIdAndOrderingCnt newOrder = new objIdAndOrderingCnt(id, orderingCnt);
                        currentObjid.Add(newOrder);
                        ObjList[id].OrderingCnt++;
                    }
                }
                overlayOrders.Add(currentObjid);
            }
        }
        public void overlay()
        {
            Bitmap background = new Bitmap(@"C:\videos\Background\0.bmp"); //*****Background는....0번째 프레임?
            VideoFileWriter writer = new VideoFileWriter();
            string outputPath = string.Format(@"C:\videos\output\video{0}_{1}_{2}_{3}.avi", videoid, maxFrameNum, outputFrameNum, clusterNum);
            if(System.IO.File.Exists(outputPath)) //해당 이름을 가진 파일이 존재한다면,,,,
            {
                outputPath = outputPath.Replace(".avi",string.Format("_{0}.avi", System.DateTime.Now.ToString("_hh시mm분")));
            }
            writer.Open(outputPath, videoWidth, videoHeight, 5, VideoCodec.H264);
            for (int resFrame = 0; resFrame < outputFrameNum; resFrame++)
            {
                Bitmap BitCopy = (Bitmap)background.Clone();
                for (int idx = 0; idx < overlayOrders[resFrame].Count; idx++)
                {
                    int id = overlayOrders[resFrame][idx].id;
                    BitCopy = combinedImage(BitCopy, ObjList[id].getNextCropImage(), ObjList[id].getNextCropArea());
                }
                writer.WriteVideoFrame(BitCopy);
                BitCopy.Dispose();
            }
            writer.Close();
        }
        public void overlayLive()
        {
            Bitmap background = new Bitmap(@"C:\videos\Background\0.bmp"); //*****Background는....0번째 프레임?
            //set panel
            Graphics gs = pictureBoxVideo.CreateGraphics();
            //System.Threading.Thread.Sleep(500);
            ///panel
            for (resFrame = 0; resFrame < outputFrameNum; resFrame++)
            {
                Bitmap BitCopy = (Bitmap)background.Clone();
                for (overlayObjIdx=0; overlayObjIdx < overlayOrders[resFrame].Count; overlayObjIdx++)
                {
                    int id = overlayOrders[resFrame][overlayObjIdx].id;
                    int orderingCnt = overlayOrders[resFrame][overlayObjIdx].orderingCnt;
                    BitCopy = combinedImage(BitCopy, ObjList[id].getCropImage(orderingCnt), ObjList[id].getCropArea(orderingCnt));
                }
                trackingBar.Value += 1;
                gs.DrawImage(BitCopy, new Point(0, 0));
                BitCopy.Dispose();
            }
        }
        public void setObjidList()
        {
            bool useFilter = false;
            if (!useFilter)
            {
                objectidList.Clear();
                for (int idIdx = 0; idIdx < ObjList.Count; idIdx++)
                {
                    ObjList[idIdx].currentAreaPositionIdx = 0;
                    ObjList[idIdx].currentImagePositionIdx = 0;
                    //ObjList[idIdx].resetPosition();
                    objectidList.Add(idIdx);
                }
            }
            else
            {
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
                    ObjList[id].currentAreaPositionIdx = 0;
                    ObjList[id].currentImagePositionIdx = 0;
                    //ObjList[id].resetPosition();
                    //MessageBox.Show(id.ToString());
                }
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
        public void imageCrop()
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
                        Rectangle temp = ObjList[objid].getNextCropArea();
                        ObjList[objid].addCropImage(videoFrame.Clone(temp, videoFrame.PixelFormat));
                    }
                }
                videoFrame.Dispose();
            }
            reader.Close();
        }
        //public void saveAviFile() //삭제예정
        //{
        //    VideoFileWriter writer = new VideoFileWriter();
        //    string outputPath = string.Format(@"C:\videos\output\{0}.avi", videoid);
        //    writer.Open(outputPath, videoWidth, videoHeight, 5, VideoCodec.H264);
        //    for (int i = 0; i < outputFrameNum; i++)
        //    {
        //        writer.WriteVideoFrame(overlayFrames[i]);
        //        overlayFrames[i].Dispose();
        //    }
        //    writer.Close();
        //}
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
        public void kMeasFunc()
        {
            var kmeas = new KMeans(k:clusterNum);
            double[][] points = new double[ObjList.Count][];
            for (int i = 0; i < ObjList.Count; i++)
            {
                points[i] = ObjList[i].getStartingPoint();
            }
            KMeansClusterCollection clusters = kmeas.Learn(points);
            int[] output = clusters.Decide(points);
            
            for (int id = 0; id < ObjList.Count; id++)
            {
                startingGroup[output[id]].Add(id);
            }
            //sort
            for (int k = 0; k < startingGroup.Count; k++)
            {
                startingGroup[k].sort(ref ObjList);
            }
        }
        public void setResFrame(int resFrameNum)
        {
            this.resFrame = resFrameNum;
        }
    }
}
