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
using Formatter.Omi;
using Formatter.Inf;
using AtumZip;

namespace AresTool
{
    public partial class GenericForm : Form
    {        
        int CurrentRowIdx { get { return dataGridView.CurrentCell.RowIndex; } }
        int CurrentColIdx { get { return dataGridView.CurrentCell.ColumnIndex; } }

        object CurrentVal { get { return dataGridView.CurrentCell.Value; } }


        public GenericForm()
        {
            IsActive = true;
            IsLoaded = false;

            InitializeComponent();

            InitData();
        }


        public bool IsActive { get; private set; }
        public bool IsLoaded { get; private set; }


        
        private bool refreshlock;
        private bool refreshagain;

        public void UpdateData()
        {
            Action update = () =>
            {
                dataGridView.EndEdit();

                RecvData();
            };

            Action act = () =>
            {
                do {

                    if (IsLoaded) Invoke(update);
                    refreshagain = false;
                    Thread.Sleep(400);

                } while (refreshagain);

                refreshlock = false;
            };

            if (!refreshlock)
            {
                refreshlock = true;
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
            IsLoaded = true;

            UpdateData();
        }


        private void dataGridView1_CurrentCellDirtyStateChanged(object sender, EventArgs e)
        {
            if (dataGridView.IsCurrentCellDirty)
            {
                dataGridView.CommitEdit(DataGridViewDataErrorContexts.Commit);
                
                if (dataGridView.CurrentCell is DataGridViewCheckBoxCell)
                {
                    dataGridView.EndEdit();

                    var cell = (dataGridView.CurrentCell as DataGridViewCheckBoxCell);

                    if (((CheckState)cell.Value) == CheckState.Indeterminate) cell.Value = CheckState.Unchecked;
                }
                
                SendData();
            }
        }



        protected virtual void InitData()
        {
            dataGridView.Enabled = false;
        }

        protected virtual void RecvData()
        {
            dataGridView.Enabled = true;
        }

        protected virtual void SendData()
        {

        }
        

        //private void SetupAttribute()
        //{
        //    Text = "ItemAttribute <- Works w/ multiselect";

        //    var col1 = new DataGridViewTextBoxColumn();
        //    col1.Name = "AttributeName";
        //    col1.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col1.ReadOnly = true;
        //    col1.SortMode = DataGridViewColumnSortMode.NotSortable;

        //    dataGridView.Columns.Add(col1);


        //    var col2 = new DataGridViewCheckBoxColumn();
        //    col2.Name = "Enabled";
        //    col2.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col2.ThreeState = true;
        //    col2.SortMode = DataGridViewColumnSortMode.NotSortable;

        //    dataGridView.Columns.Add(col2);


        //    var col3 = new DataGridViewTextBoxColumn();
        //    col3.Name = "value";
        //    col3.Visible = false;
        //    col3.SortMode = DataGridViewColumnSortMode.NotSortable;

        //    dataGridView.Columns.Add(col3);


        //    foreach (var pair in Info.ItemAttributeMap) dataGridView.Rows.Add(pair.Value, CheckState.Indeterminate, pair.Key);
        //}

        //private void SetupParameter()
        //{
        //    Text = "Parameters <- Works w/ multiselect";

        //    var col1 = new DataGridViewTextBoxColumn();
        //    col1.Name = "DesParam";
        //    col1.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col1.ReadOnly = true;
        //    col1.SortMode = DataGridViewColumnSortMode.NotSortable;

        //    dataGridView.Columns.Add(col1);


        //    var col2 = new DataGridViewComboBoxColumn();
        //    col2.Name = "Description";
        //    col2.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col2.SortMode = DataGridViewColumnSortMode.NotSortable;
        //    foreach (var pair in Info.DesParameterMap) col2.Items.Add(pair.Value);
            
        //    dataGridView.Columns.Add(col2);


        //    var col3 = new DataGridViewTextBoxColumn();
        //    col3.Name = "Value";
        //    col3.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col3.SortMode = DataGridViewColumnSortMode.NotSortable;

        //    dataGridView.Columns.Add(col3);

            
        //    for (int i = 0; i < 8; i++) dataGridView.Rows.Add(null, null, null);
        //}

        //private void SetupAll()
        //{
        //    Text = "All Around Editor <- Nope";

        //    var col1 = new DataGridViewTextBoxColumn();
        //    col1.Name = "Name";
        //    col1.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col1.SortMode = DataGridViewColumnSortMode.NotSortable;
        //    col1.ReadOnly = true;

        //    dataGridView.Columns.Add(col1);


        //    var col2 = new DataGridViewTextBoxColumn();
        //    col2.Name = "Value";
        //    col2.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col2.SortMode = DataGridViewColumnSortMode.NotSortable;

        //    dataGridView.Columns.Add(col2);


        //    var col3 = new DataGridViewTextBoxColumn();
        //    col3.Name = "Index";
        //    col3.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
        //    col3.SortMode = DataGridViewColumnSortMode.NotSortable;
        //    col3.Visible = false;

        //    dataGridView.Columns.Add(col3);


        //    foreach (var rec in OmiFormatter.ItemFormat)
        //    {
        //        if (rec.Name.StartsWith("Disabled")) continue;
        //        if (rec.Name.StartsWith("DestParameter")) continue;
        //        if (rec.Name.StartsWith("ParameterValue")) continue;
        //        if (rec.Name.StartsWith("ItemAttribute")) continue;

        //        dataGridView.Rows.Add(rec.Name, null, rec.Name);

        //        dataGridView.ReadOnly = true;
        //    }

        //    var cell = new DataGridViewComboBoxCell();
        //    foreach (var pair in Info.KindMap) cell.Items.Add(pair.Value);

        //    dataGridView[1, 1] = cell;
        //}


        //private void LoadAttribute(IEnumerable<DataGridViewRow> rows)
        //{
        //    foreach (DataGridViewRow row in dataGridView.Rows)
        //    {
        //        bool first = ((ulong)rows.First().Cells["ItemAttribute"].Value & (uint)row.Cells["value"].Value) > 0;

        //        var allsame = rows.All(x => (((ulong)x.Cells["ItemAttribute"].Value & (uint)row.Cells["value"].Value) > 0) == first);

        //        var state = CheckState.Indeterminate;

        //        if (allsame) state = first ? CheckState.Checked : CheckState.Unchecked;

        //        row.Cells["Enabled"].Value = state;
        //    }
        //}

        //private void LoadParameter(IEnumerable<DataGridViewRow> rows)
        //{
        //    for (int i = 0; i < 8; i++)
        //    {
        //        var des0 = (ushort)rows.First().Cells["DestParameter" + (i + 1)].Value;
        //        var val0 = (float)rows.First().Cells["ParameterValue" + (i + 1)].Value;

        //        var samedes = rows.All(x => (ushort)x.Cells["DestParameter" + (i + 1)].Value == des0);
        //        var sameval = rows.All(x => (float)x.Cells["ParameterValue" + (i + 1)].Value == val0);


        //        dataGridView[0, i].Value = samedes ? des0.ToString() : "<different options>";

        //        if (samedes && Info.DesParameterMap.ContainsKey(des0)) dataGridView[1, i].Value = Info.DesParameterMap[des0];
        //        else dataGridView[1, i].Value = null;

        //        dataGridView[2, i].Value = sameval ? val0.ToString() : "<different options>";
        //    }
        //}

        //private void LoadAll(IEnumerable<DataGridViewRow> rows)
        //{
        //    if (rows?.Count() != 1)
        //    {
        //        foreach (DataGridViewRow row in dataGridView.Rows) row.Cells[1].Value = null;

        //        dataGridView.ReadOnly = true;
        //    }
        //    else
        //    {
        //        foreach (DataGridViewRow row in dataGridView.Rows)
        //        {
        //            var val = rows.First().Cells[row.Cells[2].Value.ToString()].Value;

        //            if ((string)row.Cells[2].Value == "Kind")
        //            {
        //                if (Info.KindMap.ContainsKey((byte)val)) row.Cells[1].Value = Info.KindMap[(byte)val];
        //                else row.Cells[1].Value = null;
        //            }
        //            else row.Cells[1].Value = val;
        //        }

        //        dataGridView.ReadOnly = false;
        //        dataGridView.Columns[0].ReadOnly = true;
        //        dataGridView.Rows[0].ReadOnly = true;
        //    }
        //}


        //private void SaveAttribute()
        //{
        //    var state = (CheckState)CurrentVal;

        //    if (state != CheckState.Indeterminate) Owner.UpdateSelectedAttribute((uint)dataGridView["value", CurrentRowIdx].Value, state == CheckState.Checked);
        //}

        //private void SaveParameter()
        //{
        //    if (CurrentColIdx == 1)
        //    {
        //        string val = CurrentVal.ToString();

        //        var key = Info.DesParameterMap.First(x => x.Value == val).Key;
        //        dataGridView.Rows[dataGridView.CurrentCell.RowIndex].Cells[0].Value = key;

        //        Owner.UpdateSelected("DestParameter" + (CurrentRowIdx + 1), key);
        //    }

        //    if (CurrentColIdx == 2)
        //    {
        //        var val = float.Parse(CurrentVal.ToString());

        //        Owner.UpdateSelected("ParameterValue" + (CurrentRowIdx + 1), val);
        //    }
        //}

        //private void SaveAll()
        //{
        //    string name = (string)dataGridView.CurrentCell.OwningRow.Cells[2].Value;

        //    if (name == "Kind" && CurrentVal != null)
        //    {
        //        var key = Info.KindMap.First(x => x.Value == (string)CurrentVal).Key;
        //        Owner.UpdateSelected(name, key);
        //    }

        //    else Owner.UpdateSelected(name, CurrentVal);
        //}
        
    }

