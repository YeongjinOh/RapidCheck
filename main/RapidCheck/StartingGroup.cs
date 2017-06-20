using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RapidCheck
{
    class StartingGroup
    {
        public List<int> idList { get; set; }
        public List<int> lengthList{get;set;}
        private int currentidPosition; //0 ~ image length
        private int currentIndex; //index of id list
  
        public StartingGroup() 
        {
            idList = new List<int>();
            lengthList = new List<int>();
            currentidPosition = 0;
            currentIndex = 0;
        }
        public int getNextId(ref List<Obj> objList, ref List<int> idxbyObjid)
        {
            int objId = idList[currentIndex];
            currentidPosition++;
            if (objList[idxbyObjid[objId]].getLength() == currentidPosition)
            {
                currentIndex++;
                currentidPosition = 0;
            }
            return objId;
        }
        public bool hasNext()
        {
            return (currentIndex < idList.Count);
        }
        public void Add(int id)
        {
            idList.Add(id);
        }
        public int getFrameLength()
        {
            return lengthList.Sum();
        }
        public void sort(ref List<Obj> ObjList, ref List<int> idxbyObjid)
        {
            for (int idx=0; idx < idList.Count; idx++)
            {
                int objId = idList[idx]; //idxbyObjid
                lengthList.Add(ObjList[idxbyObjid[objId]].getLength());
            }

            for (int i = 0; i < lengthList.Count; i++)
            {
                for (int j=i; j>0 && lengthList[j] > lengthList[j-1]; j--)
                {
                    int tmp = lengthList[j];
                    lengthList[j] = lengthList[j - 1];
                    lengthList[j - 1] = tmp;
                    tmp = idList[j];
                    idList[j] = idList[j - 1];
                    idList[j - 1] = tmp;
                }
            }
        }
    }
}
