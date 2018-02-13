using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AresTool
{
    public partial class UserInputBox : Form
    {
        private bool accepted { get; set; }

        public string Value { get { return textBox1.Text; } }

        public UserInputBox(string text)
        {
            accepted = false;
            InitializeComponent();
            textBox1.Text = text;
        }
        
        public new string ShowDialog()
        {
            base.ShowDialog();

            if (accepted) return textBox1.Text;

            return null;
        }

        public new DialogResult ShowDialog(IWin32Window owner)
        {
            base.ShowDialog(owner);

            if (accepted) return DialogResult.OK;

            return DialogResult.Cancel;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            Capture = true;
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape) Close();
            if (e.KeyCode == Keys.Enter)
            {
                accepted = true;
                Close();
            }
        }

        protected override void OnMouseCaptureChanged(EventArgs e)
        {
            if (!this.Capture)
            {
                if (!this.RectangleToScreen(this.DisplayRectangle).Contains(Cursor.Position))
                {
                    this.Close();
                }
                else
                {
                    this.Capture = true;
                }
            }

            base.OnMouseCaptureChanged(e);
        }
    }
}
