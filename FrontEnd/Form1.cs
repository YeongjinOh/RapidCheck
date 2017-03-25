using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;

namespace FrontEnd
{
    public partial class Form1 : Form
    {
        imgProcessing.Class1 myclass = new imgProcessing.Class1();
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            listBox1.ValueMember = "Path";
            listBox1.DisplayMember = "FileName";
        }
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            MediaFile file = listBox1.SelectedItem as MediaFile;
            if (file != null)
            {
                axWindowsMediaPlayer1.URL = file.Path;
                axWindowsMediaPlayer1.Ctlcontrols.play();
            }
        }

        private void bunifuFlatButton1_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog ofd = new OpenFileDialog() { Multiselect = true, ValidateNames = true, Filter = "Files|*.*" })
            {
                ofd.InitialDirectory = @"C:\Users\trevor\Desktop\Videos";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    List<MediaFile> files = new List<MediaFile>();
                    foreach (System.String fileName in ofd.FileNames)
                    {
                        FileInfo fi = new FileInfo(fileName);
                        files.Add(new MediaFile() { FuleName = Path.GetFileNameWithoutExtension(fi.FullName), Path = fi.FullName });
                    }
                    listBox1.DataSource = files;
                }
            }
        }

        private void bunifuImageButton1_Click(object sender, EventArgs e)
        {
            Environment.Exit(0);
        }
       
    }
}
