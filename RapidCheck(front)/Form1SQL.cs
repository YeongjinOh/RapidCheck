using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
//MYSQL
using MySql.Data.MySqlClient;

using System.Diagnostics; //Debug.WriteLine
namespace RapidCheck
{
    public partial class Form1
    {
        //MySQL
        private string strConn = "Server=localhost;Database=test;Uid=root;Pwd=1234;";

        private void sqlBtn_Click(object sender, EventArgs e)
        {
            MySqlConnection conn = new MySqlConnection(strConn);
            conn.Open();
            MySqlCommand cmd = new MySqlCommand("UPDATE test_table SET name='as22d' WHERE id=11", conn);
            cmd.ExecuteNonQuery();
            conn.Close();
        }
        private void sqlAdapterBtn_Click(object sender, EventArgs e)
        {
            try
            { 
                using (MySqlConnection conn = new MySqlConnection(strConn))
                {
                    conn.Open();
                    string sql = "SELECT * FROM test_table WHERE Id>=100";

                    //ExecuteReader를 이용하여
                    //연결 모드로 데이타 가져오기
                    MySqlCommand cmd = new MySqlCommand(sql, conn);
                    MySqlDataReader rdr = cmd.ExecuteReader();
                    while (rdr.Read())
                    {
                        //Console.WriteLine("{0}: {1}", rdr["id"], rdr["name"]);
                        //Debug.WriteLine("{0}: {1}", rdr["id"], rdr["name"]);
                        string temp = "id: " + rdr["id"] + "\npw: " + rdr["name"];
                        MessageBox.Show(temp);
                    }
                    rdr.Close();
                }           
            }
            catch(Exception )
            {
                MessageBox.Show("");
            }
        }   
    }
}
