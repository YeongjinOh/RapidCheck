﻿using System;
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
        public Rectangle getCropArea() { return cropAreas[currentAreaPositionIdx++]; }
        public Bitmap getNextCropImage() { return cropImages[currentImagePositionIdx++]; }
        public void addCropPositionNum(int frame) { cropPositionNum.Add(frame); }
        public void addCropImage(Bitmap cropImage)
        {
            cropImages.Add(cropImage);
        }
        //public bool emptyImage() //삭제 예정
        //{
        //    if (currentImagePositionIdx < cropImages.Count)
        //        return false;
        //    return true;
        //}
        public void addCropArea(Rectangle cropArea)
        {
            cropAreas.Add(cropArea);
        }
        public double[] getStartingPoint()
        {
            double[] ret = {cropAreas[0].X, cropAreas[0].Y};
            return ret;
        }
    }
}
