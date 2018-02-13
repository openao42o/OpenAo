using Formatter.Inf;
using Library;
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
    public partial class EffectView : Form
    {
        public EffectView()
        {
            InitializeComponent();
        }

        public DataTable DataSource
        {
            set
            {
                dataGridView1.DataSource = value;
                dataGridView1_Click(null, null);
            }
        }

        private void dataGridView1_Click(object sender, EventArgs e)
        {
            var effname = (((dataGridView1?.CurrentCell?.OwningRow?.DataBoundItem as DataRowView)?.Row as DataRowEx)?["EffectName"] as string)?.Substring(0, 8);

            if (effname == null) return;

            var data = (Owner as Form1).effects[effname];

            var table = EffectInfoFormatter.GetSchema((int)(uint)(data[0] as object[])[2]);

            table.Rows.Add(data[1] as object[]);

            dataGridView2.DataSource = table;
        }
    }
}
