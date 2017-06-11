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
        public int objectid { get; set; }
        public int currentAreaPositionIdx { get; set; }
        public int currentImagePositionIdx { get; set; }
        private List<int> cropPositionNum;
        private List<Rectangle> cropAreas;
        public List<Bitmap> cropImages;
        public int OrderingCnt { get; set; }
        public DateTime startTime { get; set; }

        public Obj() { }
        public Obj(int id)
        {
            objectid = id;
            currentAreaPositionIdx = 0;
            currentImagePositionIdx = 0;
            OrderingCnt = 0;
            cropPositionNum = new List<int>();
            cropAreas = new List<Rectangle>();
            cropImages = new List<Bitmap>();
        }
        public int getLength()
        {
            return cropAreas.Count;
        }
        public int getStartFrameNum()
        {
            return cropPositionNum[0];
        }
    }
}