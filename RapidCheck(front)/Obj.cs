using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Collections.Generic; //List
using System.Drawing; //Bitmap

namespace RapidCheck
{
    public class Obj
    {
        public int objectid { get; set; }
        public int currentAreaPositionIdx { get; set; }
        public int currentImagePositionIdx { get; set; }
        private List<int> cropPositionNum; //자르는 위치.... 필요 없음...아마도
        private List<Rectangle> cropAreas;
        private List<Bitmap> cropImages;
        public int cropImageLength { get; set; }

        public Obj()
        {
            currentAreaPositionIdx = 0;
            currentImagePositionIdx = 0;
            cropPositionNum = new List<int>();
            cropAreas = new List<Rectangle>();
            cropImages = new List<Bitmap>();
            cropImageLength = 0;
        }
        public Obj(int id)
        {
            objectid = id;
            currentAreaPositionIdx = 0;
            currentImagePositionIdx = 0;
            cropPositionNum = new List<int>();
            cropAreas = new List<Rectangle>();
            cropImages = new List<Bitmap>();
            cropImageLength = 0;
        }
        public int getFrameCnt()
        {
            return cropImages.Count;
        }
        public void addCropPositionNum(int frame)
        {
            cropPositionNum.Add(frame);
        }
        public void addCropArea(Rectangle cropArea)
        {
            cropArea.X -= 10;
            cropArea.Y -= 20;
            cropArea.Width += 20;
            cropArea.Height += 20;

            //일단은... 960 x 720
            if (cropArea.X > 960) cropArea.X /= 2;
            if (cropArea.Y > 720) cropArea.Y /= 2;
            //if (cropArea.X < 0) cropArea.X = 0;
            //if (cropArea.Y < 0) cropArea.Y = 0;
            if (cropArea.X + cropArea.Width > 960) cropArea.Width = 960- cropArea.X;
            if (cropArea.Y + cropArea.Height > 720) cropArea.Height = 720 - cropArea.Y;

            if (cropArea.Width == 0) { System.Windows.Forms.MessageBox.Show(cropArea.ToString() + "\n" + this.objectid.ToString()); }
            cropAreas.Add(cropArea);
        }
        public Rectangle getCropArea()
        {
            return cropAreas[currentAreaPositionIdx++];
        }
        public Bitmap getNextCropImage()
        {
            return cropImages[currentImagePositionIdx++];
        }
        public bool emptyImage()
        {
            if (currentImagePositionIdx < cropImages.Count)
                return false;
            return true;
        }
        public void addCropImage(Bitmap cropImage)
        {
            cropImageLength++;
            cropImages.Add(cropImage);
        }
    }
}