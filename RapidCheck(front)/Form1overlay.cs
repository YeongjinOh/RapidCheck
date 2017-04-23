using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms; //MessageBox

//overlay
using DirectXOverlayWindow;
using System.Diagnostics; //stopwatch
using System.Drawing; //Bitmap, Point, Graphics

namespace RapidCheck
{
    public partial class Form1
    {
        private Bitmap overlay = null;

        //overlay test
        private async void overBtn_Click(object sender, EventArgs e)
        {
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

            Point overlayLocation = new Point(10, 10);

            //Add the overlay
            if (overlay != null)
            {
                using (Graphics gr = Graphics.FromImage(CombinedBitmap))
                {
                    gr.DrawImage(overlay, overlayLocation);
                }
            }
            pictureBox1.Image = CombinedBitmap;
        }
    }
}
