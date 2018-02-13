using System;
using System.IO;

namespace library
{
    public sealed class ErrorLoger
    {
        public static void LogErrors(Exception ex)
        {
            StreamWriter sw = new StreamWriter(Directory.GetCurrentDirectory() +  ex.Source+".txt", true);
            sw.WriteLine("Error Time Frame: {0}", DateTime.Now);
            if (ex.InnerException != null)
            {
                sw.Write("#Region Inner Exception: ");
                sw.Write("Exception Type: ");
                sw.WriteLine(ex.InnerException.GetType().ToString());
                sw.Write("Exception: ");
                sw.WriteLine(ex.InnerException.Message);
                sw.Write("Source: ");
                sw.WriteLine(ex.InnerException.Source);
                if (ex.InnerException.StackTrace != null)
                {
                    sw.WriteLine("Stack Trace: ");
                    sw.WriteLine(ex.InnerException.StackTrace);
                }
                sw.Write("#EndRegion Inner Exception: ");
            }
            sw.Write("Exception Type: ");
            sw.WriteLine(ex.GetType().ToString());
            sw.WriteLine("Exception: " + ex.Message);
            sw.WriteLine("Source: " + ex.Source);
            sw.WriteLine("Stack Trace: ");
            if (ex.StackTrace != null)
            {
                sw.WriteLine(ex.StackTrace);
                sw.WriteLine();
            }
            sw.Close();
        }
    }
}