    public class EffectView : GenericForm
    {
        ObjectInfoViewEx parent;

        EffectViewEx effectviewex;
        
        public EffectView(ObjectInfoViewEx parent)
        {
            this.parent = parent;
            effectviewex = null;
        }

        private void dataGridView_SlectionChanged(object sender, EventArgs e)
        {
            if (effectviewex == null || effectviewex.IsDisposed)

                effectviewex = new EffectViewEx(dataGridView);

            if (!effectviewex.Visible) effectviewex.Show();

            effectviewex.UpdateData();
        }

        protected override void InitData()
        {
            base.InitData();

            dataGridView.SelectionChanged += dataGridView_SlectionChanged;
        }

        protected override void RecvData()
        {
            base.RecvData();
            
            var table = ObjectInfoFormatter.GetSchema("effects");

            var tag = parent.Effects;

            if (tag == null) return;

            foreach (var row in tag) table.Rows.Add(row as object[]);

            dataGridView.DataSource = table;

            dataGridView_SlectionChanged(null, null);
        }
    }

    public class EffectViewEx : GenericForm
    {
        DataGridView parent;

        public EffectViewEx(DataGridView parent)
        {
            this.parent = parent;
        }

        protected override void InitData()
        {
            base.InitData();


            var col1 = new DataGridViewTextBoxColumn();

            col1.Name = "Name";
            col1.ReadOnly = true;
            col1.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            col1.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView.Columns.Add(col1);


            var col2 = new DataGridViewTextBoxColumn();

            col2.Name = "Value";
            col2.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            col2.SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView.Columns.Add(col2);


            var col3 = new DataGridViewTextBoxColumn();

            col3.Name = "Index";
            col3.Visible = false;

            dataGridView.Columns.Add(col3);
        }

