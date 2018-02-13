using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

using Library;

namespace AresTool
{
    public partial class EditItemForm : Form
    {
        int type;

        int itemkind;

        int CurrentRowIdx { get { return dataGridView1.CurrentCell.RowIndex; } }
        int CurrentColIdx { get { return dataGridView1.CurrentCell.ColumnIndex; } }

        object CurrentVal { get { return dataGridView1.CurrentCell.Value; } }

        new MainForm Owner { get { return base.Owner as MainForm; } }

        public EditItemForm(int type)
        {
            IsActive = true;
            IsLoaded = false;

            InitializeComponent();

            this.type = type;

            itemkind = -1;
        }


        public bool IsActive { get; private set; }
        public bool IsLoaded { get; private set; }



        private bool refreshpending;
        private bool refreshagain;

        public void SetDataSource()
        {
            Action update = () =>
            {
                dataGridView1.EndEdit();

                var rows = Owner.SelectedRows;

                if (type == 0) LoadAttribute(rows);
                else if (type == 1) LoadParameter(rows);
                else if (type == 2) LoadAll(rows);
            };

            Action act = () =>
            {
                Thread.Sleep(50);

                while (refreshagain)
                {
                    refreshagain = false;
                    Thread.Sleep(150);
                }

                refreshpending = false;

                if (IsLoaded) Invoke(update);
            };

            if (!refreshpending)
            {
                refreshpending = true;
                refreshagain = false;

                new Thread(new ThreadStart(act)).Start();
            }
            else refreshagain = true;
        }




        private void EditItemForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            IsActive = false;
            IsLoaded = false;

            //MessageBox.Show(Size.ToString());
        }

        private void EditItemForm_Load(object sender, EventArgs e)
        {
            if (type == 0) SetupAttribute();
            else if (type == 1) SetupParameter();
            else if (type == 2) SetupAll();

            SetDataSource();

            IsLoaded = true;
        }


        private void dataGridView1_CurrentCellDirtyStateChanged(object sender, EventArgs e)
        {
            if (dataGridView1.IsCurrentCellDirty)
            {
                dataGridView1.CommitEdit(DataGridViewDataErrorContexts.Commit);

                if (dataGridView1.CurrentCell is DataGridViewCheckBoxCell)
                {
                    dataGridView1.EndEdit();

                    var cell = (dataGridView1.CurrentCell as DataGridViewCheckBoxCell);

                    if (((CheckState)cell.Value) == CheckState.Indeterminate) cell.Value = CheckState.Unchecked;
                }


                try
                {
                    if (type == 0 && CurrentColIdx == 1) SaveAttribute();
                    if (type == 1 && CurrentColIdx > 0) SaveParameter();
                    if (type == 2 && CurrentColIdx == 1) SaveAll();
                }
                catch (Exception) { }
            }
        }





        private void SetupAttribute()
        {
            Text = "ItemAttribute <- Works w/ multiselect";

            var col1 = new DataGridViewTextBoxColumn();
            col1.Name = "AttributeName";
            col1.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col1.ReadOnly = true;
            col1.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView1.Columns.Add(col1);


            var col2 = new DataGridViewCheckBoxColumn();
            col2.Name = "Enabled";
            col2.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col2.ThreeState = true;
            col2.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView1.Columns.Add(col2);


            var col3 = new DataGridViewTextBoxColumn();
            col3.Name = "value";
            col3.Visible = false;
            col3.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView1.Columns.Add(col3);


            foreach (var pair in Info.ItemAttributeMap) dataGridView1.Rows.Add(pair.Value, CheckState.Indeterminate, pair.Key);
        }

        private void SetupParameter()
        {
            Text = "Parameters <- Works w/ multiselect";

            var col1 = new DataGridViewTextBoxColumn();
            col1.Name = "DesParam";
            col1.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col1.ReadOnly = true;
            col1.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView1.Columns.Add(col1);


            var col2 = new DataGridViewComboBoxColumn();
            col2.Name = "Description";
            col2.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col2.SortMode = DataGridViewColumnSortMode.NotSortable;
            foreach (var pair in Info.DesParameterMap) col2.Items.Add(pair.Value);

            dataGridView1.Columns.Add(col2);


            var col3 = new DataGridViewTextBoxColumn();
            col3.Name = "Value";
            col3.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col3.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView1.Columns.Add(col3);


            for (int i = 0; i < 8; i++) dataGridView1.Rows.Add(null, null, null);
        }

        private void SetupAll()
        {
            Text = "All Around Editor <- Nope";

            var col1 = new DataGridViewTextBoxColumn();
            col1.Name = "Name";
            col1.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col1.SortMode = DataGridViewColumnSortMode.NotSortable;
            col1.ReadOnly = true;

            dataGridView1.Columns.Add(col1);


            var col2 = new DataGridViewTextBoxColumn();
            col2.Name = "Value";
            col2.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col2.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView1.Columns.Add(col2);


            var col3 = new DataGridViewTextBoxColumn();
            col3.Name = "Index";
            col3.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            col3.SortMode = DataGridViewColumnSortMode.NotSortable;
            col3.Visible = false;

            dataGridView1.Columns.Add(col3);


            foreach (var field in FormatterOmi.GetFormatter("ti_Item").Fields)
            {
                if (field.Name.StartsWith("Disabled")) continue;
                if (field.Name.StartsWith("DestParameter")) continue;
                if (field.Name.StartsWith("ParameterValue")) continue;
                if (field.Name.StartsWith("ItemAttribute")) continue;

                dataGridView1.Rows.Add(field.Name, null, field.Name);

                dataGridView1.ReadOnly = true;
            }

            var cell = new DataGridViewComboBoxCell();
            foreach (var pair in Info.KindMap) cell.Items.Add(pair.Value);

            dataGridView1[1, 1] = cell;
        }


