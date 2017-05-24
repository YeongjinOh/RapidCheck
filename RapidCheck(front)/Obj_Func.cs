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
        public Rectangle getCropArea() { return cropAreas[currentAreaPositionIdx++]; }
        public Bitmap getNextCropImage() { return cropImages[currentImagePositionIdx++]; }
        public void addCropPositionNum(int frame) { cropPositionNum.Add(frame); }
        public void addCropImage(Bitmap cropImage)
        {
            cropImageLength++;
            cropImages.Add(cropImage);
        }
        public bool emptyImage()
        {
            if (currentImagePositionIdx < cropImages.Count)
                return false;
            return true;
        }
        public void addCropArea(Rectangle cropArea)
        {
            //cropArea.X -= 10;
            //cropArea.Y -= 20;
            //cropArea.Width += 20;
            //cropArea.Height += 20;

            ////일단은... width x height
            //if (cropArea.X > width) cropArea.X /= 2;
            //if (cropArea.Y > height) cropArea.Y /= 2;
            ////if (cropArea.X < 0) cropArea.X = 0;
            ////if (cropArea.Y < 0) cropArea.Y = 0;
            //if (cropArea.X + cropArea.Width > width) cropArea.Width = width - cropArea.X;
            //if (cropArea.Y + cropArea.Height > height) cropArea.Height = height - cropArea.Y;

            //if (cropArea.Width == 0) { System.Windows.Forms.MessageBox.Show(cropArea.ToString() + "\n" + this.objectid.ToString()); }
            cropAreas.Add(cropArea);
        }
    }
}
