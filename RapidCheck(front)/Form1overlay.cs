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
using System.Drawing.Imaging;
namespace RapidCheck
{
    public partial class Form1
    {
        //overlay test
        private async void overBtn_Click(object sender, EventArgs e)
        {
            int[] oId = {67,53,5,52,102,72,103,36,89, 1};
            int[] fra = {168,114,102,102,84,72,72,66,66,66};
            //over(@"C:\videos\Background\0.bmp",@"C:\videos\Background\0.bmp",@"C:\videos\test.bmp",30,30);
            for (int i = 6; i < 10; i++)
            {
                int frame = 0;
                string[] files;
                files = System.IO.Directory.GetFiles(@"C:\videos\obj\" + oId[i] + @"\", "*.bmp", System.IO.SearchOption.TopDirectoryOnly);
                int rowNum;
                for (int j = 0; j < 66; j++) //img파일 접근
                {
                    makeFolder(@"C:\videos\Background" + (i+1));
                    string background = @"C:\videos\Background"+ i + @"\" + frame + ".bmp";
                    string outputpath = @"C:\videos\Background"+ (i+1) + @"\" + frame + ".bmp";
                    string crop = files[j];
                    rowNum = Convert.ToInt32(files[j].Replace(@"C:\videos\obj\" + oId[i] + @"\", "").Replace(".bmp", ""));
                    Rectangle temp = new Rectangle(myData[rowNum].x, myData[rowNum].y, myData[rowNum].w, myData[rowNum].h);
                    overlay1(background, crop, outputpath, temp);
                    frame += 1;
                }
            }
            //overlay1();
            //overlay2();
        }

        private Bitmap overlay = null;
        private void overlay1(string baseImgPath, string cropImgPath,string outputImgPath, Rectangle position)
        {
            try
            {
                Bitmap CombinedBitmap = new Bitmap(baseImgPath);
                //Point overlayLocation = new Point(x, y);
                overlay = new Bitmap(cropImgPath);
                //pictureBox1.Cursor = Cursors.Cross;
                //Add the overlay
                if (overlay != null)
                {
                    using (Graphics gr = Graphics.FromImage(CombinedBitmap))
                    {
                        //create a color matrix object  
                        ColorMatrix matrix = new ColorMatrix();

                        //set the opacity  
                        matrix.Matrix33 = 0.6f;

                        //create image attributes  
                        ImageAttributes attributes = new ImageAttributes();

                        //set the color(opacity) of the image  
                        attributes.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);

                        gr.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                        gr.DrawImage(overlay, position, 0, 0, overlay.Width, overlay.Height, GraphicsUnit.Pixel, attributes);
                    }
                }
                //if (System.IO.File.Exists(outputImgPath))
                //{
                //    System.IO.File.Delete(outputImgPath);
                //}

                CombinedBitmap.Save(outputImgPath);                
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error opening file.\n" + ex.Message,
                "Open Error", MessageBoxButtons.OK,
                MessageBoxIcon.Error);
            }
        }
        //private void ShowCombinedImage()
        //{
        //    CombinedBitmap = new Bitmap(@"C:\videos\Background\0.bmp");

        //    Point overlayLocation = new Point(30, 30);

        //    //Add the overlay
        //    if (overlay != null)
        //    {
        //        using (Graphics gr = Graphics.FromImage(CombinedBitmap))
        //        {
        //            gr.DrawImage(overlay, overlayLocation);
        //        }
        //    }
        //    CombinedBitmap.Save(@"C:\videos\temp.bmp");
        //}
    }
}





