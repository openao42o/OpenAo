using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

using Formatter.Inf;
using Library;

namespace ObjectInfoEditor
{
    /// <summary>
    /// This form is an objectinfo editor. It displays data in two DataGridViews.
    /// </summary>
    public partial class ObjInfEditor : Form
    {        
        const string AppName = "Object Info Editor";

        /// <summary>
        /// Filename used for saving or null.
        /// </summary>
        string Filename { get; set; }

        /// <summary>
        /// Character file name (found in objectinfo file header) or null.
        /// </summary>
        string CharacterFilename { get; set; }

        /// <summary>
        /// Current DataSource of the first DataGridView (which is bound to bsDGV1 now).
        /// </summary>
        DataTableEx DataSource => (bsDGV1.DataSource as DataTableEx);
        
        /// <summary>
        /// True if the datatables have been modified since they were loaded.
        /// </summary>
        bool HasUnsavedChanges { get; set; }

        /// <summary>
        /// BindingSource helper object used as data source for dataGridView1
        /// </summary>
        BindingSource bsDGV1;

        /// <summary>
        /// BindingSource helper object used as data source for dataGridView2
        /// </summary>
        BindingSource bsDGV2;

        /// <summary>
        /// Initializes the form.
        /// </summary>
        public ObjInfEditor()
        {
            InitializeComponent();
            
            Text = AppName;

            Filename = null;

            CharacterFilename = null;

            HasUnsavedChanges = false;

            bsDGV1 = new BindingSource();
            bsDGV2 = new BindingSource();

            dataGridView1.DataSource = bsDGV1;
            dataGridView2.DataSource = bsDGV2;

            // force the DataGridView1 to immediately add new rows into its DataTable
            dataGridView1.CellValueChanged += (sender, e) =>
            {
                bsDGV1.EndEdit();
                dataGridView1.NotifyCurrentCellDirty(true);
                dataGridView1.EndEdit();
                dataGridView1.NotifyCurrentCellDirty(false);
            };

            // force the DataGridView2 to immediately add new rows into its DataTable
            dataGridView2.CellValueChanged += (sender, e) =>
            {
                bsDGV2.EndEdit();
                dataGridView2.NotifyCurrentCellDirty(true);
                dataGridView2.EndEdit();
                dataGridView2.NotifyCurrentCellDirty(false);
            };
        }

        /// <summary>
        /// Should be called when the filename changes.
        /// </summary>
        private void UpdateResource(object[] LoadedInfo)
        {
            if (LoadedInfo != null)
            {
                // save the character file name
                CharacterFilename = (LoadedInfo[0] as object[])[0] as string;
                
                var dataformatter = ObjectInfoFormatter.GetFormatter("data");
                var effsformatter = ObjectInfoFormatter.GetFormatter("effects");

                // create an empty DataTableEx that matches our data format
                var data = dataformatter.GetSchema<DataTableEx>();

                // iterate the body conditions
                foreach (var row in LoadedInfo[1] as IEnumerable<object[]>)
                {
                    // create a new row
                    var newrow = data.NewRow();

                    // create a DataTable and fill it with the effects of this row
                    var effs = effsformatter.GetSchema(row[1] as IEnumerable<object[]>);

                    // assign the body condition data to the row
                    newrow.ItemArray = row[0] as object[];
                    // tag the effects DataTable
                    newrow.Tag = effs;

                    // set up the appropriate events
                    effs.ColumnChanged += (sender, e) => DataChanged();
                    effs.RowChanged += (sender, e) => DataChanged();
                    effs.RowDeleted += (sender, e) => DataChanged();

                    // effs.ColumnChanged += (sender, e) => newrow[dataformatter["EffectNum"]] = effs.Rows.Count;
                    effs.RowChanged += (sender, e) => newrow[dataformatter["EffectNum"]] = effs.Rows.Count;
                    effs.RowDeleted += (sender, e) => newrow[dataformatter["EffectNum"]] = effs.Rows.Count;
                    
                    // add the new row
                    data.Add(newrow);
                }

                // set up appropriate events for the first DataTable
                data.ColumnChanged += (sender, e) => DataChanged();
                data.RowChanged += (sender, e) => DataChanged();
                data.RowDeleted += (sender, e) => DataChanged();

                // if a row is added by the user
                data.TableNewRow += (sender, e)
                    // we need to properly construct the effects' datatable and tag it
                    => (e.Row as DataRowEx).Tag = ObjectInfoFormatter.GetFormatter("effects").GetSchema();


                bsDGV1.DataSource = data;
                bsDGV2.DataSource = null;

                dataGridView1.Refresh();
                dataGridView2.Refresh();

                Text = Filename;
            }

            else
            {
                Filename = null;
                CharacterFilename = null;

                bsDGV1.DataSource = null;
                bsDGV2.DataSource = null;

                dataGridView1.Refresh();
                dataGridView2.Refresh();

                Text = AppName;
            }

            HasUnsavedChanges = false;
        }

