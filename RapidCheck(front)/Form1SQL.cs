using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
//MYSQL
using MySql.Data.MySqlClient;
using System.Data;

namespace RapidCheck
{
    public partial class Form1
    {
        //Tracking Table store
        struct tracking
        {
            public int videoId;
            public int objId;
            public int frameNum;
            public int x;
            public int y;
            public int w;
            public int h;
        }
        int nrow; //nrow = tracking[Array Size]

        //MySQL setting
        private string strConn = "Server=localhost;Database=test;Uid=root;Pwd=1234;";
        
        private void sqlAdapterBtn_Click(object sender, EventArgs e)
        {
            sqlAdapter();
        }   

        //데이터의 열을 가져와서 열만큼 arr생성하고, 생성한 arr 초기화
        private void sqlAdapter()
        {
            try
            {
                using (MySqlConnection conn = new MySqlConnection(strConn))
                {
                    DataSet ds = new DataSet();

                    //table row
                    MySqlDataAdapter adapter = new MySqlDataAdapter();
                    adapter.SelectCommand = new MySqlCommand("SELECT count(*) FROM tracking;", conn);
                    adapter.Fill(ds, "nrow");

                    //tracking data
                    adapter.SelectCommand = new MySqlCommand("SELECT * FROM tracking", conn);
                    adapter.Fill(ds, "data");

                    //set nrow
                    DataTable dtCnt = new DataTable();
                    dtCnt = ds.Tables["nrow"];
                    foreach (DataRow dr in dtCnt.Rows)
                    {
                        nrow = Int32.Parse(dr["count(*)"].ToString());
                    }
                    //make array
                    tracking[] trackingData = new tracking[nrow];

                    //get tracking data
                    DataTable dt = new DataTable();
                    dt = ds.Tables["data"];
                    //set tracking data
                    int index = 0;
                    foreach (DataRow dr in dt.Rows)
                    {
                        trackingData[index].videoId = Convert.ToInt32(dr["videoId"]);
                        trackingData[index].objId = Convert.ToInt32(dr["objectId"]);
                        trackingData[index].frameNum = Convert.ToInt32(dr["frameNum"]);
                        trackingData[index].x = Convert.ToInt32(dr["x"]);
                        trackingData[index].y = Convert.ToInt32(dr["y"]);
                        trackingData[index].w = Convert.ToInt32(dr["width"]);
                        trackingData[index].h = Convert.ToInt32(dr["height"]);
                        index += 1;
                    }
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Adapter Error");
            }
        }
    }
}