        private void LoadAttribute(IEnumerable<DataGridViewRow> rows)
        {
            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                bool first = ((ulong)rows.First().Cells["ItemAttribute"].Value & (uint)row.Cells["value"].Value) > 0;

                var allsame = rows.All(x => (((ulong)x.Cells["ItemAttribute"].Value & (uint)row.Cells["value"].Value) > 0) == first);

                var state = CheckState.Indeterminate;

                if (allsame) state = first ? CheckState.Checked : CheckState.Unchecked;

                row.Cells["Enabled"].Value = state;
            }
        }

        private void LoadParameter(IEnumerable<DataGridViewRow> rows)
        {
            for (int i = 0; i < 8; i++)
            {
                var des0 = (ushort)rows.First().Cells["DestParameter" + (i + 1)].Value;
                var val0 = (float)rows.First().Cells["ParameterValue" + (i + 1)].Value;

                var samedes = rows.All(x => (ushort)x.Cells["DestParameter" + (i + 1)].Value == des0);
                var sameval = rows.All(x => (float)x.Cells["ParameterValue" + (i + 1)].Value == val0);


                dataGridView1[0, i].Value = samedes ? des0.ToString() : "<different options>";

                if (samedes && Info.DesParameterMap.ContainsKey(des0)) dataGridView1[1, i].Value = Info.DesParameterMap[des0];
                else dataGridView1[1, i].Value = null;

                dataGridView1[2, i].Value = sameval ? val0.ToString() : "<different options>";
            }
        }

        private void LoadAll(IEnumerable<DataGridViewRow> rows)
        {
            if (rows?.Count() != 1)
            {
                foreach (DataGridViewRow row in dataGridView1.Rows) row.Cells[1].Value = null;

                dataGridView1.ReadOnly = true;
            }
            else
            {
                itemkind = (byte)rows.First().Cells["Kind"].Value;

                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    var info = Info.itemInfo[row.Cells[2].Value as string].First(x => x.Item3((byte)itemkind));

                    row.Cells[0].Value = info.Item1;

                    var val = rows.First().Cells[row.Cells[2].Value.ToString()].Value;

                    if ((string)row.Cells[2].Value == "Kind")
                    {
                        if (Info.KindMap.ContainsKey((byte)val)) row.Cells[1].Value = Info.KindMap[(byte)val];
                        else row.Cells[1].Value = null;
                    }
                    else row.Cells[1].Value = val;
                }

                dataGridView1.ReadOnly = false;
                dataGridView1.Columns[0].ReadOnly = true;
                dataGridView1.Rows[0].ReadOnly = true;
            }
        }


        private void SaveAttribute()
        {
            var state = (CheckState)CurrentVal;

            if (state != CheckState.Indeterminate) Owner.UpdateSelectedAttribute((uint)dataGridView1["value", CurrentRowIdx].Value, state == CheckState.Checked);
        }

        private void SaveParameter()
        {
            if (CurrentColIdx == 1)
            {
                string val = CurrentVal.ToString();

                var key = Info.DesParameterMap.First(x => x.Value == val).Key;
                dataGridView1.Rows[dataGridView1.CurrentCell.RowIndex].Cells[0].Value = key;

                Owner.UpdateSelected("DestParameter" + (CurrentRowIdx + 1), key);
            }

            if (CurrentColIdx == 2)
            {
                var val = float.Parse(CurrentVal.ToString());

                Owner.UpdateSelected("ParameterValue" + (CurrentRowIdx + 1), val);
            }
        }

        private void SaveAll()
        {
            string name = (string)dataGridView1.CurrentCell.OwningRow.Cells[2].Value;

            if (name == "Kind" && CurrentVal != null)
            {
                var key = Info.KindMap.First(x => x.Value == (string)CurrentVal).Key;
                Owner.UpdateSelected(name, key);
            }

            else Owner.UpdateSelected(name, CurrentVal);
        }


        #region Tool Tips for All Around Editor

        int[] lastindex = new int[] { -1, -1 };

        Point lastPoint;

        private void dataGridView1_CellMouseEnter(object sender, DataGridViewCellEventArgs e)
        {
            if (type != 2) return;

            lastindex[0] = e.ColumnIndex;
            lastindex[1] = e.RowIndex;

            lastPoint = Cursor.Position;

            timer1.Start();

            dataGridView1.CellMouseMove += dataGridView1_CellMouseMove;
        }

        private void dataGridView1_CellMouseLeave(object sender, DataGridViewCellEventArgs e)
        {
            if (type != 2) return;

            dataGridView1.CellMouseMove -= dataGridView1_CellMouseMove;

            timer1.Stop();
        }

        private void dataGridView1_CellMouseMove(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (lastPoint != Cursor.Position)
            {
                timer1.Stop();

                lastPoint = Cursor.Position;
                toolTip1.Hide(this);

                timer1.Start();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Stop();

            if (type != 2) return;

            int colidx = lastindex[0];
            int rowidx = lastindex[1];

            if (colidx == 0 && rowidx >= 0)
            {
                var info = Info.itemInfo[dataGridView1[2, rowidx].Value as string].First(x => x.Item3((byte)itemkind));

                var cellRect = dataGridView1.GetCellDisplayRectangle(colidx, rowidx, false);

                toolTip1.Show(info.Item2,
                      this,
                      dataGridView1.Location.X + cellRect.X + cellRect.Size.Width,
                      dataGridView1.Location.Y + cellRect.Y + cellRect.Size.Height,
                      30000);    // Duration: 30 seconds.
            }
        }

        #endregion
    }
}
