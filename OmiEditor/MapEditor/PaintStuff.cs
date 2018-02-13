using System;
using System.Drawing;
using System.IO;
using Library;
using System.Linq;
using System.Windows.Forms;
using System.Collections.Generic;

namespace MapEditor
{
    class PaintStuff
    {
        private static Bitmap MapTexture = new Bitmap(512, 512);

        /// <summary>
        /// //it creates image aka texture from .map file original size that return is 2048x2048
        /// if(temp.Length 16).. or 17 because city towns have one additional file packed in !
        /// </summary>
        /// <param name="path">to the xxxx.map file location </param>
        /// <returns></returns>
        public static Bitmap GetMapImage(string path)
        {
            Bitmap FinalImage = new Bitmap(2048, 2048);
            Bitmap[] slices = new Bitmap[16];
            try
            {
                var temp = AtumZip.ZippedFile.UnzipItems(path);

                if (temp.Length == 17)
                {
                    for (int i = 1; i < 17; i++) // town map has one additional file at temp[0] so i am skipping it 
                    {
                        //so that we start at index 0 to save later when putting together image some headache
                        slices[i - 1] = new Bitmap(new MemoryStream(temp[i].Data));
                    }
                }
                if (temp.Length == 16)
                {
                    for (int i = 0; i < 16; i++)
                    {
                        slices[i] = new Bitmap(new MemoryStream(temp[i].Data));
                    }
                }

                Graphics g = Graphics.FromImage(FinalImage);
                int x_pos = 0, y_pos = 0;

                for (int i = 0; i < slices.Length; i++)
                {
                    g.DrawImage(slices[i], x_pos, y_pos, 512, 512);
                    x_pos += 512;
                    if (x_pos == 2048)
                    {
                        x_pos = 0;
                        y_pos += 512;
                    }
                }
                //rotate image so that is shown corecly based to other stuff like drawing water ... 
                FinalImage.RotateFlip(RotateFlipType.Rotate180FlipNone);

            }catch (Exception ex) { ErrorLogger.LogException(ex); }
            MapTexture = new Bitmap(FinalImage, new Size(512,512)); // temporary solution!
            return FinalImage;
        }

        /// <summary>
        /// GetWaterImage(object[] data)
        /// </summary>
        /// <param name="data"> it draws water based on data from tileinfo from xxxx.dat file</param>
        /// <returns></returns>          
        public static Bitmap GetWaterImage(object[] tileinfodata)
        {
            Bitmap tempimg = new Bitmap(512, 512);
         
            Brush aBrush = Brushes.Black;
            Graphics g = Graphics.FromImage(tempimg);       
            // image = new Bitmap(512, 512);
            var idx = Formatter.Dat.MapFormatter.GetIndex("tileinfo", "EventType");
            var temp = (tileinfodata[2] as object[]).Select(x => (byte)(x as object[])[idx]).ToArray();
            for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
                    for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)
                            if(temp[x + 256 * y] == 1)
                            {
                                g.FillRectangle(aBrush, 2 * (255 - y) + i, 2 * x + j, 1,1);
                            }
                              
                             //tempimg.SetPixel(, ,  == 0 ? Color.Transparent : Color.Black);
      
            //todo implent drawing crap
            return tempimg;
        }

        //not complete jet !
        public static Bitmap GetPadsImage(object[] tileinfodata)
        {
            return GetPadsImage(null);
        }

        /// <summary>
        /// Tuple<string,int, Rectangle, int> //region name, monsternum, rectangle(x,y,width,height),spawn count!
        /// </summary>

        private static IEnumerable<Tuple<string, int, Rectangle, int>> MonsterData = null;



        public static IEnumerable<Tuple<string, int, Rectangle, int>>PrepareDataForDrawing(DataGridView dgv)
        {
          
            
                for (int i = 0; i < dgv.Rows.Count-1; i++)
                {
                    yield return new Tuple<string, int, Rectangle, int>(
                        //region name
                        dgv.Rows[i].Cells[0].Value.ToString(),
                        //monsternum                                                  
                        int.Parse(dgv.Rows[i].Cells[1].Value.ToString()),
                        //new Rectangle for drawing later on                                               
                        new Rectangle(           
                            int.Parse(dgv.Rows[i].Cells[2].Value.ToString()) * 2, 
                            int.Parse(dgv.Rows[i].Cells[3].Value.ToString()) * 2,
                            int.Parse(dgv.Rows[i].Cells[4].Value.ToString()) * 2 - int.Parse(dgv.Rows[i].Cells[2].Value.ToString()) * 2,
                            int.Parse(dgv.Rows[i].Cells[5].Value.ToString()) * 2 - int.Parse(dgv.Rows[i].Cells[3].Value.ToString()) * 2),
                        //max monster count                                                              
                        int.Parse(dgv.Rows[i].Cells[6].Value.ToString()));
                }
       
        }


     



        public static Bitmap DrawMonsterRectangles(DataGridView dgv)
        {
            MonsterData = null;
            MonsterData = PrepareDataForDrawing(dgv);
            Bitmap tempimg = new Bitmap(512, 512);
            tempimg = MapTexture;

            Graphics g = Graphics.FromImage(tempimg);
      

            foreach (var data in MonsterData)
            {   
                g.FillRectangle(new SolidBrush(Color.GreenYellow), data.Item3);
                g.DrawString(data.Item1, new Font(FontFamily.GenericSansSerif, 10, FontStyle.Regular), new SolidBrush(Color.DarkBlue), new Point(data.Item3.X, data.Item3.Y));     
            }
            //  box.Image = tempimg;
            return tempimg; 
        }



        









    }
}

