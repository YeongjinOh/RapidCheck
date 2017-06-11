using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms; //MessageBox

//overlay
//using DirectXOverlayWindow;
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
            //overlay1();
            //overlay2();
        }

        //private void overlay1()
        //{
        //    try
        //    {
        //        overlay = new Bitmap(@"C:\Users\trevor\Desktop\Videos\overlay.png");
        //        pictureBox1.Cursor = Cursors.Cross;

        //        ShowCombinedImage();
        //    }
        //    catch (Exception ex)
        //    {
        //        MessageBox.Show("Error opening file.\n" + ex.Message,
        //        "Open Error", MessageBoxButtons.OK,
        //        MessageBoxIcon.Error);
        //    }
        //}
        //}
        //private Bitmap CombinedBitmap = null;
        ////ShowCombinedImage
        //private void ShowCombinedImage()
        //{
        //    // If there's no background image, do nothing
        //    if (pictureBox1.Image == null)
        //    {
        //        MessageBox.Show("pictureBox1 image is nulln");
        //        return;
        //    }
        //    CombinedBitmap = new Bitmap(pictureBox1.Image);

        //    Point overlayLocation = new Point(10, 10);

        //    //Add the overlay
        //    if (overlay != null)
        //    {
        //        using (Graphics gr = Graphics.FromImage(CombinedBitmap))
        //        {
        //            gr.DrawImage(overlay, overlayLocation);
        //        }
        //    }
        //    pictureBox1.Image = CombinedBitmap;
        //}
    }
}
