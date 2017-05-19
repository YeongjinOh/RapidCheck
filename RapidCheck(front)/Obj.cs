using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Collections.Generic; //List
using System.Drawing; //Bitmap

namespace RapidCheck
{
    class Obj
    {
        public int objectid { get; set; }
        public int currentPositionIdx { get; set; } //그리는 위치
        private List<int> frameCropPosition; //자르는 위치
        private List<Rectangle> cropAreas;
        private List<Bitmap> cropImages;

        Obj()
        {
            currentPositionIdx = 0;
        }
        Obj(Rectangle cropArea)
        {
            currentPositionIdx = 0;
            addCropArea(cropArea);
        }
        public void addCropArea(Rectangle cropArea)
        {
            //x -= 10;
            //y -= 20;
            //w += 20;
            //h += 20;
		    
            //if (x > backgroundWidth) x = backgroundWidth;
            //if (x < 0) x = 0;
            //if (y < 0) y = 0;
            //if (y > backgroundHeight) y = backgroundHeight;
            //if (x + w > backgroundWidth) w = backgroundWidth - x;
            //if (y + h > backgroundHeight) h = backgroundHeight - y;
            cropAreas.Add(cropArea);
        }
        public Rectangle getCropArea(int idx)
        {
            return cropAreas[idx];
        }
        public Bitmap getNextCropImage()
        {
            return cropImages[currentPositionIdx++];
        }
        public void addCropImage(Bitmap cropImage)
        {
            cropImages.Add(cropImage); //Add는 배열 마지막에 삽입
        }
        public void addCropImage(Bitmap img)
        {
            cropImages.Add(img);
        }
    }
}