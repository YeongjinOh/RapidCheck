using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RapidCheck
{
    using SharpDX;
    using SharpDX.IO;

    using d2 = SharpDX.Direct2D1;
    using d3d = SharpDX.Direct3D11;
    using dxgi = SharpDX.DXGI;
    using wic = SharpDX.WIC;
    using dw = SharpDX.DirectWrite;

    class D2DOverlay
    {
        static void over()
        {
            var inputPath = "Input.png";
            var outputPath = "output.png";

            //initialization

            // initialize the D3D device which will allow to render to image any graphics - 3D or 2D
            var defaultDevice = new SharpDX.Direct3D11.Device(SharpDX.Direct3D.DriverType.Hardware,
                d3d.DeviceCreationFlags.VideoSupport
                | d3d.DeviceCreationFlags.BgraSupport
                | d3d.DeviceCreationFlags.None);
            var d3dDevice = defaultDevice.QueryInterface<d3d.Device1>(); //get a refer to the D3D 11.1 device
            var dxgiDevice = d3dDevice.QueryInterface<dxgi.Device1>(); //get a refer to the DXGI device

            var d2dDevice = new d2.Device(dxgiDevice);

            var imagingFactory = new wic.ImagingFactory2();

            // initialize the DeviceContext - it will be the D2D render target and will allow all rendering operations
            var d2dContext = new d2.DeviceContext(d2dDevice, d2.DeviceContextOptions.None);

            var dwFactory = new dw.Factory();

            //specify a pixel format that is supported both D2D and WIC
            var d2PixelFormat = new d2.PixelFormat(dxgi.Format.R8G8B8A8_UNorm, d2.AlphaMode.Premultiplied);
            //if in D2D was specified an RGBA format - use the same for wic
            var wicPixelFormat = wic.PixelFormat.Format32bppPRGBA;

            //Image Loading
            var decoder = new wic.PngBitmapDecoder(imagingFactory);
            var inputStream = new wic.WICStream(imagingFactory, inputPath, NativeFileAccess.Read);
            decoder.Initialize(inputStream, wic.DecodeOptions.CacheOnLoad);

            //decode the loaded image to a format that can be consumed by D2D
            var formatConverter = new wic.FormatConverter(imagingFactory);
            formatConverter.Initialize(decoder.GetFrame(0), wicPixelFormat);

            //load the base image into a D2D Bitmap
            //var inputBitmap = d2.Bitmap1.FromWicBitmap(d2dContext, formatConverter, new d2.BitmapProperties1(d2PixelFormat));

            //store the image size - output will be of the same size
            var inputImageSize = formatConverter.Size;
            var pixelWidth = inputImageSize.Width;
            var pixelHeight = inputImageSize.Height;

            //EFFECT SETUP

            //Effect1 : BitpmapSource - take decoded image data and get a BitmapSource from it
            var bitmapSourceEffect = new d2.Effects.BitmapSource(d2dContext);
            bitmapSourceEffect.WicBitmapSource = formatConverter;

            // Effect 2 : GaussianBlur - give the bitmapsource a gaussian blurred effect
            //var gaussianBlurEffect = new d2.Effects.GaussianBlur(d2dContext);
            //gaussianBlurEffect.SetInput(0, bitmapSourceEffect.Output, true);
            //gaussianBlurEffect.StandardDeviation = 5f;

            //overlay text setup
            var textFormat = new dw.TextFormat(dwFactory, "Arial", 15f); //create the text format of specified font configuration

            //draw a long text to show the automatic line wrapping
            var textToDraw = "sime ling text..." + "text" + "dddd";

            //create the text layout - this imroves the drawing performance for static text
            // as the glyph positions are precalculated
            var textLayout = new dw.TextLayout(dwFactory, textToDraw, textFormat, 300f, 1000f);

            SharpDX.Mathematics.Interop.RawColor4 color = new SharpDX.Mathematics.Interop.RawColor4(255, 255, 255, 1);
            var textBrush = new d2.SolidColorBrush(d2dContext, color);

            //render target setup

            //create the d2d bitmap description using default flags and 96 DPI
            var d2dBitmapProps = new d2.BitmapProperties1(d2PixelFormat, 96, 96, d2.BitmapOptions.Target | d2.BitmapOptions.CannotDraw);

            //the render target
            var d2dRenderTarget = new d2.Bitmap1(d2dContext, new Size2(pixelWidth, pixelHeight), d2dBitmapProps);
            d2dContext.Target = d2dRenderTarget; //associate bitmap with the d2d context

            //Drawing

            //slow preparations - fast drawing
            d2dContext.BeginDraw();
            //d2dContext.DrawImage()
            d2dContext.DrawTextLayout(new SharpDX.Mathematics.Interop.RawVector2(5f,5f), textLayout, textBrush);
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
            textBrush.Dispose();
            textLayout.Dispose();
            textFormat.Dispose();
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
            System.Diagnostics.Process.Start(outputPath);
        }
    }
}
