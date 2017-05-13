using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//Direct 2d
using SharpDX;
using SharpDX.IO;
using d2 = SharpDX.Direct2D1;
using d3d = SharpDX.Direct3D11;
using dxgi = SharpDX.DXGI;
using wic = SharpDX.WIC;
using dw = SharpDX.DirectWrite;

namespace RapidCheck
{
    public partial class Form1
    {
        static void over(string inputPath, string cropImgPath, string outputPath, int x, int y)
        {
            
            // 그래픽을 랜더링할 장비를 추가 - 3D or 2D
            var defaultDevice = new SharpDX.Direct3D11.Device(SharpDX.Direct3D.DriverType.Hardware,
                d3d.DeviceCreationFlags.VideoSupport
                | d3d.DeviceCreationFlags.BgraSupport
                | d3d.DeviceCreationFlags.None);
            var d3dDevice = defaultDevice.QueryInterface<d3d.Device1>(); //get a refer to the D3D 11.1 device
            var dxgiDevice = d3dDevice.QueryInterface<dxgi.Device1>(); //get a refer to the DXGI device
            var d2dDevice = new d2.Device(dxgiDevice);

            // DeviceContext를 초기화. D2D 렌더링 타겟이 될 것이고 모든 렌더링 작업을 허용
            var d2dContext = new d2.DeviceContext(d2dDevice, d2.DeviceContextOptions.None);
            var dwFactory = new dw.Factory();
            
            
            //D2D, WIC 둘 다 지원되는 픽셀 형식 지정
            var d2PixelFormat = new d2.PixelFormat(dxgi.Format.R8G8B8A8_UNorm, d2.AlphaMode.Premultiplied);
            //RGBA형식 사용
            var wicPixelFormat = wic.PixelFormat.Format32bppPRGBA;


            //이미지 로딩
            var imagingFactory = new wic.ImagingFactory2();
            var decoder = new wic.BmpBitmapDecoder(imagingFactory);
            var inputStream = new wic.WICStream(imagingFactory, inputPath, NativeFileAccess.Read);
            decoder.Initialize(inputStream, wic.DecodeOptions.CacheOnLoad);            

            //다이렉트2D가 사용할수 있도록 디코딩
            var formatConverter = new wic.FormatConverter(imagingFactory);
            formatConverter.Initialize(decoder.GetFrame(0), wicPixelFormat);

            //기본 이미지를 D2D이미지로 로드
            //var inputBitmap = d2.Bitmap1.FromWicBitmap(d2dContext, formatConverter, new d2.BitmapProperties1(d2PixelFormat));

            //이미지 크기 저장
            var inputImageSize = formatConverter.Size;
            var pixelWidth = inputImageSize.Width;
            var pixelHeight = inputImageSize.Height;


            //Effect1 : BitpmapSource - 디코딩된 이미지 데이터를 가져오고 BitmapSource 가져오기
            var bitmapSourceEffect = new d2.Effects.BitmapSource(d2dContext);
            bitmapSourceEffect.WicBitmapSource = formatConverter;


            
            //IntPtr crop = new System.Drawing.Bitmap(cropImg).GetHbitmap();
            //var temp = new d2.DeviceContext(crop);
            //var blend = new d2.Effects.Blend(temp);
            //var cropCrop = new d2.Effects.BitmapSource(temp);
            //cropCrop.WicBitmapSource = formatConverter;


            // Effect 2 : GaussianBlur - bitmapsource에 가우시안블러 효과 적용
            //var gaussianBlurEffect = new d2.Effects.GaussianBlur(d2dContext);
            //gaussianBlurEffect.SetInput(0, bitmapSourceEffect.Output, true);
            //gaussianBlurEffect.StandardDeviation = 5f;







            ////overlay text setup
            //var textFormat = new dw.TextFormat(dwFactory, "Arial", 15f);

            ////draw a long text to show the automatic line wrapping
            //var textToDraw = "sime ling text..." + "text" + "dddd";

            ////create the text layout - this imroves the drawing performance for static text
            //// as the glyph positions are precalculated
            ////윤곽선 글꼴 데이터에서 글자 하나의 모양에 대한 기본 단위를 글리프(glyph)라고 한다
            //var textLayout = new dw.TextLayout(dwFactory, textToDraw, textFormat, 300f, 1000f);

            //SharpDX.Mathematics.Interop.RawColor4 color = new SharpDX.Mathematics.Interop.RawColor4(255, 255, 255, 1);
            //var textBrush = new d2.SolidColorBrush(d2dContext, color);

            


            //여기서부터 다시

            //render target setup

            //create the d2d bitmap description using default flags and 96 DPI
            var d2dBitmapProps = new d2.BitmapProperties1(d2PixelFormat, 96, 96, d2.BitmapOptions.Target | d2.BitmapOptions.CannotDraw);

            //the render target
            var d2dRenderTarget = new d2.Bitmap1(d2dContext, new Size2(pixelWidth, pixelHeight), d2dBitmapProps);
            d2dContext.Target = d2dRenderTarget; //associate bitmap with the d2d context

            System.Drawing.Bitmap temp = new System.Drawing.Bitmap(cropImgPath);
            IntPtr temp2 = temp.GetHbitmap();
            var obj = new SharpDX.Direct2D1.Bitmap(temp2);

            Vector2 center = new Vector2(temp.Size.Width / 2, temp.Size.Height / 2);
            //d2dContext.Transform = Matrix.Transformation2D(center, 0f, new Vector2(pixelWidth / temp.Size.Width, pixelHeight / temp.Size.Height), center, 0f, new Vector2(x - center.X, y - center.Y));
            var lin = SharpDX.Direct2D1.BitmapInterpolationMode.NearestNeighbor;
            d2dContext.DrawBitmap(obj, 1f, lin);
            //d2dContext.Transform = Matrix.Transformation2D(Vector2.Zero, 0f, new Vector2(1f, 1f), Vector2.Zero, 0f, Vector2.Zero);
            
            
            

            //Drawing        
            //slow preparations - fast drawing
            d2dContext.BeginDraw();
            d2dContext.DrawImage(bitmapSourceEffect, new SharpDX.Mathematics.Interop.RawVector2(0f, 0f));            
            //d2dContext.DrawImage(temp, new SharpDX.Mathematics.Interop.RawVector2((float)x, (float)y));


            //test
            SharpDX.Direct2D1.Bitmap1 newBitmap;
            // Neccessary for creating WIC objects.
            NativeFileStream fileStream = new NativeFileStream(cropImgPath, NativeFileMode.Open, NativeFileAccess.Read);
            // Used to read the image source file.
            wic.BitmapDecoder bitmapDecoder = new wic.BitmapDecoder(imagingFactory, fileStream, wic.DecodeOptions.CacheOnDemand);
            // Get the first frame of the image.
            wic.BitmapFrameDecode frame = bitmapDecoder.GetFrame(0);
            // Convert it to a compatible pixel format.
            wic.FormatConverter converter = new wic.FormatConverter(imagingFactory);
            converter.Initialize(frame, SharpDX.WIC.PixelFormat.Format32bppPRGBA);
            // Create the new Bitmap1 directly from the FormatConverter.
            newBitmap = SharpDX.Direct2D1.Bitmap1.FromWicBitmap(d2dContext, converter);
            
            ///test
            d2dContext.DrawBitmap(newBitmap, 1.0f, SharpDX.Direct2D1.BitmapInterpolationMode.Linear);

            
            //d2dContext.DrawBitmap(new d2.Bitmap(cropImg), rect,0.5f, new SharpDX.Direct2D1.BitmapInterpolationMode());
            //d2dContext.DrawTextLayout(new SharpDX.Mathematics.Interop.RawVector2(50f, 50f), textLayout, textBrush);
            d2dContext.EndDraw();

            //Image save

            //delete the output file if it already exists
            if (System.IO.File.Exists(outputPath)) System.IO.File.Delete(outputPath);

            //use the appropiate overload to write either to tream or to a file
            var stream = new wic.WICStream(imagingFactory, outputPath, NativeFileAccess.Write);

            //select the image encoding format HERE
            var encoder = new wic.PngBitmapEncoder(imagingFactory);
            encoder.Initialize(stream);

            var bitmapFrameEncode = new wic.BitmapFrameEncode(encoder);
            bitmapFrameEncode.Initialize();
            bitmapFrameEncode.SetSize(pixelWidth, pixelHeight);
            bitmapFrameEncode.SetPixelFormat(ref wicPixelFormat);

            //this is the trick to write d2d1 bitmap to WIC
            var imageEncoder = new wic.ImageEncoder(imagingFactory, d2dDevice);
            imageEncoder.WriteFrame(d2dRenderTarget, bitmapFrameEncode, new wic.ImageParameters(d2PixelFormat, 96, 96, 0, 0, pixelWidth, pixelHeight));

            bitmapFrameEncode.Commit();
            encoder.Commit();

            //cleanup

            //dispose everything and free used resources
            bitmapFrameEncode.Dispose();
            encoder.Dispose();
            stream.Dispose();
            //textBrush.Dispose();
            //textLayout.Dispose();
            //textFormat.Dispose();
            formatConverter.Dispose();
            //gaussianBlurEffect.Dispose();
            bitmapSourceEffect.Dispose();
            d2dRenderTarget.Dispose();
            inputStream.Dispose();
            decoder.Dispose();
            d2dContext.Dispose();
            dwFactory.Dispose();
            imagingFactory.Dispose();
            d2dDevice.Dispose();
            dxgiDevice.Dispose();
            d3dDevice.Dispose();
            defaultDevice.Dispose();

            //save
            //System.Diagnostics.Process.Start(outputPath);
        }
    }
}
