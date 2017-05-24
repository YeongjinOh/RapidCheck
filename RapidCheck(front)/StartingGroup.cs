using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RapidCheck
{
    class StartingGroup
    {
        private List<int> idList;
        private int currentidPosition; //0 ~ image length
        private int currentIndex; //index of id list


        public StartingGroup() 
        {
            idList = new List<int>();
            currentidPosition = 0;
            currentIndex = 0;
        }
        public int getNextId(ref List<Obj> objList)
        {
            int objId = idList[currentIndex];
            currentidPosition++;
            if (objList[objId].cropImageLength == currentidPosition)
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
    }
}
