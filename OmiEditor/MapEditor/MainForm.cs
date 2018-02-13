using System;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

using Library;
using Formatter.Sma;
using System.IO;
using Formatter.Dat;
using System.Collections.Generic;
using Formatter.Omd;
using System.ComponentModel;
using System.Reflection;

namespace MapEditor
{
    public partial class MainForm : Form
    {
   
       
        //WATER_TYPE_NOTING				= 0,
        //WATER_TYPE_SERVER_AND_CLIENT	= 1,
        //WATER_TYPE_ONLY_SERVER	    = 2,
        //WATER_TYPE_ONLY_CLIENT	    = 3

        /// <summary>
        /// TODO LIST FOR ME ZEYDEN :D 
        /// Add support for 128x128 maps curently bitmaps can be saved only for 256x256!
        /// Add saving for monsterdata and the draw data of locations !
        /// Add better funkcionality for Map aka Dat files (treeview control)
        /// Add AtumUnziper to unpack map files (load .dat files and extract data like bitmap or other stuff like object location etc..) 
        /// Add AtumZip for saving changes direcly to dat files  
        /// better error checking aka fields inputs ...
        /// 
        /// </summary>

        object[] _SmaData = null; // from sma file aka xxxx.sma
        object[] _DatData = null; // from map file aka xxxx.dat aka extracted one atm whithout extension!

        Bitmap image = new Bitmap(512, 512);

        public MainForm()
        {
            InitializeComponent();
            //dataGridView1.DoubleBuffered(true);  its hiden atm so no need to have it double buffered 
            dataGridView2.DoubleBuffered(true);
            panel1.DoubleBuffered(true);
            dataGridView3.DoubleBuffered(true);
            LoadingData.GetListSma(treeView1);
            LoadingData.GetListDat(treeView2);
        
            toolStripLabel1.Text = "Sma Count: " + treeView1.GetNodeCount(false).ToString();
            DataGridViewLBL.Text = "";
            BitmapLBL.Text = "";
            tabControl1.SelectedTab = tabPage1;

           




        }

        #region SMA_DAT

        private enum BitmapType
        {
            SMA = 0,
            DAT = 1,
        };