        protected override void RecvData()
        {
            var effname = (parent.CurrentCell?.OwningRow?.Cells?[1]?.Value as string);

            if (effname == null || effname.Length < 8) return;

            effname = effname.Substring(0, 8);

            base.RecvData();

            dataGridView.Rows.Clear();


            //var effect = Form1.Effects[effname];

            var effect = (Owner as ItemViewer).effectinfo.Single(x => x.Name == effname).Data as object[];

            var header = effect[0] as object[];

            var headerformat = EffectInfoFormatter.HeaderFormatter.Fields;

            for (int i = 0; i < header.Length; i++) dataGridView.Rows.Add(headerformat[i].Name, header[i], i);


            var type = (uint)header[2];

            var data = effect[1] as object[];

            var dataformat = EffectInfoFormatter.GetFormat(type);

            for (int i = 0; i < data.Length; i++) dataGridView.Rows.Add(dataformat[i].Name, data[i], i);
        }

        protected override void SendData()
        {
            base.SendData();
        }

        public new DataGridView dataGridView { get { return base.dataGridView; } }
    }

    public class ObjectInfoViewEx : GenericForm
    {
        DataGridView parent;
        Func<DataGridViewRow, uint> func;

        EffectView effectview;

        public object[] Effects { get; private set; }

        public ObjectInfoViewEx(DataGridView parent, Func<DataGridViewRow, uint> func)
        {
            this.func = func;
            this.parent = parent;

            parent.SelectionChanged += (a, b) => UpdateData();

            effectview = new EffectView(this);
        }

        protected override void InitData()
        {
            base.InitData();
        }

        protected override void RecvData()
        {
            dataGridView.DataSource = null;
            dataGridView.Invalidate();

            var row = parent.CurrentCell?.OwningRow;

            if (row == null) return;

            var kind = (byte)row.Cells["Kind"]?.Value;
            var idx = func(row);

            if (idx < 9000000 && idx >= 8000000) idx += 4000000;

            

            base.RecvData();

            var objinf = (Owner as ItemViewer).objectinfo.Single(x => x.Name == idx.ToString("D8")).Data as object[];

            var table = ObjectInfoFormatter.GetSchema("data");

            foreach (var r in objinf[1] as object[]) table.Add((r as object[])[0] as object[], (r as object[])[1]);

            dataGridView.DataSource = table;
        }
        
        public new DataGridView dataGridView { get { return base.dataGridView; } }
    }
}
