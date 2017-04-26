using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//CMD command
using System.Diagnostics;

namespace RapidCheck
{
    public partial class Form1
    {

        private void CMDBtn_Click(object sender, EventArgs e)
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
        }
        void test_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            Console.WriteLine(e.Data);
        }

    }
}