        private void makebitmap(PictureBox box, BitmapType btype)
        {
            try
            {
                image = new Bitmap(512, 512);
                if (btype == BitmapType.SMA)
                {
                    var idx = SmaFormatter.GetIndex("tileinfo", "EventType");

                    var temp = (_SmaData[1] as object[]).Select(x => (byte)(x as object[])[idx]).ToArray();
                    for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
                            for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)
                                    image.SetPixel(2 * (255 - y) + i, 2 * x + j, temp[x + 256 * y] == 0 ? Color.LightBlue : Color.Black);
                    box.Image = image;

                }

                if (btype == BitmapType.DAT)
                {
                    var idx = MapFormatter.GetIndex("tileinfo", "EventType");
                    var temp = (_DatData[2] as object[]).Select(x => (byte)(x as object[])[idx]).ToArray();
                    for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
                            for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)
                                    image.SetPixel(2 * (255 - y) + i, 2 * x + j, temp[x + 256 * y] == 0 ? Color.LightBlue : Color.Black);
                    box.Image = image;
                }

            }catch(Exception ex) { ErrorLogger.LogException(ex); }
        }

     
    
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            var node = e.Node;
            if (node == null || !(node.Tag is Tuple<string, int, int>)) return;
            var info = node.Tag as Tuple<string, int, int>;

            toolStripLabel2.Text = "Location: " + info.Item1 + " Map: " + info.Item2 + " Size: " + info.Item3;
            if(info.Item2 != -1)//just little handling because if u click [+]xxxx it will want to open file named -1 
            {
                var source = File.OpenRead(LoadingData.directory + info.Item2 + ".sma");
                _SmaData = SmaFormatter.Read(source, (uint)info.Item3);
            }
           
            switch (info.Item1)
            {
               
                case "header":


                    var tempdt = SmaFormatter.GetFormatter(0).GetSchema();
                    tempdt.Rows.Add(_SmaData[0] as object[]);
                    dataGridView2.DataSource = tempdt;

                    //it crashes the program atm!
                    // dataGridView2.DataSource = SmaFormatter.GetFormatter(0).GetSchema(_SmaData[0] as object[][]);
                    break;

                case "tileinfo":

                    dataGridView2.DataSource = SmaFormatter.GetFormatter(1).GetSchema(_SmaData[1] as object[][]);         
                    makebitmap(pictureBox1,BitmapType.SMA);

                    break;
                case "monsterinfo":

                    dataGridView2.DataSource = SmaFormatter.GetFormatter(2).GetSchema(_SmaData[2] as object[][]);
                    var temp = SmaFormatter.GetFormatter(2).GetSchema(_SmaData[2] as object[][]);

                    Draw.MonsterData = null;
                 
                    
                    panel1.Controls.Clear();
                     

                    Draw.MonsterData = Draw.PrepareMonsterData(_SmaData[2] as object[][]);
                    panel1.BackgroundImage = new Bitmap(PaintStuff.GetMapImage(Directory.GetCurrentDirectory() + @"..\..\..\map\" + info.Item2 + ".map"), new Size(512, 512));
                    foreach (var spawn in Draw.MonsterData)
                    {
                        //spawn.Item3.BackColor = Color.Red;
                        ToolTip tt = new ToolTip();
                        tt.SetToolTip(spawn.Item3,"Region Name: "+ spawn.Item1+"\n MonsterNum: " + spawn.Item2 + "\n MaxCount: " + spawn.Item4);

                        panel1.Controls.Add(spawn.Item3);
                        
                    }


                 //   image = new Bitmap(PaintStuff.GetMapImage(@"C:\Ares_Source\BuildResult\Client\Release\Res-Map\" + info.Item2+".map"), new Size(512, 512));
                 //   pictureBox1.Image = image;

                    //if (dataGridView2.SelectedRows.Count == 1)
                    //{
                    //  //  Bitmap test = new Bitmap(512, 512);
                    //    var selectedrow = dataGridView2.SelectedRows[0];
                    //    int sx = GetSize(int.Parse(selectedrow.Cells[2].Value.ToString()));
                    //    int sy = GetSize(int.Parse(selectedrow.Cells[3].Value.ToString()));
                    //    int ex = GetSize(int.Parse(selectedrow.Cells[4].Value.ToString()));
                    //    int ey = GetSize(int.Parse(selectedrow.Cells[5].Value.ToString()));



                    //    Graphics g = Graphics.FromImage(image);

                    //    g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

                    //    //g.DrawRectangle(new Pen(Color.Red), sx,sy,ex-sx,ey-sy);   i think rectangle its abit off so its disabled atm !         
                    //    g.DrawString("S", new Font(FontFamily.GenericSansSerif, 18, FontStyle.Regular), new SolidBrush(Color.Green), sx - 9, sy - 9);
                    //    g.DrawString("E", new Font(FontFamily.GenericSansSerif, 18, FontStyle.Regular), new SolidBrush(Color.Red), sx - 9, ey - 9);
                    //    g.Dispose();

                    //    pictureBox1.Refresh();
                    //}


                        break;
            } 
        }
        
      

      

        private void treeView2_dat_AfterSelect(object sender, TreeViewEventArgs e)
        {
            var node = e.Node;
            if (node == null || !(node.Tag is Tuple<string, string, int>)) return;
            var info = node.Tag as Tuple<string, string, int>;

            var temp = AtumZip.ZippedFile.UnzipItems(LoadingData.directory + info.Item1);
            using (var tempstream = new MemoryStream(temp[0].Data))
                _DatData = MapFormatter.Read(tempstream);


            if(info.Item2 == "base")
            {             
                string temp5 = info.Item1;
                temp5 = temp5.Remove(temp5.Length - 4, 4);
                pictureBox2.Image =  new Bitmap(PaintStuff.GetMapImage(LoadingData.directory + temp5 + ".map"),new Size(512,512));
                PictureBox cc = new PictureBox();
                cc.Width = pictureBox2.Width;
                cc.Height = pictureBox2.Height;
                Controls.Add(cc);
                Bitmap crap = (Bitmap)pictureBox2.Image;
                Graphics g = Graphics.FromImage(crap);
                Bitmap ttt = PaintStuff.GetWaterImage(_DatData);
               // g.DrawImage(ttt, 0, 0, 512, 512);
                SolidBrush sb = new SolidBrush(Color.Red);
                g.FillEllipse(sb, 20, 20, 20, 20);
                g.DrawEllipse(new Pen(Color.Red), 20,20,20,20);
                pictureBox2.Refresh();

            }
          
            switch (info.Item2)
            {
                
                case "mapdata":
                
                    var tempdt = MapFormatter.GetFormatter(0).GetSchema();
                    tempdt.Rows.Add(_DatData[0] as object[]);
                    dataGridView3.DataSource = tempdt;
                   
                    // dataGridView3.DataSource = MapFormatter.GetFormatter(0).GetSchema(new object[] { (_DatData[0] as object[] });     

                    break;

                case "vertexinfo":
                    dataGridView3.DataSource = MapFormatter.GetFormatter(1).GetSchema(_DatData[1] as object[][]);
                    break;

                case "tileinfo":  
                    dataGridView3.DataSource = MapFormatter.GetFormatter(2).GetSchema(_DatData[2] as object[][]);               
                    break;

                case "objectinfo":
                    dataGridView3.DataSource = MapFormatter.GetFormatter(3).GetSchema(_DatData[3] as object[][]);
                    break;
            }
        }

        private void WaterMap_BTN_Click(object sender, EventArgs e)
        {
            image = new Bitmap(512, 512);

            var idx = MapFormatter.GetIndex("tileinfo", "WaterType");
            var temp = (_DatData[2] as object[]).Select(x => (uint)(x as object[])[idx]).ToArray();
            for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
                    for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)

                            switch (temp[x + 256 * y])
                            {
                                case 1:
                                    image.SetPixel(2 * (255 - y) + i, 2 * x + j, Color.LightBlue);
                                    break;

                                default:
                                    image.SetPixel(2 * (255 - y) + i, 2 * x + j, Color.Green);
                                    break;
                            }


         
            pictureBox2.Image = image;

        }

        private void PadMap_BTN_Click(object sender, EventArgs e)
        {
            image = new Bitmap(512, 512);
                var idx = MapFormatter.GetIndex("tileinfo", "EventType");
                var temp = (_DatData[2] as object[]).Select(x => (byte)(x as object[])[idx]).ToArray();
                for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
                        for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)
                                image.SetPixel(2 * (255 - y) + i, 2 * x + j, temp[x + 256 * y] == 0 ? Color.LightBlue : Color.Black);
                pictureBox2.Image = image;
            

        }


        #endregion

        private void dataGridView2_SelectionChanged(object sender, EventArgs e)
        {
            //MessageBox.Show( dataGridView2.SelectedCells.ToString());


            //if (e.RowIndex >= 0)
            //{
            //    DataGridViewRow row = dataGridView2.Rows[e.RowIndex];
            //    //int x = (int)row.Cells["Employee ID"].Value;
            //    //int y = (int)row.Cells["Employee ID"].Value;
            //    MessageBox.Show((int)row.Cells["Employee ID"].Value + "");
            //}

        }

        //private int GetSize(int crap)
        //{
        //    return (crap * 2);// / 100;
        //}

        private void dataGridView2_SelectionChanged_1(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode.ToString().Contains("monsterinfo"))
            {
               
            }

              
           
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                pictureBox1.Image = new Bitmap(PaintStuff.DrawMonsterRectangles(dataGridView2), new Size(512, 512)) ;
            }catch(Exception ex) { MessageBox.Show(ex.ToString() + "\n" + ex.StackTrace); }
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }
    }


  

}


