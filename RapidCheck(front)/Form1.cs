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

//DirectX
using Microsoft.DirectX.AudioVideoPlayback;
using Microsoft.DirectX;
//MYSQL
using MySql.Data.MySqlClient;

//CMD command
using System.Diagnostics;

//overlay
using DirectXOverlayWindow;

//picker
//using Windows.Media.Editing;
//using Windows.Media.Core;
//using Windows.Media.Playback;
using System.Threading.Tasks;

namespace RapidCheck
{
    public partial class Form1 : MaterialForm
    {
        //DirectX
        private Video video;
        private Size formSize;
        private Size pnlSize;
        
        //MySQL
        private string strConn = "Server=localhost;Database=test;Uid=root;Pwd=1234;";

        //overlay test
        private Bitmap overlay = null;

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

           

            //SQL
            //MySqlConnection conn = new MySqlConnection(strConn);
            //conn.Open();
            //MySqlCommand cmd = new MySqlCommand("INSERT INTO test_table VALUES (11, 'tasdad1')", conn);
            //cmd.ExecuteNonQuery();
            //conn.Close();

        }
        private void Form1_Load(object sender, EventArgs e)
        {
            formSize = new Size(this.Width, this.Height);
            pnlSize = new Size(tabPage3.Width, tabPage3.Height);
        }
        private void videoBtn_Click(object sender, EventArgs e)
        {
            MySqlConnection conn = new MySqlConnection(strConn);
            conn.Open();
            MySqlCommand cmd = new MySqlCommand("UPDATE test_table SET name='sss' WHERE id=11", conn);
            cmd.ExecuteNonQuery();
            conn.Close();
        }
        private void materialFlatButton2_Click(object sender, EventArgs e)
        {
            openFileDialog1.InitialDirectory = @"C:\Users\trevor\Desktop\Videos";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                // open the video
                // remember the original dimensions of the panel
                int height = videoPanel.Height;
                int width = videoPanel.Width;
                // dispose of the old video to clean up resources
                if (video != null)
                {
                    video.Dispose();
                }
                try
                {
                    // open a new video
                    video = new Video(openFileDialog1.FileName);
                    // assign the win form control that will contain the video
                    video.Owner = videoPanel;
                    // resize to fit in the panel
                    videoPanel.Width = width;
                    videoPanel.Height = height;
                    // play the first frame of the video so we can identify it
                    video.Play();
                    video.CurrentPosition = 200/60;
                    //video.Pause();
                }
                catch (Exception ex)
                {
                    materialFlatButton2.Text = "error";
                }
                
                
            }
            // enable video buttons
            //ControlLogic();
        }
        
        //overlay test
        private async void overBtn_Click(object sender, EventArgs e)
        {
            //CMD
            var test = new System.Diagnostics.Process()
            {
                EnableRaisingEvents = true
            };

            //test.StartInfo.FileName = @"C:\Users\SoMa\Desktop\RapidCheck\main\project\x64\Debug\RapidCheck.exe";
            test.StartInfo.FileName = @"C:\Users\trevor\Desktop\cpp.bat";
            test.StartInfo.RedirectStandardOutput = true;
            test.StartInfo.UseShellExecute = false;
            //test.StartInfo.WindowStyle  = ProcessWindowStyle.Hidden;\

            //test.OutputDataReceived += test_OutputDataReceived;

            test.Start();
            //test.BeginOutputReadLine();
            //test.WaitForExit();

            /*
            while (!test.HasExited)
            {
                await Task.Delay(500);

                test.Refresh();
            }
            */
           
            //another pro.

            System.Diagnostics.Process test1 = new System.Diagnostics.Process();
            //test1.StartInfo.FileName = @"C:\Users\SoMa\Anaconda3\envs\venvJupyter\python.exe C:\Users\SoMa\myworkspace\darkflow\test.py";
            test1.StartInfo.FileName = @"C:\Users\trevor\Desktop\python.bat";
            //test1.StartInfo.WindowStyle  = ProcessWindowStyle.Hidden;
            test1.Start();

            /*
            try
            {
                overlay = new Bitmap(@"C:\Users\trevor\Desktop\Videos\overlay.png");
                pictureBox1.Cursor = Cursors.Cross;
                
                ShowCombinedImage();
            }
            catch(Exception ex)
            {
                MessageBox.Show("Error opening file.\n" + ex.Message,
                "Open Error", MessageBoxButtons.OK,
                MessageBoxIcon.Error);
            }
             */






            //overlay test
            OverlayWindow overlay = new OverlayWindow(false);

            Stopwatch watch = Stopwatch.StartNew();

            int redBrush = overlay.Graphics.CreateBrush(0x7FFF0000);
            int redOpacityBrush = overlay.Graphics.CreateBrush(System.Drawing.Color.FromArgb(80, 255, 0, 0));
            int yellowBrush = overlay.Graphics.CreateBrush(0x7FFFFF00);

            int font = overlay.Graphics.CreateFont("Arial", 20);
            int hugeFont = overlay.Graphics.CreateFont("Arial", 50, true);

            float rotation = 0.0f;
            int fps = 0;
            int displayFPS = 0;

            while (true)
            {
                overlay.Graphics.BeginScene();
                overlay.Graphics.ClearScene();

                overlay.Graphics.DrawText("RotateSwastika", font, redBrush, 50, 450);
                overlay.Graphics.RotateSwastika(150, 600, 50, 2, rotation, redBrush);

                Console.WriteLine(overlay.ParentWindow);

                rotation += 0.03f;//related to speed
                if (rotation > 50.0f)//size of the swastika
                    rotation = -50.0f;

                if (watch.ElapsedMilliseconds > 1000)
                {
                    displayFPS = fps;
                    fps = 0;
                    watch.Restart();
                }
                else
                {
                    fps++;
                }

                overlay.Graphics.DrawText("FPS: " + displayFPS.ToString(), hugeFont, redBrush, 400, 600, false);
                overlay.Graphics.EndScene();
            }
        }

        void test_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            Console.WriteLine(e.Data);
        }

        void test_Exited(object sender, EventArgs e)
        {
            MessageBox.Show("test_Exited");
        }

        private Bitmap CombinedBitmap = null;
        //ShowCombinedImage
        private void ShowCombinedImage()
        {
            // If there's no background image, do nothing
            if (pictureBox1.Image == null)
            {
                MessageBox.Show("pictureBox1 image is nulln");
                return;
            }
            CombinedBitmap = new Bitmap(pictureBox1.Image);

            Point overlayLocation = new Point(10,10);

            //Add the overlay
            if(overlay != null)
            {
                using(Graphics gr = Graphics.FromImage(CombinedBitmap))
                {
                    gr.DrawImage(overlay, overlayLocation);
                }
            }
            pictureBox1.Image = CombinedBitmap;
        }
    }
}
