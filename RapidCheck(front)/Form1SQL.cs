using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//MYSQL
using MySql.Data.MySqlClient;

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
    }
}