// .map == 16 slices of textures each is 512x512
/*
private void button4_Click(object sender, EventArgs e)
{


    Bitmap[] imgs = new Bitmap[16];
    var temp = AtumZip.ZippedFile.UnzipItems(@"C:\Users\Zeyden\Desktop\3031.map");

    for(int i = 0; i < imgs.Length; i++)
    {              
        imgs[i] = new Bitmap(new MemoryStream(temp[i].Data));
    }

    Bitmap endimage = new Bitmap(2048, 2048);

    Graphics g = Graphics.FromImage(endimage);
    int x_pos = 0, y_pos = 0;

    for(int i = 0; i < imgs.Length; i++)
    {      
        g.DrawImage(imgs[i], x_pos, y_pos,512,512);
        x_pos += 512;
        if (x_pos == 2048)
        {
            x_pos = 0;
            y_pos += 512;
        }
    }
    Bitmap ttt = new Bitmap(endimage,new Size(endimage.Width/4,endimage.Height/4));      
    image = new Bitmap(512, 512);  
    ttt.RotateFlip(RotateFlipType.Rotate180FlipNone);

    var idx = MapFormatter.GetIndex("tileinfo", "WaterType");
    var temp2 = (_DatData[2] as object[]).Select(x => (uint)(x as object[])[idx]).ToArray();
    for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
            for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)

                    if(temp2[x + 256 * y] == 1)
                    { 
                           ttt.SetPixel(2 * (255 - y) + i, 2 * x + j, test);                                                  
                    }




    var idx2 = MapFormatter.GetIndex("tileinfo", "EventType");
    var temp3 = (_DatData[2] as object[]).Select(x => (byte)(x as object[])[idx2]).ToArray();
    for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
            for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)
                    if(temp3[x + 256 * y] == 1)
                    {
                        image.SetPixel(2 * (255 - y) + i, 2 * x + j, Color.Black);
                    }

    pictureBox2.Image = ttt;

}

Color test = new Color();     
private void button5_Click(object sender, EventArgs e)
{
    colorDialog1.ShowDialog();
    test =  colorDialog1.Color;

}
*/


     //private void DAT_OpenFile_MenuItem_Click(object sender, EventArgs e)
     //{
     //    _DatData = null;
     //    OpenFileDialog ofd = new OpenFileDialog();
     //    ofd.ShowDialog();

     //    using (var source = File.OpenRead(ofd.FileName)) _DatData = MapFormatter.Read(source);
     //    var table = MapFormatter.GetSchema("tileinfo");
     //    foreach (var row in (_DatData[2] as object[])) table.Rows.Add(row as object[]);       
     //    dataGridView1.DataSource = table;

     //    NewFormDrawBitmap("Bitmap From (.dat)", BitmapType.DAT).Show();

     //    using (var source = File.OpenRead(ofd.FileName + ".sma")) _SmaData = SmaFormatter.Read(source, 256);
     //    var temp = SmaFormatter.GetSchema("tileinfo");
     //    foreach (var row in (_SmaData[1] as object[])) temp.Rows.Add(row as object[]);
     //    DataGridViewLBL.Text = "data from (.sma) " + ofd.SafeFileName + ".sma";
     //    dataGridView2.DataSource = temp;

     //    BitmapLBL.Text = "curent " + ofd.SafeFileName + ".sma contains this bitmap!";
     //}

     /*
      */
     //private void button1_Click_1(object sender, EventArgs e)
     //{
     //    using (var source = File.OpenRead(LoadingData.directory + 3031)) //3018
     //        _DatData = MapFormatter.Read(source);

     //    dataGridView2.DataSource = MapFormatter.GetFormatter(3).GetSchema(_DatData[3] as object[][]);


     //    image = new Bitmap(512, 512);

     //    var idx = MapFormatter.GetIndex("tileinfo", "WaterType");
     //    var temp = (_DatData[2] as object[]).Select(x => (uint)(x as object[])[idx]).ToArray();
     //    for (int x = 0; x < 256; x++) for (int y = 0; y < 256; y++)
     //            for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++)

     //                    switch (temp[x + 256 * y])
     //                    {
     //                        case 1:
     //                            image.SetPixel(2 * (255 - y) + i, 2 * x + j, Color.LightBlue);
     //                            break;

     //                        default:
     //                            image.SetPixel(2 * (255 - y) + i, 2 * x + j, Color.Green);
     //                            break;
     //                    }


     //    pictureBox1.Image = image;
     //}






     //private Form NewFormDrawBitmap(string description,BitmapType btype_SMA_DAT)
     //{
     //    Form pForm = new Form();
     //    PictureBox pbox = new PictureBox();
     //    pForm.Text = description;
     //    pForm.Width = 535;
     //    pForm.Height = 555;
     //    pbox.Width = 512;
     //    pbox.Height = 512;

     //    if (btype_SMA_DAT == BitmapType.DAT)
     //    {
     //        makebitmap(pbox, BitmapType.DAT);
     //    }
     //    if(btype_SMA_DAT == BitmapType.SMA)
     //    {
     //        makebitmap(pbox,BitmapType.SMA);
     //    }
     //    pForm.Controls.Add(pbox);

     //    return pForm;
     //}



     //working export of pad bitmap to the sma 256x256!

     /*
             private void exportTileInfoToolStripMenuItem_Click(object sender, EventArgs e)
             {
                 if (textBox1.Text == "")
                 {
                     MessageBox.Show("eventparam1 is empty fill it up before saving!", "Warning!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                 }
                 else
                 {
                     SaveFileDialog sfd = new SaveFileDialog();
                     sfd.Filter = "Ares Sma Files (*.sma) | *.sma";
                     sfd.ShowDialog();

                     for (int i = 0; i < 256 * 256; i++)
                     {
                         //9234 pad num
                         if (int.Parse(dataGridView1.Rows[i].Cells[6].Value.ToString()) == 3)
                         {
                             dataGridView2.Rows[i].Cells[5].Value = dataGridView1.Rows[i].Cells[5].Value; // landable or not 0 or 1
                             dataGridView2.Rows[i].Cells[6].Value = dataGridView1.Rows[i].Cells[6].Value; // 3
                             dataGridView2.Rows[i].Cells[7].Value = (int.Parse(textBox1.Text));
                         }

                     }
                     var dgvRows = dataGridView2.Rows.OfType<DataGridViewRow>().Where(x => !x.IsNewRow);
                     _SmaData[1] = dgvRows.Select(x => x.Cells.OfType<DataGridViewCell>().Select(y => y.Value).ToArray()).ToArray();

                     using (var destination = new FileStream(sfd.FileName, FileMode.Create, FileAccess.Write))
                         SmaFormatter.Write(destination, _SmaData);

                     MessageBox.Show("File " + sfd.FileName + "\n was created", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
                     BitmapLBL.Text = "New Bitmap That was saved in Sma File!";
                     makebitmap(pictureBox1, BitmapType.SMA);
                 }
             }

         */


     /*load files */
// private void LoadFileBTN_Click(object sender, EventArgs e)
//{
//    treeView1.Nodes.Clear();
//    LoadingData.GetListSma(treeView1);
//}