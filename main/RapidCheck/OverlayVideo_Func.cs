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
using OxyPlot;
using OxyPlot.Series;
using OxyPlot.Axes;
namespace RapidCheck
{
    public partial class OverlayVideo
    {
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
                    string SQL = String.Format("SELECT EXISTS ( SELECT videoId FROM rapidcheck.file WHERE path=\"{0}\" AND {1} % frameStep = 0 AND maxFrameNum >= {2}) AS checkFlag;", videoPath, frameStep, maxFrameNum);
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
                        string insertCMD = String.Format("INSERT INTO rapidcheck.file(path, frameStep, maxFrameNum) VALUES ('{0}',{1}, {2});", videoPath, frameStep, maxFrameNum);
                        MySqlCommand cmd = new MySqlCommand(insertCMD, conn);
                        cmd.ExecuteNonQuery();
                    }

                    //SELECT
                    SQL = String.Format("SELECT videoId, status FROM rapidcheck.file WHERE path=\"{0}\" AND {1} % frameStep = 0 AND maxFrameNum >= {2}", videoPath, frameStep, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "videoid");
                    dt = ds.Tables["videoid"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        videoid = Convert.ToInt32(dr["videoid"]);
                        status = Convert.ToInt32(dr["status"]);
                    }
                    // detection
                    string dir = @"..\..\..\..\Detection_Engine\";
                    System.IO.Directory.SetCurrentDirectory(dir);
                    if (status == 0)
                    {
                        //연동하는부분
                        try
                        {
                            // TODO : read config file and change file path relatively
                            string pro = @"C:\Users\SoMa\Anaconda3\envs\venvJupyter\python.exe";
                            string modelPath = @"dropbox\models\train\rcnet-2class-base-from-voctrain\mydata-reversed-trainval-steps96000.h5";
                            string args = string.Format(@"C:\Users\SoMa\Desktop\RapidCheck\main\Detection_Engine\detection.py --videoId {0} --maxFrame {1} --videoPath {2} --frameSteps {3} --modelPath {4}", videoid, maxFrameNum, videoPath, frameStep, modelPath);
                            var p = new System.Diagnostics.Process();
                            p.StartInfo.FileName = pro;
                            p.StartInfo.Arguments = args;

                            p.StartInfo.CreateNoWindow = true;
                            p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                            p.StartInfo.RedirectStandardOutput = true;
                            p.StartInfo.UseShellExecute = false;
                            p.OutputDataReceived += processOutputHandler;

                            p.Start();
                            p.BeginOutputReadLine();
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
                            dir = @"..\Tracking_Engine\RapidCheck";
                            System.IO.Directory.SetCurrentDirectory(dir);

                            string pro = @"C:\Users\SoMa\Desktop\RapidCheck\main\Tracking_Engine\x64\Debug\Tracking_Engine.exe";
                            string args = string.Format("--videoId {0} --path {1} --maxFrameNum {2} --frameStep {3}", videoid, videoPath, maxFrameNum, frameStep);
                            var p = new System.Diagnostics.Process();
                            p.StartInfo.FileName = pro;
                            p.StartInfo.Arguments = args;
                            
                            p.StartInfo.RedirectStandardOutput = true;
                            
                            p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                            p.StartInfo.CreateNoWindow = true;
                            p.StartInfo.UseShellExecute = false;
                            p.OutputDataReceived += processOutputHandler;

                            p.Start();
                            p.BeginOutputReadLine();
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
                    

                    //----------------------------------------------overlay Module----------------------------------------------
                    SQL = string.Format("SELECT max(objectId) as maxid FROM rapidcheck.tracking where videoId={0} AND frameNum % {1} = 0 AND frameNum < {2};", videoid, frameStep, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "maxid");
                    
                    SQL = string.Format("SELECT objectId, frameNum, x, y, width, height FROM tracking where videoId={0} AND frameNum % {1} = 0 AND frameNum < {2} ORDER BY objectId ASC, frameNum ASC", videoid, frameStep, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "data");

                    SQL = string.Format("SELECT objectId, classId, direction13+direction14 as dir0, direction12+direction11 as dir1, direction10+direction9 as dir2, direction8+direction7 as dir3, direction6+direction5 as dir4, direction4+direction3 as dir5, direction2+direction1 as dir6, direction0+direction15 as dir7 FROM rapidcheck.objectinfo where videoId = {0};", videoid);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "classid");
                    
                    SQL = string.Format("SELECT objectId, frameNum FROM tracking WHERE videoId={0} AND frameNum % {1} = 0 AND frameNum < {2} ORDER BY frameNum ASC", videoid, frameStep, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "objidByframe");
                    
                    SQL = string.Format("SELECT objectId, count(objectId) as cnt FROM rapidcheck.tracking where videoId={0} AND frameNum % {1} = 0 AND frameNum < {2} group by objectId;", videoid, frameStep, maxFrameNum);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "objCnt");

