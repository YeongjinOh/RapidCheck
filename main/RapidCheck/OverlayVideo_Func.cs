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
using System.Diagnostics;
namespace RapidCheck
{
    public partial class OverlayVideo
    {
        //basic flow
        //getMysqlObjList
        //addObj
        //imageCrop

        //-----------여기가 분기 포인트.... 검색 조건 들어올시...---------//

        //kMeasFunc
        //buildOverlayOrderUsingCluster
        //overlayLive

        //****************************** Main function ******************************
        public void getMysqlObjList()
        {
            int status = 0;
            try
            {
                using (MySqlConnection conn = new MySqlConnection(strConn))
                {
                    //INTRO Check the video file
                    int checkFlag = 0;
                    DataSet ds = new DataSet();
                    MySqlDataAdapter adapter = new MySqlDataAdapter();
                    DataTable dt = new DataTable();
                    conn.Open();
                    string SQL = String.Format("select exists ( select videoId from rapidcheck.file where path=\"{0}\" and frameStep = {1}) as checkFlag;", videoPath, frameStep);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "checkFlag");
                    dt = ds.Tables["checkFlag"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        checkFlag = Convert.ToInt32(dr["checkFlag"]);
                    }
                    if(checkFlag == 0)
                    {
                        //INSERT
                        
                        string insertCMD = String.Format("INSERT INTO rapidcheck.file(path, frameStep) values ('{0}',{1});", videoPath, frameStep);
                        MySqlCommand cmd = new MySqlCommand(insertCMD, conn);
                        cmd.ExecuteNonQuery();
                    }

                    //SELECT
                    SQL = String.Format("Select videoId, status from rapidcheck.file where path=\"{0}\"", videoPath);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "videoid");
                    dt = ds.Tables["videoid"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        videoid = Convert.ToInt32(dr["videoid"]);
                        status = Convert.ToInt32(dr["status"]);
                    }

                    /***********************************************************************/status = 2;/***********************************************************************/
                    // detection
                    if (status == 0)
                    {
                        string dir = @"..\..\..\..\Detection_Engine\";
                        System.IO.Directory.SetCurrentDirectory(dir);
                        //연동하는부분
                        try
                        {
                            // TODO : read config file and change file path relatively
                            string pro = @"C:\Users\SoMa\Anaconda3\envs\venvJupyter\python.exe";
                            string args = string.Format(@"C:\Users\SoMa\Desktop\RapidCheck\main\Detection_Engine\detection.py --videoId {0} --maxFrame {1}", videoid, maxFrameNum);
                            var p = new System.Diagnostics.Process();
                            p.StartInfo.FileName = pro;
                            p.StartInfo.Arguments = args;
                            p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                            p.Start();
                            p.WaitForExit();

                            int result = p.ExitCode;
                            if (result == 0)
                            {
                                SQL = string.Format("UPDATE file SET status=1 WHERE videoId = {0};", videoid);
                                MySqlCommand cmd = new MySqlCommand(SQL, conn);
                                cmd.ExecuteNonQuery();
                                status = 1;
                            }
                            else
                            {
                                MessageBox.Show("DETECTION ERROR");
                            }
                        }
                        catch
                        {
                            MessageBox.Show("DETECTION ERROR");
                        }
                    }

                    if (status == 1)
                    {
                        //연동하는부분
                        try
                        {
                            string dir = @"..\Tracking_Engine\RapidCheck";
                            System.IO.Directory.SetCurrentDirectory(dir);

                            string pro = @"C:\Users\SoMa\Desktop\RapidCheck\main\Tracking_Engine\x64\Debug\Tracking_Engine.exe";
                            string args = string.Format("--videoId {0} --path {1} --maxFrameNum {2} --frameStep {3}", videoid, videoPath, maxFrameNum, frameStep);
                            var p = new System.Diagnostics.Process();
                            p.StartInfo.FileName = pro;
                            p.StartInfo.Arguments = args;
                            p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                            p.Start();
                            p.WaitForExit();
                            int result = p.ExitCode;
                            if (result == 0)
                            {
                                SQL = string.Format("UPDATE file SET status=2 WHERE videoId = {0};", videoid);
                                MySqlCommand cmd = new MySqlCommand(SQL, conn);
                                cmd.ExecuteNonQuery();
                                status = 2;
                            }
                            else
                            {
                                MessageBox.Show("TRACKING ERROR");
                            }
                        }
                        catch
                        {
                            MessageBox.Show("TRACKING ERROR");
                        }
                    }

                    // TODO
                    //videoid = 3;

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
                    foreach (DataRow dr in dt.Rows)
                    {
                        this.maxObjectid = Convert.ToInt32(dr["maxid"]);
                    }

                    idxbyObjid = new List<int>();
                    for (int i = 0; i <= maxObjectid; i++)
                    {
                        idxbyObjid.Add(-1);
                    }
                    //set ObjectidList
                    //--------잘 동작하는지 확인해볼것--- => ㅇㅇ잘동작함
                    dt = ds.Tables["objCnt"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        if (Convert.ToInt32(dr["cnt"]) > minTrackingLength)
                        {
                            int objid = Convert.ToInt32(dr["objectId"]);
                            objectidList.Add(objid);
                            originObjectidList.Add(objid); //copy
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
        
        public void addObj()
        {
            for (int idx = 0; idx < objectidList.Count; idx++)
            {
                int objid = objectidList[idx];
                idxbyObjid[objid] = idx;
                Obj obj = new Obj(objid);
                ObjList.Add(obj);
            }
            //objList 
            for (int row = 0; row < trackingTableObjid.Count; row++)
            {
                int objid = trackingTableObjid[row];
                int idx = idxbyObjid[objid];
                if (idx == -1) continue;
                Rectangle rect = trackingTableRectangle[row];
                modifyCropArea(ref rect);
                ObjList[idx].addCropArea(rect);
                ObjList[idx].addCropPositionNum(trackingTableFrameNum[row]);
                ObjList[idx].setStartTime(frameStep, frameRate, createTime);
            }
        }
        public void imageCrop()
        {
            Accord.Video.FFMPEG.VideoFileReader reader = new Accord.Video.FFMPEG.VideoFileReader();
            reader.Open(videoPath);
            for (int frameNum = 0; frameNum < maxFrameNum/*reader.FrameCount*/; frameNum++)
            {
                dataGridView.Invoke(new Action(() =>
                {
                    Bitmap videoFrame = reader.ReadVideoFrame();
                    if (objidByFrame.ContainsKey(frameNum))
                    {
                        foreach (int objid in objidByFrame[frameNum])
                        {
                            if (idxbyObjid[objid] == -1)
                            {
                                continue;
                            }
                            Rectangle temp = ObjList[idxbyObjid[objid]].getNextCropArea();
                            Bitmap bit = videoFrame.Clone(temp, videoFrame.PixelFormat);
                            ObjList[idxbyObjid[objid]].addCropImage(bit);

                            if (ObjList[idxbyObjid[objid]].cropImages.Count == 1)
                            {
                                int gridHeight = 200;
                                int headlineHeight = 20;
                                Bitmap gridImg = new Bitmap(bit, new Size(bit.Width * gridHeight / bit.Height, gridHeight)); // crop img
                                Bitmap headlineBox = new Bitmap(gridImg.Width, headlineHeight); // obj info
                                
                                //SET HEADLINEBOX COLOR WHITE
                                for(int i = 0;i < headlineBox.Height ; i++)
                                {
                                    for(int j = 0 ; j < headlineBox.Width ; j++)
                                    {
                                        headlineBox.SetPixel(j, i, Color.White);
                                    }
                                }


                                RectangleF rectf = new RectangleF(0, 0, headlineBox.Width, headlineBox.Height); // 다음 위치에 택스트를 그린다 (시간)

                                //alpha
                                ColorMatrix matrix = new ColorMatrix();
                                matrix.Matrix33 = 0.5f; //0.7~0.75
                                ImageAttributes att = new ImageAttributes();
                                att.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);

                                Graphics g = Graphics.FromImage(gridImg);
                                //System.Drawing.SolidBrush bgcolor = new System.Drawing.SolidBrush(System.Drawing.Color.White);
                                //g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                                g.DrawImage(headlineBox, new Rectangle(0, 0, headlineBox.Width, headlineBox.Height), 0, 0, headlineBox.Width, headlineHeight, GraphicsUnit.Pixel, att);
                                g.DrawString(ObjList[idxbyObjid[objid]].startTime.ToString("HH:mm"), new Font("Arial", 8), Brushes.Black, rectf);
                                
                                
                                //g.SmoothingMode = SmoothingMode.AntiAlias;
                                //g.InterpolationMode = InterpolationMode.HighQualityBicubic;
                                //g.PixelOffsetMode = PixelOffsetMode.HighQuality;
                                //g.DrawImage(bit, new Rectangle(0, 21, bit.Width * gridHeight / bit.Height, gridHeight));

                                dataGridView.Rows.Add(gridImg);
                                dataGridView.Rows[dataGridView.RowCount - 1].Height = gridImg.Height;
                            }
                        }
                    }
                    videoFrame.Dispose();
            }));
            }
            reader.Close();
        }
        public void setFileterObjectidList()
        {
            try
            {
                using (MySqlConnection conn = new MySqlConnection(strConn))
                {
                    if (maxObjectid != 0)
                    {
                        List<int> tempObjidList = new List<int>();

                        DataSet ds = new DataSet(); //이것들 전역변수로 선언해야하나..???
                        MySqlDataAdapter adapter = new MySqlDataAdapter();
                        DataTable dt = new DataTable();

                        //string SQL = string.Format("SELECT objectId FROM rapidcheck.objectinfo where videoId={0} AND objectId <= {1} and direction1 + direction2 + direction0 > 0.7;", videoid, maxObjectid);
                        //string SQL = string.Format("SELECT objectId FROM rapidcheck.objectinfo where videoId={0} AND objectId <= {1} and direction5 + direction7 + direction6 > 0.7;", videoid, maxObjectid);
                        string SQL = string.Format("SELECT objectId FROM rapidcheck.objectinfo where videoId={0} AND objectId <= {1} and classId = 0 {2};", videoid, maxObjectid, condition);
                        adapter.SelectCommand = new MySqlCommand(SQL, conn);
                        adapter.Fill(ds, "objIds");
                        dt = ds.Tables["objIds"];
                        foreach (DataRow dr in dt.Rows)
                        {
                            tempObjidList.Add(Convert.ToInt32(dr["objectId"]));
                        }
                        objectidList = tempObjidList.Intersect(objectidList).ToList();
                        if (objectidList.Count == 0)
                        {
                            MessageBox.Show("해당 조건에 맞는 대상이 없습니다.");
                            resetObjectidList();
                        }
                        else
                        {
                            //OrderingCnt초기화
                            resetOrdering();
                        }
                    }
                }
            }
            catch
            {
                MessageBox.Show("Database conn && Filtering Error");
            }
        }

        public void kMeasFunc()
        {
            int applyClusterNum = clusterNum;
            if (applyClusterNum > objectidList.Count)
                applyClusterNum = objectidList.Count;
            var kmeas = new KMeans(k: applyClusterNum);
            double[][] points = new double[objectidList.Count][];

            for (int i = 0; i < objectidList.Count; i++)
            {
                int id = objectidList[i];
                points[i] = ObjList[idxbyObjid[id]].getStartingPoint();
            }
            KMeansClusterCollection clusters = kmeas.Learn(points);
            int[] output = clusters.Decide(points);

            setStartingGroup();
            for (int i = 0; i < objectidList.Count; i++)
            {
                int id = objectidList[i];
                startingGroup[output[i]].Add(id);
            }
            //sort
            for (int k = 0; k < startingGroup.Count; k++)
            {
                startingGroup[k].sort(ref ObjList, ref idxbyObjid);
            }
        }
        public void buildOverlayOrderUsingCluster() // overlayOrders;
        {
            for (int overlayFrameNum = 0; overlayFrameNum < outputFrameNum; overlayFrameNum++)
            {
                List<objIdAndOrderingCnt> currentObjid = new List<objIdAndOrderingCnt>();
                for (int groupIdx = 0; groupIdx < startingGroup.Count; groupIdx++)
                {
                    if (startingGroup[groupIdx].hasNext())
                    {
                        int id = startingGroup[groupIdx].getNextId(ref ObjList, ref idxbyObjid);
                        int orderingCnt = ObjList[idxbyObjid[id]].OrderingCnt;
                        objIdAndOrderingCnt newOrder = new objIdAndOrderingCnt(id, orderingCnt);
                        currentObjid.Add(newOrder);
                        ObjList[idxbyObjid[id]].OrderingCnt++;
                    }
                }
                overlayOrders.Add(currentObjid);
            }
        }
        public void setPictureBoxSize()
        {
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
        }
        public void overlayLive()
        {
            background = new Bitmap(@"C:\videos\0.png"); //*****Background는....0번째 프레임?
            Graphics gs = pictureBoxVideo.CreateGraphics();
            startBtn.Text = "Pause";
            int drawWidth = pictureBoxVideo.Width;
            int drawHeight = pictureBoxVideo.Height;
            //overlay time
            int frameTime = 1000 / frameStep;
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            float alphaMin = 0.5f, alphaDiff = 0.3f;
            //**********************DRAWING CODE**********************
            for (resFrame = 0; resFrame < outputFrameNum; resFrame++)
            {
                sw.Start();
                Bitmap BitCopy = (Bitmap)background.Clone();
                int passTimeSec, frameHour, frameMin, frameSec;
                for (overlayObjIdx = 0; overlayObjIdx < overlayOrders[resFrame].Count; overlayObjIdx++)
                {
                    int id = overlayOrders[resFrame][overlayObjIdx].id;
                    int orderingCnt = overlayOrders[resFrame][overlayObjIdx].orderingCnt;
                    Rectangle currentObjectArea = ObjList[idxbyObjid[id]].getCropArea(orderingCnt);
                    float alpha = 0.9f;

                    for (int prevOverlayObjIdx = 0; prevOverlayObjIdx < overlayObjIdx; prevOverlayObjIdx++)
                    {
                        int previd = idxbyObjid[overlayOrders[resFrame][prevOverlayObjIdx].id];
                        int prevOrderingCnt = overlayOrders[resFrame][prevOverlayObjIdx].orderingCnt;
                        Rectangle prevObjectArea = ObjList[previd].getCropArea(prevOrderingCnt);

                        if (isIntersect(currentObjectArea, prevObjectArea))
                        {
                            alpha -= alphaDiff;
                        }
                    }
                    if (alpha < alphaMin)
                        alpha = alphaMin;
                    int currentFrameNum = ObjList[idxbyObjid[id]].getStartFrameNum() + frameStep * orderingCnt;
                    passTimeSec = currentFrameNum / frameRate;
                    frameHour = passTimeSec / 3600;
                    passTimeSec = passTimeSec % 3600;
                    DateTime printTime = createTime;
                    printTime = printTime.AddHours(frameHour);
                    frameMin = passTimeSec / 60;
                    passTimeSec = passTimeSec % 60;
                    printTime = printTime.AddMinutes(frameMin);
                    frameSec = passTimeSec;
                    printTime = printTime.AddSeconds(frameSec);
                    BitCopy = combinedImage(BitCopy, ObjList[idxbyObjid[id]].getCropImage(orderingCnt), currentObjectArea, alpha, printTime.ToString("HH:mm:ss"));
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

        //******************************SubFunction******************************        
        public void modifyCropArea(ref Rectangle cropArea)
        {
            cropArea.X -= 15;
            cropArea.Y -= 20;
            cropArea.Width += 30;
            cropArea.Height += 40;

            if (cropArea.X < 0)
            {
                cropArea.Width += cropArea.X;
                cropArea.X = 0;
            }
            if (cropArea.Y < 0)
            {
                cropArea.Height += cropArea.Y;
                cropArea.Y = 0;
            }
            if (cropArea.X > videoWidth) cropArea.X /= 2;
            if (cropArea.Y > videoHeight) cropArea.Y /= 2;
            if (cropArea.X > videoWidth) cropArea.X = videoWidth - 1;
            if (cropArea.Y > videoHeight) cropArea.Y = videoHeight - 1;
            if (cropArea.X + cropArea.Width > videoWidth) cropArea.Width = videoWidth - cropArea.X;
            if (cropArea.Y + cropArea.Height > videoHeight) cropArea.Height = videoHeight - cropArea.Y;
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
        public Bitmap combinedImage(Bitmap back, Bitmap front, Rectangle position, float alpha, string time = null)
        {
            int min_diff = 1300;
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
                        Bitmap diffBmp = back.Clone(position, back.PixelFormat);
                        for (int y = 0; y < diffBmp.Height; y++)
                        {
                            for (int x = 0; x < diffBmp.Width; x++)
                            {
                                // Calculate the pixels' difference.
                                Color color1 = front.GetPixel(x, y);
                                Color color2 = diffBmp.GetPixel(x, y);

                                int diff = (color1.R - color2.R) * (color1.R - color2.R) +
                                        (color1.G - color2.G) * (color1.G - color2.G)+
                                        (color1.B - color2.B) * (color1.B - color2.B);
                                if (diff >= min_diff)
                                {
                                    diffBmp.SetPixel(x, y, color1);
                                }                                
                                else
                                {
                                    diffBmp.SetPixel(x, y, color2);
                                }
                                
                            }
                        }

                            //draw
                        gr.DrawImage(diffBmp, position, 0, 0, front.Width, front.Height, GraphicsUnit.Pixel, att);
                        
                        //drawing time(frame num)
                        if(time != null & drawTime)
                        {
                            gr.DrawString(time, drawFont, drawBrush, position.X + position.Width/2 - 50, position.Y + position.Height/2 - 10);
                        }
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
        public int getClickedObjectOriginalFrameNum(double clickPositionX, double clickPositionY)
        {
            int startFrame = -1;
            double maxClickedPositionArea = 0;
            for (int idx = 0; idx < overlayOrders[resFrame].Count; idx++) // id의 인덱스
            {
                int id = overlayOrders[resFrame][idx].id;
                int orderingCnt = overlayOrders[resFrame][idx].orderingCnt;
                Rectangle objRect = ObjList[idxbyObjid[id]].getCropArea(orderingCnt);
                if ((objRect.X < clickPositionX) && (objRect.Width + objRect.X > clickPositionX) && (objRect.Y < clickPositionY) && (objRect.Height + objRect.Y > clickPositionY))
                {
                    double clickedPositionWidth = min(clickPositionX - objRect.X, objRect.Width + objRect.X - clickPositionX);
                    double clickedPositionHeight = min(clickPositionY - objRect.Y, objRect.Height + objRect.Y - clickPositionY);
                    double curClickedPositionArea = clickedPositionWidth * clickedPositionHeight;
                    if (maxClickedPositionArea < curClickedPositionArea)
                    {
                        startFrame = ObjList[idxbyObjid[id]].getStartFrameNum();
                        maxClickedPositionArea = curClickedPositionArea;
                    }
                }
            }
            return startFrame;
        }
        private double min(double num1, double num2) { return num1 > num2 ? num2 : num1; }
        private void setStartingGroup()
        {
            startingGroup.Clear();
            for (int i = 0; i < clusterNum; i++)
            {
                startingGroup.Add(new StartingGroup());
            }
        }
        public void resetObjectidList()
        {
            objectidList = originObjectidList.GetRange(0, originObjectidList.Count);
            resetOrdering();
        }
        private void resetOrdering()
        {
            foreach (int i in objectidList)
            {
                ObjList[idxbyObjid[i]].OrderingCnt = 0;
            }
            overlayOrders.Clear();
        }
        //******************************Not used******************************
        //public void buildOverlayOrder() // 애를 만든다. overlayOrders;
        //{
        //    //일단은 모든 obj들을 0frame부터 합성
        //    int offset = 10;
        //    int group = 3;
        //    for (int overlayFrameNum = 0; overlayFrameNum < outputFrameNum; overlayFrameNum++)
        //    {
        //        List<objIdAndOrderingCnt> temp = new List<objIdAndOrderingCnt>();
        //        for (int objectidListIdx = 0; (objectidListIdx < objectidList.Count) && (overlayFrameNum >= (objectidListIdx / group) * offset); objectidListIdx++)
        //        {
        //            int id = objectidList[objectidListIdx];
        //            int orderingCnt = ObjList[idxbyObjid[id]].OrderingCnt;
        //            objIdAndOrderingCnt newOrder = new objIdAndOrderingCnt(id, orderingCnt);
        //            temp.Add(newOrder);
        //            ObjList[idxbyObjid[id]].OrderingCnt++;
        //        }
        //        overlayOrders.Add(temp);
        //    }
        //}
        //public void addOverlayFrames(Bitmap frame)
        //{
        //    overlayFrames.Add(frame);
        //}
        //public void setResFrame(int resFrameNum) { this.resFrame = resFrameNum; }

        //public void overlay()
        //{
        //    //Bitmap background = new Bitmap(@"C:\videos\0.png"); //*****Background는....0번째 프레임?
        //    VideoFileWriter writer = new VideoFileWriter();
        //    string outputPath = string.Format(@"C:\videos\output\video{0}_{1}_{2}_{3}.avi", videoid, maxFrameNum, outputFrameNum, clusterNum);
        //    if (System.IO.File.Exists(outputPath)) //해당 이름을 가진 파일이 존재한다면,,,,
        //    {
        //        outputPath = outputPath.Replace(".avi", string.Format("_{0}.avi", System.DateTime.Now.ToString("_hh시mm분")));
        //    }
        //    writer.Open(outputPath, videoWidth, videoHeight, frameStep, VideoCodec.H264);
        //    for (int resFrame = 0; resFrame < outputFrameNum; resFrame++)
        //    {
        //        Bitmap BitCopy = (Bitmap)background.Clone();
        //        for (int idx = 0; idx < overlayOrders[resFrame].Count; idx++)
        //        {
        //            int id = overlayOrders[resFrame][idx].id;
        //            BitCopy = combinedImage(BitCopy, ObjList[idxbyObjid[id]].getNextCropImage(), ObjList[idxbyObjid[id]].getNextCropArea(), 0.75f);
        //        }
        //        writer.WriteVideoFrame(BitCopy);
        //        BitCopy.Dispose();
        //    }
        //    writer.Close();
        //}

        //public void setObjidList()
        //{
        //    // TODO
        //    return;
        //    bool useFilter = true;
        //    if (!useFilter)
        //    {
        //        objectidList.Clear();
        //        for (int idIdx = 0; idIdx < ObjList.Count; idIdx++)
        //        {
        //            ObjList[idIdx].currentAreaPositionIdx = 0;
        //            ObjList[idIdx].currentImagePositionIdx = 0;
        //            objectidList.Add(idIdx);
        //        }
        //    }
        //    else
        //    {
        //        List<int> temp = new List<int>();
        //        using (MySqlConnection conn = new MySqlConnection(strConn))
        //        {
        //            DataSet ds = new DataSet();
        //            MySqlDataAdapter adapter = new MySqlDataAdapter();
        //            DataTable dt = new DataTable();

        //            //string SQL = String.Format("SELECT objectId FROM rapidcheck.objectinfo where direction1 + direction2 > 0.7 and videoId={0};", videoid);
        //            string SQL = String.Format("SELECT * FROM rapidcheck.objectinfo where videoId={0} AND objectId <= {1};", videoid, ObjList.Count);

        //            adapter.SelectCommand = new MySqlCommand(SQL, conn);
        //            adapter.Fill(ds, "directionid");
        //            dt = ds.Tables["directionid"];
        //            foreach (DataRow dr in dt.Rows)
        //            {
        //                temp.Add(Convert.ToInt32(dr["objectId"]));
        //            }
        //        }
        //        List<int> store = new List<int>();
        //        for (int i = 0; i < objectidList.Count; i++)
        //        {
        //            for (int j = 0; j < temp.Count; j++)
        //            {
        //                if (objectidList[i] == temp[j])
        //                {
        //                    store.Add(objectidList[i]);
        //                    break;
        //                }
        //            }
        //        }
        //        objectidList.Clear();
        //        foreach (int id in store)
        //        {
        //            objectidList.Add(id);
        //            ObjList[idxbyObjid[id]].currentAreaPositionIdx = 0;
        //            ObjList[idxbyObjid[id]].currentImagePositionIdx = 0;
        //        }
        //    }
        //}
        
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
    }
}