        /// <summary>
        /// Should be called when the selection of the first datagridview changes.
        /// </summary>
        private void SelectionChanged()
        {
            var selectedrows = dataGridView1
                .SelectedCells
                .OfType<DataGridViewCell>()
                .Select(x => x.OwningRow)
                .Distinct();

            if (selectedrows.Count() != 1)
            {
                bsDGV2.DataSource = null;
                dataGridView2.Refresh();
            }

            else
            {
                var selrow = selectedrows.Single();
                
                var row = (DataRowEx)(selrow.DataBoundItem as DataRowView)?.Row;
                bsDGV2.DataSource = row?.Tag;
                dataGridView2.Refresh();
            }
        }

        /// <summary>
        /// Should be called when any change to the datatables occurs
        /// </summary>
        private void DataChanged()
        {
            if (!HasUnsavedChanges) Text += "*";

            HasUnsavedChanges = true;
        }

        /// <summary>
        /// Opens the specified file for reading.
        /// </summary>
        /// <param name="filename">The specified file's name.</param>
        private void OpenFile(string filename)
        {
            object[] LoadedInfo = null;

            try
            {
                Filename = filename;

                using (var source = File.OpenRead(filename))
                {
                    //if (OperationMode == 0)
                    LoadedInfo = ObjectInfoFormatter.Read(source);
                    //if (OperationMode == 1) LoadedInfo = EffectInfoFormatter.Read(source);
                }
            }
            catch
            {
                Filename = null;
            }
            finally
            {
                UpdateResource(LoadedInfo);

                SelectionChanged();
            }
        }

        /// <summary>
        /// Saves the current DataSource to the specified file.
        /// </summary>
        /// <param name="filename">The specified file's name.</param>
        private void SaveFile(string filename)
        {
            dataGridView1.EndEdit();
            dataGridView2.EndEdit();


            var name = CharacterFilename;
            var number = DataSource.Rows.Count;

            var header = new object[] { name, number };

            // pack all data
            var data = from r0 in DataSource.Rows.OfType<DataRowEx>()
                       select new object[2] {
                           r0.ItemArray,
                           from r1 in (r0.Tag as DataTable).Rows.OfType<DataRow>()
                           select r1.ItemArray
                       };

            var info = new object[2] { header, data };

            using (var dest = File.OpenWrite(filename))

                ObjectInfoFormatter.Write(dest, info);


            HasUnsavedChanges = false;
            Text = filename;
        }

        /// <summary>
        /// Event that is used to open files with dragging, supports only a single file.
        /// </summary>
        private void ObjInfEditor_DragEnter(object sender, DragEventArgs e)
        {
            // allow files only
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            
                // allow single file only
                if ((e.Data.GetData(DataFormats.FileDrop) as string[]).Length == 1)

                    e.Effect = DragDropEffects.Copy;
        }

        /// <summary>
        /// Event that is used to open files with dragging, supports only a single file.
        /// </summary>
        private void ObjInfEditor_DragDrop(object sender, DragEventArgs e)
        {
            string filename = (e.Data.GetData(DataFormats.FileDrop) as string[]).Single();

            OpenFile(filename);
        }


        private void dataGridView1_Click(object sender, EventArgs e) => SelectionChanged();

        private void dataGridView1_SelectionChanged(object sender, EventArgs e) => SelectionChanged();

        private void dataGridView1_UserAddedRow(object sender, DataGridViewRowEventArgs e) => SelectionChanged();
        

        /// <summary>
        /// Event that is used to save when ctrl-s is pressed.
        /// </summary>
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (Filename != null && keyData == (Keys.Control | Keys.S)) SaveFile(Filename);

            return base.ProcessCmdKey(ref msg, keyData);
        }
    }
}
