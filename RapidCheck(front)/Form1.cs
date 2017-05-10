using MaterialSkin;
using MaterialSkin.Controls;
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

//
//using SharpDX.MediaFoundation;

namespace RapidCheck
{
    public partial class Form1 : MaterialForm
    {
        private Size formSize;
        private Size pnlSize;
        public Form1()
        {
            InitializeComponent();
            var skinManager = MaterialSkinManager.Instance;
            skinManager.AddFormToManage(this);
            //skinManager.Theme = MaterialSkinManager.Themes.DARK;
            skinManager.Theme = MaterialSkinManager.Themes.LIGHT;
            skinManager.ColorScheme = new ColorScheme(Primary.BlueGrey800, Primary.BlueGrey900, Primary.BlueGrey500, Accent.LightBlue200, TextShade.WHITE);
            
            tabPage1.Text = "검색";
            tabPage2.Text = "요약";
            tabPage3.Text = "영상";

            //SharpDX.MediaFoundation.VideoDisplayControl test;
            //SharpDX.MediaFoundation.VideoNormalizedRect temp;
            //temp.Bottom = 30;
            //temp.Left = 20;
            //temp.Right = 50;
            //temp.Top = 10;

            //test.VideoWindow = tabPage2;
            //test.SetVideoPosition(temp,null);
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            formSize = new Size(this.Width, this.Height);
            pnlSize = new Size(tabPage3.Width, tabPage3.Height);
        }
    }
}
