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
            //Bitmap background = new Bitmap(@"C:\videos\Background\0.bmp"); //*****Background는....0번째 프레임?
            VideoFileWriter writer = new VideoFileWriter();
            string outputPath = string.Format(@"C:\videos\output\video{0}_{1}_{2}_{3}.avi", videoid, maxFrameNum, outputFrameNum, clusterNum);
            if(System.IO.File.Exists(outputPath)) //해당 이름을 가진 파일이 존재한다면,,,,
            {
                outputPath = outputPath.Replace(".avi",string.Format("_{0}.avi", System.DateTime.Now.ToString("_hh시mm분")));
            }
            writer.Open(outputPath, videoWidth, videoHeight, frameStep, VideoCodec.H264);
            for (int resFrame = 0; resFrame < outputFrameNum; resFrame++)
            {
                Bitmap BitCopy = (Bitmap)background.Clone();
                for (int idx = 0; idx < overlayOrders[resFrame].Count; idx++)
                {
                    int id = overlayOrders[resFrame][idx].id;
                    BitCopy = combinedImage(BitCopy, ObjList[id].getNextCropImage(), ObjList[id].getNextCropArea(), 0.75f, ObjList[id].getStartFrameNum());
                }
                writer.WriteVideoFrame(BitCopy);
                BitCopy.Dispose();
            }
            writer.Close();
        }
        public void overlayLive()
        {
            //Bitmap background = new Bitmap(@"C:\videos\Background\0.bmp"); //*****Background는....0번째 프레임?
            Graphics gs = pictureBoxVideo.CreateGraphics();
            startBtn.Enabled = true;
            trackingBar.Enabled = true;
            startBtn.Text = "Pause";

            // set draw size
            int drawWidth = pictureBoxVideo.Width;
            int drawHeight = pictureBoxVideo.Height;
            if (background.Height * drawWidth > background.Width * drawHeight)
            {
                drawHeight = pictureBoxVideo.Height;
                drawWidth = (int)((double)pictureBoxVideo.Height * ((double)background.Width / (double)background.Height));
            }
            else if (background.Height * drawWidth < background.Width * drawHeight)
            {
                drawWidth = pictureBoxVideo.Width;
                drawHeight = (int)((double)pictureBoxVideo.Width * ((double)background.Height / (double)background.Width));
            }
            //set draw position
            int drawX = (pictureBoxVideo.Width - drawWidth) / 2;
            int drawY = (pictureBoxVideo.Height - drawHeight) / 2;
            //set puictureBox
            pictureBoxVideo.Height = drawHeight;
            pictureBoxVideo.Width = drawWidth;
            pictureBoxVideo.Location = new Point(drawX, drawY);


            //overlay time
            int frameTime = 1000 / frameStep;
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            float alphaMin = 0.4f, alphaDiff = 0.2f;
            //**********************DRAWING CODE**********************
            for (resFrame = 0; resFrame < outputFrameNum; resFrame++)
            {
                sw.Start();
                Bitmap BitCopy = (Bitmap)background.Clone();
                for (overlayObjIdx=0; overlayObjIdx < overlayOrders[resFrame].Count; overlayObjIdx++)
                {
                    int id = overlayOrders[resFrame][overlayObjIdx].id;
                    int orderingCnt = overlayOrders[resFrame][overlayObjIdx].orderingCnt;
                    Rectangle currentObjectArea = ObjList[id].getCropArea(orderingCnt);
                    float alpha = 0.8f;
                    
                    for (int prevOverlayObjIdx = 0; prevOverlayObjIdx < overlayObjIdx; prevOverlayObjIdx++)
                    {
                        int previd = overlayOrders[resFrame][prevOverlayObjIdx].id;
                        int prevOrderingCnt = overlayOrders[resFrame][prevOverlayObjIdx].orderingCnt;
                        Rectangle prevObjectArea = ObjList[previd].getCropArea(prevOrderingCnt);
                        if (isIntersect(currentObjectArea, prevObjectArea))
                        {
                            alpha -= alphaDiff;
                        }
                    }
                    if (alpha < alphaMin)
                        alpha = alphaMin;
                    BitCopy = combinedImage(BitCopy, ObjList[id].getCropImage(orderingCnt), currentObjectArea, alpha, ObjList[id].getStartFrameNum());
                }
                trackingBar.Value += 1;
                if (resFrame == outputFrameNum - 1)
                {
                    startBtn.Text = "Start";
                }
                sw.Stop();
                do
                {
                    gs.DrawImage(BitCopy, new Rectangle(0, 0, drawWidth, drawHeight));
                    if (frameTime - (int)sw.ElapsedMilliseconds > 0) { System.Threading.Thread.Sleep(frameTime - (int)sw.ElapsedMilliseconds); }
                    sw.Reset();
                    frameTime = 1000 / frameStep / speed;
                    
                } while (startBtn.Text == "Start");
                BitCopy.Dispose();
            }
        }
        public bool isIntersect(Rectangle rect1, Rectangle rect2)
        {
            return isIntersectPoint(rect1, new Point(rect2.X, rect2.Y)) ||
            isIntersectPoint(rect1, new Point(rect2.X, rect2.Y+rect2.Height)) ||
            isIntersectPoint(rect1, new Point(rect2.X + rect2.Width, rect2.Y)) ||
            isIntersectPoint(rect1, new Point(rect2.X + rect2.Width, rect2.Y + rect2.Height));
        }
        public bool isIntersectPoint(Rectangle rect, Point po)
        {
            return rect.X < po.X && rect.X + rect.Width > po.X && rect.Y < po.Y && rect.Y + rect.Height > po.Y;
        }
        public void setObjidList()
        {
            // TODO
            return;
            bool useFilter = true;
            if (!useFilter)
            {
                objectidList.Clear();
                for (int idIdx = 0; idIdx < ObjList.Count; idIdx++)
                {
                    ObjList[idIdx].currentAreaPositionIdx = 0;
                    ObjList[idIdx].currentImagePositionIdx = 0;
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
                }
            }
        }
        public Bitmap combinedImage(Bitmap back, Bitmap front, Rectangle position, float alpha, int frameNum)
        {
            try
            {
                if ((back != null) | (front != null))
                {
                    
                    using (Graphics gr = Graphics.FromImage(back))
                    {
                        //set alpha
                        ColorMatrix matrix = new ColorMatrix();
                        matrix.Matrix33 = alpha; //0.7~0.75
                        ImageAttributes att = new ImageAttributes();
                        att.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
                        gr.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;

                        //drawing time(frame num)
                        gr.DrawString(frameNum.ToString(), drawFont, drawBrush, position.X, position.Y-20);
                        //draw
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

                    SQL = string.Format("SELECT objectId, count(objectId) as cnt FROM rapidcheck.tracking where videoId={0} AND frameNum < {1} group by objectId;", videoid, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "objCnt");

                    dt = ds.Tables["maxid"];
                    int maxObjectid = 0;
                    foreach (DataRow dr in dt.Rows)
                    {
                        maxObjectid = Convert.ToInt32(dr["maxid"]);
                    }
                    //set ObjectidList
                    // setDefaultObjectidList(maxObjectid);
                    dt = ds.Tables["objCnt"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        if (Convert.ToInt32(dr["cnt"]) > minTrackingLength)
                        {
                            objectidList.Add(Convert.ToInt32(dr["objectId"]));
                        }
                    }
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
                        Bitmap bit = videoFrame.Clone(temp, videoFrame.PixelFormat);
                        ObjList[objid].addCropImage(bit);

                        if (ObjList[objid].cropImages.Count == 1)
                        {   
                            //set string
                            dataGridView.Invoke(new Action(() =>
                            {
                                string cont = string.Format("object id : {0}\nstart time : {1}\nend time : {2}\nmain color : {3}\ndirection : {4}", objid, "123", "345", "red", "31");

                                int gridHeight = 100;
                                Bitmap gridImg = new Bitmap(bit, new Size(bit.Width * gridHeight / bit.Height, gridHeight));
                                dataGridView.Rows.Add(gridImg, cont);
                                //dataGridView.Rows[dataGridView.RowCount - 1].Height = bit.Height;
                                dataGridView.Rows[dataGridView.RowCount - 1].Height = gridHeight;
                            }));
                        }
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
            double[][] points = new double[objectidList.Count][];

            for (int i = 0; i < objectidList.Count; i++)
            {
                int id = objectidList[i];
                points[i] = ObjList[id].getStartingPoint();
            }
            KMeansClusterCollection clusters = kmeas.Learn(points);
            int[] output = clusters.Decide(points);

            for (int i = 0; i < objectidList.Count; i++)
            {
                int id = objectidList[i];
                startingGroup[output[i]].Add(id);
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
        private double min(double num1, double num2)
        {
            return num1 > num2 ? num2 : num1;
        }
        public int getClickedObjectOriginalFrameNum(double clickPositionX, double clickPositionY)
        {
            /*
            int startFrame = -1;
            for(int idx = 0 ; idx < overlayOrders[resFrame].Count ; idx ++) // id의 인덱스
            {
                int id = overlayOrders[resFrame][idx].id;
                int orderingCnt = overlayOrders[resFrame][idx].orderingCnt;
                Rectangle objRect = ObjList[id].getCropArea(orderingCnt);
                if( (objRect.X < clickPositionX) && (objRect.Width+objRect.X > clickPositionX) && (objRect.Y < clickPositionY) && (objRect.Height+objRect.Y > clickPositionY))
                {
                    startFrame = ObjList[id].getStartFrameNum();
                    break;
                }
            }
            return startFrame;
             */
            int startFrame = -1;
            double maxClickedPositionArea = 0;
            for (int idx = 0; idx < overlayOrders[resFrame].Count; idx++) // id의 인덱스
            {
                int id = overlayOrders[resFrame][idx].id;
                int orderingCnt = overlayOrders[resFrame][idx].orderingCnt;
                Rectangle objRect = ObjList[id].getCropArea(orderingCnt);
                if ((objRect.X < clickPositionX) && (objRect.Width + objRect.X > clickPositionX) && (objRect.Y < clickPositionY) && (objRect.Height + objRect.Y > clickPositionY))
                {
                    double clickedPositionWidth = min(clickPositionX - objRect.X, objRect.Width + objRect.X - clickPositionX);
                    double clickedPositionHeight = min(clickPositionY - objRect.Y, objRect.Height + objRect.Y - clickPositionY);
                    double curClickedPositionArea = clickedPositionWidth * clickedPositionHeight;
                    if (maxClickedPositionArea < curClickedPositionArea)
                    {
                        startFrame = ObjList[id].getStartFrameNum();
                        maxClickedPositionArea = curClickedPositionArea;
                    }
                }
            }
            return startFrame;
        }
        public void setSpeed(int speed)
        {
            this.speed = speed;
        }
    }
}
