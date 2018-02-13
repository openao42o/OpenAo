using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;




   // internal static MoveOrResize WorkType { get; set; }

 
   


public partial class Zbox : PictureBox
{
    private int thickness = 3;

    bool mousedown = false;
    Point xypos_on_click = new Point();
    public Zbox(Rectangle rec)// float scale, string[] tooltip)
    {
       
        DoubleBuffered = true;
        Location = new Point(rec.X, rec.Y);
        Width = rec.Width;
        Height = rec.Height;

        BackColor = Color.Transparent;
        

        BorderStyle = BorderStyle.FixedSingle;
        

        MouseDown += Zbox_MouseDown;
        MouseMove += Zbox_MouseMove;
        MouseUp += Zbox_MouseUp;
        MouseHover += Zbox_MouseHover;

    }

    private void Zbox_MouseHover(object sender, EventArgs e)
    {


        Rectangle a = DisplayRectangle;
        Rectangle b = new Rectangle(xypos_on_click.X, xypos_on_click.Y, 1, 1);

       
        // throw new NotImplementedException();
        if(a.IntersectsWith(b))
        {
            MessageBox.Show("intersected ");
        }
        
     


    }

    private void Zbox_MouseMove(object sender, MouseEventArgs e)
    {
        xypos_on_click.X = e.X;
        xypos_on_click.Y = e.Y;
        if (mousedown)
        {
            //for moving picturebox around 
            Left += (e.X -xypos_on_click.X);
            Top += (e.Y -xypos_on_click.Y);
        }
    }
    private void Zbox_MouseUp(object sender, MouseEventArgs e)
    {
        mousedown = false;     
    }
  
    private void Zbox_MouseDown(object sender, MouseEventArgs e)
    {
        if (e.Button == System.Windows.Forms.MouseButtons.Left)
        {
            mousedown = true;
            xypos_on_click.X = e.X;
            xypos_on_click.Y = e.Y;
        }
    }






}