                    SQL = string.Format("select classId, sum(color0) as color0, sum(color1) as color1, sum(color2) as color2, sum(color3)+sum(color4) as color3, sum(color5)+sum(color6) as color4, sum(color7) as color5, sum(color8)+sum(color9) as color6, sum(color10)+sum(color11) as color7, sum(color12) as color8, sum(color13) as color9 from rapidcheck.objectinfo where videoId = {0} group by classId;", videoid);
                    adapter.SelectCommand = new MySqlCommand(SQL, conn);
                    adapter.Fill(ds, "videoColorRatio");
                    
                    dt = ds.Tables["maxid"];
                    foreach (DataRow dr in dt.Rows)
                    {
                        this.maxObjectid = Convert.ToInt32(dr["maxid"]);
                    }
                    idxbyObjid = new List<int>();
                    for (int i = 0; i <= maxObjectid; i++) { idxbyObjid.Add(-1); }
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
                    //set class id table
                    dt = ds.Tables["classid"];
                    for (int i = 0; i < 8; i++)
                    {
                        directionCntCar.Add(0);
                        directionCntPeople.Add(0);
                    }
                        
                    foreach (DataRow dr in dt.Rows) //dictionay형태이며,,, key = id, value = classid
                    {
                        //trackingTableClassid.Add(Convert.ToInt32(dr["id"]), Convert.ToInt32(dr["classId"]));
                        int classid = Convert.ToInt32(dr["classId"]), maxDirid = 0;
                        trackingTableClassid[Convert.ToInt32(dr["objectId"])] = classid;
                        
                        for (int i = 1; i < 8; i++)
                        {
                            if (Convert.ToDouble(dr["dir" + maxDirid]) < Convert.ToDouble(dr["dir" + i]))
                            {
                                maxDirid = i;
                            }
                        }
                        trackingTableDirectionid[Convert.ToInt32(dr["objectId"])] = maxDirid;
                        if (classid == 0)
                        {
                            directionCntCar[maxDirid]++;
                        }
                        else if (classid == 1)
                        {
                            directionCntPeople[maxDirid]++;
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
             
                    dt = ds.Tables["videoColorRatio"];
                    for (int i = 0; i < 10; i++)
                    {
                        colorRatioCar.Add(0.0);
                        colorRatioPeople.Add(0.0);
                    }
                    foreach (DataRow dr in dt.Rows)
                    {
                        int classId = Convert.ToInt32(dr["classId"]);
                        if (classId == 0)
                        {
                            for (int i = 0; i < colorRatioCar.Count; i++)
                            {
                                colorRatioCar[i] = Convert.ToDouble(dr["color" + i]);
                            }
                        }
                        else if (classId == 1)
                        {
                            for (int i = 0; i < colorRatioPeople.Count; i++)
                            {
                                colorRatioPeople[i] = Convert.ToDouble(dr["color" + i]);
                            }
                        }
                    }
                }
            }
            catch (Exception)
            {
                Console.WriteLine("getMysqlObjList() ERROR");
            }
        }
        private void processOutputHandler(object sendingProcess, DataReceivedEventArgs outLine)
        {
            if (outLine.Data != null)
            {
                if (outLine.Data.Split(' ')[0] == "RapidCheck_Detection")
                {
                    int percent = Convert.ToInt32(outLine.Data.Split(' ')[1]);
                    labelProgress.Text = "Detection " + percent + "%";
                }
                else if (outLine.Data.Split(' ')[0] == "RapidCheck_Tracking")
                {
                    int percent = Convert.ToInt32(outLine.Data.Split(' ')[1]);
                    labelProgress.Text = "Detection 100%\nTracking " + percent + "%";
                }
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

            int cropMaxFrameNum = 0;
            for (int frameNum = 0; frameNum < maxFrameNum/*reader.FrameCount*/; frameNum += frameStep)
            {
                 if (objidByFrame.ContainsKey(frameNum))
                 {
                     cropMaxFrameNum = frameNum;
                 }
            }

            for (int frameNum = 0; frameNum < cropMaxFrameNum/*reader.FrameCount*/; frameNum++)
            {
                Bitmap videoFrame = reader.ReadVideoFrame();
                if (frameNum % frameStep != 0)
                {
                    videoFrame.Dispose();
                    continue;
                }
                dataGridView1.Invoke(new Action(() =>
                { 
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
                                //int gridHeight = 190;
                                int headlineHeight = 30;
                                Bitmap gridImg= null;
                                if (trackingTableClassid[objid] == 0)
                                {
                                    gridImg = (Bitmap)backgroundCar.Clone();
                                }
                                else
                                {
                                    gridImg = (Bitmap)backgroundPeople.Clone();
                                }
                                Bitmap headlineBox = new Bitmap(gridImg.Width, headlineHeight); // obj info
                                
                                //SET HEADLINEBOX COLOR WHITE
                                for(int i = 0;i < headlineBox.Height ; i++)
                                {
                                    for(int j = 0 ; j < headlineBox.Width ; j++)
                                    {
                                        headlineBox.SetPixel(j, i, Color.White);
                                    }
                                }


                                RectangleF rectf = new RectangleF(35, 0, headlineBox.Width, headlineBox.Height); // 다음 위치에 택스트를 그린다 (시간)

                                //alpha
                                ColorMatrix matrix = new ColorMatrix();
                                matrix.Matrix33 = 0.7f; //0.7~0.75
                                ImageAttributes att = new ImageAttributes();
                                att.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);

                                Graphics g = Graphics.FromImage(gridImg);
                                //System.Drawing.SolidBrush bgcolor = new System.Drawing.SolidBrush(System.Drawing.Color.White);
                                //g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;

                                double ratio = 1.0;
                                if (bit.Width * gridImg.Height < gridImg.Width * bit.Height)
                                {
                                    ratio = (double)gridImg.Height / bit.Height;

                                }
                                else
                                {
                                    ratio = (double)gridImg.Width / bit.Width;
                                }

                                int width = (int)(bit.Width * ratio);
                                int height = (int)(bit.Height * ratio);
                                Rectangle rect = new Rectangle((gridImg.Width - width)/2, (gridImg.Height- height)/2, width, height);
                                g.DrawImage(bit, rect);
                                
                                g.DrawImage(headlineBox, new Rectangle(0, 0, headlineBox.Width, headlineBox.Height), 0, 0, headlineBox.Width, headlineHeight, GraphicsUnit.Pixel, att);
                                
                                switch (trackingTableDirectionid[objid])
                                {
                                    case 0:
                                        g.DrawImage(Direction0, new Rectangle(0, 0, 30, 30));
                                        break;
                                    case 1:
                                        g.DrawImage(Direction1, new Rectangle(0, 0, 30, 30));
                                        break;
                                    case 2:
                                        g.DrawImage(Direction2, new Rectangle(0, 0, 30, 30));
                                        break;
                                    case 3:
                                        g.DrawImage(Direction3, new Rectangle(0, 0, 30, 30));
                                        break;
                                    case 4:
                                        g.DrawImage(Direction4, new Rectangle(0, 0, 30, 30));
                                        break;
                                    case 5:
                                        g.DrawImage(Direction5, new Rectangle(0, 0, 30, 30));
                                        break;
                                    case 6:
                                        g.DrawImage(Direction6, new Rectangle(0, 0, 30, 30));
                                        break;
                                    case 7:
                                        g.DrawImage(Direction7, new Rectangle(0, 0, 30, 30));
                                        break;
                                }
                                    


                                g.DrawString(ObjList[idxbyObjid[objid]].startTime.ToString("HH:mm"), new Font("SpoqaHanSans", 14, FontStyle.Bold), Brushes.Black, rectf);

                                if (trackingTableClassid[objid] == 0) // class id = 0 => people
                                {
                                    dataGridView1.Rows.Add(gridImg);
                                    dataGridView1.Rows[dataGridView1.RowCount - 1].Height = gridImg.Height;
                                    gridViewList1.Add(objid);
                                    
                                }
                                else if (trackingTableClassid[objid] == 1)
                                {
                                    dataGridView2.Rows.Add(gridImg);
                                    dataGridView2.Rows[dataGridView2.RowCount - 1].Height = gridImg.Height;
                                    gridViewList2.Add(objid);
                                }
                                else
                                {
                                    MessageBox.Show("DataGridView ERROR");
                                }
                            }
                        }
                    }
                    //videoFrame.Dispose();
                }));
                videoFrame.Dispose();
                int percent = frameNum * 100 / cropMaxFrameNum;
                labelProgress.Text = "Detection 100%\nTracking 100%\nOverlay " + percent + "%";
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
                        string SQL = string.Format("SELECT objectId FROM rapidcheck.objectinfo where videoId={0} AND objectId <= {1} {2} {3} {4};", videoid, maxObjectid, conditionTarget, conditionColor, conditionDirection);
                        MessageBox.Show(SQL);
                        adapter.SelectCommand = new MySqlCommand(SQL, conn);
                        adapter.Fill(ds, "objIds");
                        dt = ds.Tables["objIds"];
                        foreach (DataRow dr in dt.Rows)
                        {
                            tempObjidList.Add(Convert.ToInt32(dr["objectId"]));
                        }
                        objectidList = tempObjidList.Intersect(originObjectidList).ToList();
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

        public void objectClustering()
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
            labelProgress.Text = "Detection 100%\nTracking 100%\nOverlay 100%";
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
            labelProgress.Dispose();
        }
        public void overlayLive()
        {
            //background = new Bitmap(@"C:\videos\0.png"); //*****Background는....0번째 프레임?
            Graphics gs = pictureBoxVideo.CreateGraphics();
            
            int drawWidth = pictureBoxVideo.Width;
            int drawHeight = pictureBoxVideo.Height;
            //overlay time
            int frameTime = 1000 / frameStep;
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            float alphaMin = 0.5f, alphaDiff = 0.1f;
            //**********************DRAWING CODE**********************
            Accord.Video.FFMPEG.VideoFileReader reader = new Accord.Video.FFMPEG.VideoFileReader();
            reader.Open(videoPath);
            for (resFrame = 0; resFrame < outputFrameNum; resFrame++)
            {
                sw.Start();
                //background = reader.ReadVideoFrame();
                //Bitmap BitCopy = (Bitmap)background.Clone();
                Bitmap BitCopy = reader.ReadVideoFrame();
                int passTimeSec, frameHour, frameMin, frameSec;
                for (overlayObjIdx = 0; overlayObjIdx < overlayOrders[resFrame].Count; overlayObjIdx++)
                {
                    int id = overlayOrders[resFrame][overlayObjIdx].id;
                    int orderingCnt = overlayOrders[resFrame][overlayObjIdx].orderingCnt;
                    Rectangle currentObjectArea = ObjList[idxbyObjid[id]].getCropArea(orderingCnt);
                    float alpha = 0.8f;

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
                    // TODO
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
            reader.Close();
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
                Console.WriteLine(ex.ToString() + "\nImage combined Error");
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
        public DateTime getClickedGridViewOriginalDataTime(int id)
        {
            return ObjList[idxbyObjid[id]].startTime;
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
        public void barChartSetting()
        {

            modelBarChart = new PlotModel
            {
                Title = "방향",
                LegendPlacement = LegendPlacement.Outside,
                LegendPosition = LegendPosition.BottomCenter,
                LegendOrientation = LegendOrientation.Horizontal,
                LegendBorderThickness = 0
            };
            var s1 = new ColumnSeries { Title = "People", FillColor = OxyColor.FromRgb(83, 164, 188), StrokeColor = OxyColors.Black, StrokeThickness = 1, ColumnWidth = 50 };
            for (int i = 0; i < directionCntPeople.Count; i++)
            {
                s1.Items.Add(new ColumnItem { Value = directionCntPeople[i] });
            }
            var s2 = new ColumnSeries { Title = "Car", FillColor = OxyColor.FromRgb(67, 87, 99), StrokeColor = OxyColors.Black, StrokeThickness = 1, ColumnWidth = 50 };
            for (int i = 0; i < directionCntCar.Count; i++)
            {
                s2.Items.Add(new ColumnItem { Value = directionCntCar[i] });
            }
            var categoryAxis = new CategoryAxis { Position = AxisPosition.Bottom };
            categoryAxis.Labels.Add("↖");
            categoryAxis.Labels.Add("↑");
            categoryAxis.Labels.Add("↗");
            categoryAxis.Labels.Add("→");
            categoryAxis.Labels.Add("↘");
            categoryAxis.Labels.Add("↓");
            categoryAxis.Labels.Add("↙");
            categoryAxis.Labels.Add("←");
            var valueAxis = new LinearAxis { Position = AxisPosition.Left, MinimumPadding = 0, MaximumPadding = 0.06, AbsoluteMinimum = 0 };
            modelBarChart.Series.Add(s1);
            modelBarChart.Series.Add(s2);
            modelBarChart.Axes.Add(categoryAxis);
            modelBarChart.Axes.Add(valueAxis);
        }
        public void pieChartSetting()
        {
            //People
            modelLineChart = new PlotModel { Title = "Car" };
            dynamic seriesP1 = new PieSeries { StrokeThickness = 2.0, InsideLabelPosition = 0.8, AngleSpan = 360, StartAngle = 0 };

            double[] Hue = {1.0, 3.0, 5.0, 8.0, 12.0, 15.0, 18.0, 22.0};
            for (int i = 0; i < Hue.Length; i++)
            {
                seriesP1.Slices.Add(new PieSlice("Color" + i, colorRatioCar[i]) { IsExploded = false, Fill = OxyColor.FromHsv(Hue[i]/24, 0.7, 0.9) });
            }
            seriesP1.Slices.Add(new PieSlice("White", colorRatioCar[8]) { IsExploded = false, Fill = OxyColors.White });
            seriesP1.Slices.Add(new PieSlice("Black", colorRatioCar[9]) { IsExploded = false, Fill = OxyColors.Black });
            modelLineChart.Series.Add(seriesP1);
            //Car
            modelPieChartPeople = new PlotModel { Title = "People" };
            dynamic seriesP2 = new PieSeries { StrokeThickness = 2.0, InsideLabelPosition = 0.8, AngleSpan = 360, StartAngle = 0 };

            for (int i = 0; i < Hue.Length; i++)
            {
                seriesP2.Slices.Add(new PieSlice("Color" + i, colorRatioPeople[i]) { IsExploded = false, Fill = OxyColor.FromHsv(Hue[i] / 24, 0.7, 0.9) });
            }
            seriesP2.Slices.Add(new PieSlice("White", colorRatioPeople[8]) { IsExploded = false, Fill = OxyColors.White });
            seriesP2.Slices.Add(new PieSlice("Black", colorRatioPeople[9]) { IsExploded = false, Fill = OxyColors.Black });
            modelPieChartPeople.Series.Add(seriesP2);
        }
        public void LineChartSetting()
        {
            List<DataPoint> peopleCntList = new List<DataPoint>();
            List<DataPoint> carCntList = new List<DataPoint>();
            foreach (int i in objidByFrame.Keys) //i =framenum
            {
                int peopleCnt = 0;
                int carCnt = 0;
                foreach(int j in objidByFrame[i]) // j = obj num
                {
                    if(trackingTableClassid[j] == 0)
                    {
                        carCnt++;
                    }
                    else if (trackingTableClassid[j] == 1)
                    {
                        peopleCnt++;
                    }
                }
                peopleCntList.Add(new DataPoint((double)i, (double)peopleCnt));
                carCntList.Add(new DataPoint((double)i, (double)carCnt));
            }

            modelLineChart = new PlotModel { Title = "Frame" };
            var people = new LineSeries();
            for (int i = 0; i < peopleCntList.Count; i++)
            {
                people.Points.Add(peopleCntList[i]);
            }
            var car = new LineSeries();
            for (int i = 0; i < carCntList.Count; i++)
            {
                car.Points.Add(carCntList[i]);
            }
            modelLineChart.Series.Add(people);
            modelLineChart.Series.Add(car);
            
            //obj cnt
            
            for(int i = 0 ; i < peopleCntList.Count; i++)
            {
                peopleTotal += (int)peopleCntList[i].Y;
            }
            for (int i = 0; i < carCntList.Count; i++)
            {
                carTotal += (int)carCntList[i].Y;
            }
        }
    }
}
