using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing; //Bitmap

namespace RapidCheck
{
    public partial class Obj
    {
        public int getFrameCnt() { return cropImages.Count; }
        public Rectangle getNextCropArea() { return cropAreas[currentAreaPositionIdx++]; }
        public Bitmap getNextCropImage() { return cropImages[currentImagePositionIdx++]; }
        public void addCropPositionNum(int frame) { cropPositionNum.Add(frame); }
        public void addCropImage(Bitmap cropImage)
        {
            cropImages.Add(cropImage);
        }
        public Bitmap getCropImage(int index)
        {
            currentImagePositionIdx = index;
            return getNextCropImage();
        }
        public Rectangle getCropArea(int index)
        {
            currentAreaPositionIdx = index;
            return getNextCropArea();
        }
        public void addCropArea(Rectangle cropArea)
        {
            cropAreas.Add(cropArea);
        }
        public double[] getStartingPoint()
        {
            //int index = cropAreas.Count-1;
            int XWidth = cropAreas[0].X + cropAreas[0].Width;
            int YHeight = cropAreas[0].Y + cropAreas[0].Height;
            double[] ret = {XWidth, YHeight};
            return ret;
        }
        public void setStartTime(int frameStep, int frameRate, DateTime createTime)
        {
            int passTimeSec, frameHour, frameMin, frameSec;

            int currentFrameNum = cropPositionNum[0] + frameStep;
            passTimeSec = currentFrameNum / frameRate;
            frameHour = passTimeSec / 3600;
            passTimeSec = passTimeSec % 3600;
            DateTime startTime = createTime;
            startTime = startTime.AddHours(frameHour);
            frameMin = passTimeSec / 60;
            passTimeSec = passTimeSec % 60;
            startTime = startTime.AddMinutes(frameMin);
            frameSec = passTimeSec;
            startTime = startTime.AddSeconds(frameSec);

            this.startTime = startTime;
        }
    }
}
