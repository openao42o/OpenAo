using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AresTool
{
    public partial class DataGridViewForm : Form
    {
        public DataGridViewForm(object[] data)
        {
            InitializeComponent();

            dataGridView1.DataSource = data;
        }
    }
}
