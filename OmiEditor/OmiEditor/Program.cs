using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace AresTool
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MainForm());
            }
            catch (Exception ex)
            {
                ReportException(ex);
            }
        }

        public static void ReportException(Exception ex)
        {
            var s = new List<string>();
            while (ex != null)
            {
                s.Add("Exception Info:");
                s.Add("Message:");
                s.Add(ex.Message);
                s.Add("Source:");
                s.Add(ex.Source);
                s.Add("Stack Trace:");
                s.Add(ex.StackTrace);
                s.Add("");
                ex = ex.InnerException;
            }
            File.WriteAllLines("error.txt", s.ToArray());
        }
    }
}
