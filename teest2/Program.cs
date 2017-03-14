using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using OpenCvSharp;

namespace teest2
{
    class Program
    {
        [DllImport("test1-1.dll")]
        private static extern int ADD(int a, int b);
        [DllImport("test1-1.dll")]
        private static extern void asd();

        static void Main(string[] args)
        {
            int result = ADD(3 , 2);
            Console.Write(result);
            asd();
            //Cv2.ImShow("test", test);
        }
    }
}
