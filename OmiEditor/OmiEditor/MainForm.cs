using System;
using System.IO;
using System.Net;
using System.Data;
using System.Linq;
using System.Drawing;
using System.Threading;
using System.Diagnostics;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Runtime.Serialization.Formatters.Binary;

using Library;
using Formatter.Omi;

namespace AresTool
{
    public partial class MainForm : Form
    {
        EditItemForm[] forms;
        
        object[] omi_0;

        object[][] ti_item;

        DataTableEx hidden { get; set; }

        bool modified;


        /// <summary>
        /// Gets any row that contains selected DataGridViewCells.
        /// </summary>
        public IEnumerable<DataGridViewRow> SelectedRows => dataGridView.SelectedCells?.OfType<DataGridViewCell>().Select(c => c.OwningRow).Distinct();


        public string LastSelect { get; set; }

     

        public MainForm()
        {
            forms = new EditItemForm[3] { null, null, null };
            
            ti_item = null;

            hidden = null;

            LastSelect = "Enter a filter here, e.g: Kind = 35 and -NO_TRANSFER and +QUEST_ITEM";

            modified = false;

            InitializeComponent();

            dataGridView.DoubleBuffered(true);
            
            //AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-eff\objectinfo.inf");
            //AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-eff\effectinfo.inf");
            //AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-tex\spreff.tex");
            //AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-tex\item.tex");
            //AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-tex\bigitem.tex");
            //AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-snd\sound.dat");
            
            //AtumZip.ZippedFile.ZipFolder(@"C:\users\null\desktop\objectinfo", @"C:\users\null\desktop\objectinfo.inf");
            //AtumZip.ZippedFile.ZipFolder(@"C:\users\null\desktop\effectinfo", @"C:\users\null\desktop\effectinfo.inf");

            //Close();
        }

        /// <summary>
        /// Initialization of the form done asynchronously.
        /// </summary>
        void AsyncLoad()
        {
            try
            {
                using (var source = new FileStream(@"res-tex\omi.tex", FileMode.Open, FileAccess.Read))

                    omi_0 = FormatterOmi.Read(source);


                if (!Directory.Exists("Res-Dev")) Directory.CreateDirectory("Res-Dev");

                var formatter = new BinaryFormatter();

                if (!File.Exists(@"Res-Dev\csitem"))
                {
                    using (var dest = new FileStream(@"Res-Dev\csitem", FileMode.Create, FileAccess.Write))
                        formatter.Serialize(dest, omi_0[0]);
                }

                using (var source = new FileStream(@"Res-Dev\csitem", FileMode.Open, FileAccess.Read))
                    ti_item = formatter.Deserialize(source) as object[][];


                MakeHidden(ti_item);

                Action bind = () =>
                {
                    dataGridView.DataSource = hidden;
                    WriteLine("Done - You can start by double-clicking or right-clicking a cell.");
                };

                Invoke(bind);
            }
            catch (Exception ex)
            {
                ErrorLogger.LogException(ex);

                Action onerror = () =>
                {
                    MessageBox.Show(ex.Message, "GRRRRR!!!!!!");
                    Close();
                };

                Invoke(onerror);
            }
        }


        private void MainForm_Load(object sender, EventArgs e)
        {
            WriteLine("Wait, I am preparing the database for you, lazy ***!");

            new Task(AsyncLoad).Start();
        }



        private void MakeHidden(object[] rows)
        {
            var formatter = FormatterOmi.GetFormatter("ti_Item");

            var itemnumidx = formatter["ItemNum"];

            hidden = formatter.GetSchema<DataTableEx>();

            hidden.PrimaryKey = new [] { hidden.Columns[itemnumidx] };

            for (uint i = 0; i < rows.Length; i++)
            {
                var newrow = hidden.NewRow();

                newrow.ItemArray = rows[i] as object[];

                newrow.Tag = i;

                hidden.Add(newrow);
            }
        }


        private void WriteLine(string line)
        {
            textBoxOutput.Focus();
            textBoxOutput.AppendText(line + "\n");
        }




        private IEnumerable<ToolStripItem> GetMenuEnableDisable()
        {
            bool disable = SelectedRows.Any(x => !(bool)x.Cells["Disabled"].Value);
            bool enable = SelectedRows.Any(x => (bool)x.Cells["Disabled"].Value);

            if (disable)
            {
                ToolStripItem tool1 = new ToolStripMenuItem();
                tool1.Text = "DISABLE";
                tool1.Click += (x, y) => { UpdateSelected("Disabled", true); };

                yield return tool1;
            }

            if (enable)
            {
                ToolStripItem tool2 = new ToolStripMenuItem();
                tool2.Text = "ENABLE";
                tool2.Click += (x, y) => { UpdateSelected("Disabled", false); };

                yield return tool2;
            }
        }

        private IEnumerable<ToolStripItem> GetMenuSaveDiscard()
        {
            var allrows = SelectedRows.Select(x => hidden.Rows.Find(x.Cells["ItemNum"].Value) as DataRowEx);

            var changedrows = allrows.Where(x => !x.ItemArray.SequenceEqual(ti_item[(uint)x.Tag] as object[]));

            if (changedrows.Any())
            {

                ToolStripItem srefresh = new ToolStripMenuItem();
                srefresh.Text = "DISCARD SELECTED";
                srefresh.Click += (x, y) => { RefreshSelected(changedrows); };

                yield return srefresh;

                ToolStripItem ssave = new ToolStripMenuItem();
                ssave.Text = "SAVE SELECTED";
                ssave.Click += (x, y) => { SaveSelected(changedrows); };

                yield return ssave;
            }
        }

        private void ShowMenu(int? col = null, int? row = null)
        {
            try
            {
                var ctxtMenuItem = new ContextMenuStrip();



                if (row >= 0 && col >= 0) ctxtMenuItem.Items.AddRange(GetMenuEnableDisable().ToArray());

                if (row >= 0 && col >= 0 && modified) ctxtMenuItem.Items.AddRange(GetMenuSaveDiscard().ToArray());

                if (row != -1)
                {
                    ToolStripItem selector = new ToolStripMenuItem();
                    selector.Text = "SET FILTER";
                    selector.Click += (x, y) => ShowFilter();
                    ctxtMenuItem.Items.Add(selector);
                }

                if (row == -1 && col >= 0)
                {
                    ToolStripItem show = new ToolStripMenuItem();
                    show.Text = "Show all";
                    show.Click += (x, y) => { for (int i = 0; i < dataGridView.Columns.Count; i++) if (!dataGridView.Columns[i].Name.StartsWith("__")) dataGridView.Columns[i].Visible = true; };
                    ctxtMenuItem.Items.Add(show);

                    if (dataGridView.Columns[col.Value].Name != "ItemNum")
                    {
                        ToolStripItem hide = new ToolStripMenuItem();
                        hide.Text = "Hide this column";
                        hide.Click += (x, y) => { dataGridView.Columns[col.Value].Visible = false; };
                        ctxtMenuItem.Items.Add(hide);
                    }

                    ToolStripItem hidereq = new ToolStripMenuItem();
                    hidereq.Text = "Hide Requirements";
                    hidereq.Click += (x, y) => { foreach (DataGridViewColumn z in dataGridView.Columns) if (z.Name.StartsWith("Req")) z.Visible = false; };
                    ctxtMenuItem.Items.Add(hidereq);

                    ToolStripItem hideparam = new ToolStripMenuItem();
                    hideparam.Text = "Hide Parameters";
                    hideparam.Click += (x, y) => { foreach (DataGridViewColumn z in dataGridView.Columns) if (z.Name.StartsWith("ParameterValue") || z.Name.StartsWith("DestParameter")) z.Visible = false; };
                    ctxtMenuItem.Items.Add(hideparam);

                    ToolStripItem hideskill = new ToolStripMenuItem();
                    hideskill.Text = "Hide Skill Related";
                    hideskill.Click += (x, y) => { foreach (DataGridViewColumn z in dataGridView.Columns) if (z.Name.StartsWith("Skill") || z.Name == "NextSkill") z.Visible = false; };
                    ctxtMenuItem.Items.Add(hideskill);
                }

                ctxtMenuItem.Show(MousePosition);
            }
            catch (Exception) { }
        }

        
        private void ShowFilter()
        {
            var box = new UserInputBox(LastSelect);
            box.Location = MousePosition;
            box.StartPosition = FormStartPosition.Manual;
            if (box.ShowDialog(this) != DialogResult.OK) return;

            LastSelect = box.Value;
            
            var cmd = box.Value;

            // replace the strings

            if (cmd.Contains('@')) foreach (var pair in Info.KindMap)
            {
                if (!cmd.Contains('@')) break;

                cmd = cmd.Replace("@" + pair.Value, pair.Key.ToString());
            }

            if (cmd.Contains('+') || cmd.Contains('-')) foreach (var pair in Info.ItemAttributeMap)
            {
                var temp = "(Convert((ItemAttribute / " + pair.Key + ") - 0.5, System.UInt64) % 2)";
                        
                cmd = cmd.Replace("+" + pair.Value, temp + " > 0");
                cmd = cmd.Replace("-" + pair.Value, temp + " = 0");
            }


            try { hidden.DefaultView.RowFilter = cmd; }
            catch (Exception ex) { WriteLine(ex.Message); }
        }


        private void ShowHelpers()
        {
            for (int x = 0; x < forms.Length; x++)
            {
                if (forms[x] == null)
                {
                    forms[x] = new EditItemForm(x);

                    Rectangle workingArea = Screen.GetWorkingArea(this);
                    forms[x].StartPosition = FormStartPosition.Manual;

                    switch (x)
                    {
                        case 0:

                            forms[x].Size = new Size(355, 724);
                            forms[x].Location = new Point(workingArea.Right - 355, workingArea.Top);
                            break;

                        case 1:

                            forms[x].Size = new Size(251, 279);
                            forms[x].Location = new Point(workingArea.Right - 251, workingArea.Bottom - 279);
                            break;

                        case 2:

                            forms[x].Size = new Size(420, 878);
                            forms[x].Location = new Point(workingArea.Left, workingArea.Top);
                            break;
                    }

                    forms[x].Show(this);
                }
                else if (!forms[x].IsActive)
                {
                    var pos = forms[x].Location;
                    var size = forms[x].Size;

                    forms[x] = new EditItemForm(x);
                    forms[x].Size = size;
                    forms[x].Location = pos;
                    forms[x].StartPosition = FormStartPosition.Manual;

                    forms[x].Show(this);
                }
                else if (forms[x].IsLoaded) forms[x].SetDataSource();
            }
        }

        


        private void ReloadOMI()
        {
            WriteLine("You asked for it,");

            Action reload = () =>
            {
                ti_item = (omi_0[0] as object[][]).Clone() as object[][];

                MakeHidden(ti_item);

                Action done = () =>
                {
                    dataGridView.DataSource = hidden;
                    WriteLine("and here's a fresh copy of game's data.");
                };

                Invoke(done);
            };

            new Task(reload).Start();

            modified = false;
        }
        
        private void RefreshOMI()
        {
            WriteLine("Just a sec, I will discard all current changes...");

            Action copy = () =>
            {
                MakeHidden(ti_item);

                Action done = () =>
                {
                    dataGridView.DataSource = hidden;
                    WriteLine("Done!");
                };

                Invoke(done);
            };

            if (modified) new Task(copy).Start();
            else WriteLine("Nothing to save.");

            modified = false;
        }

        private void SaveOMI()
        {
            WriteLine("Ok I will write these down, it could take me some time though...");

            Action save = () =>
            {
                ti_item = hidden.AsEnumerable().Select(x => x.ItemArray).ToArray();

                Action invalidate = () => dataGridView.Invalidate();

                Invoke(invalidate);

                var formatter = new BinaryFormatter();

                using (var dest = new FileStream(@"Res-Dev\csitem", FileMode.Create, FileAccess.Write))

                    formatter.Serialize(dest, ti_item);

                Action done = () => WriteLine("Oh I finished already!!");

                Invoke(done);
            };

            if (modified) new Task(save).Start();
            else WriteLine("Nothing to save.");

            modified = false;
        }


        private void UploadPatch()
        {
            var allrows = ti_item.Select(x => new Tuple<object[], object[]>(
                
                x as object[],
                (omi_0[0] as object[][]).First(y => (uint)(y as object[])[1] == (uint)(x as object[])[1]) as object[]));

            var changedrows = allrows.Where(x => !x.Item1.SequenceEqual(x.Item2));

            if (!changedrows.Any()) return;

            var patch = changedrows.Select(x => x.Item1).ToArray();

            var formatter = new BinaryFormatter();

            byte[] buffer;

            using (var stream = new MemoryStream())
            {
                formatter.Serialize(stream, patch);

                buffer = new byte[stream.Length + 4];

                Array.Copy(BitConverter.GetBytes(stream.Length), buffer, 4);

                Array.Copy(stream.ToArray(), 0, buffer, 4, stream.Length);
            }

            Action upload = () =>
            {
                uint response = AsynchronousClient.StartClient(this, buffer);

                if (response == 0) Invoke(new Action(() => WriteLine("I think something went wrong...")));

                if (response == 0xFFFFFFFF) Invoke(new Action(() => WriteLine("Server says all data was received, let's see what happens now...")));
            };

            new Task(upload).Start();
        }


        private void RefreshSelected(IEnumerable<DataRowEx> rows)
        {
            WriteLine("Just a sec,");

            Action copy = () =>
            {
                var watch = new Stopwatch();

                watch.Start();

                foreach (var row in rows) row.ItemArray = ti_item[(uint)row.Tag] as object[];

                watch.Stop();

                Action done = () =>
                {
                    dataGridView.Invalidate();
                    if (watch.ElapsedMilliseconds < 1100) WriteLine("Pfft that was a piece of cake.");
                    else if (watch.ElapsedMilliseconds < 5100) WriteLine("OK....");
                    else WriteLine("-_-");
                };

                Invoke(done);
            };

            new Task(copy).Start();
        }

        private void SaveSelected(IEnumerable<DataRowEx> rows)
        {
            WriteLine("Ok I will do it...");

            Action save = () =>
            {
                var watch = new Stopwatch();

                watch.Start();

                foreach (var row in rows) ti_item[(uint)row.Tag] = row.ItemArray;

                watch.Stop();

                Action invalidate = () => dataGridView.Invalidate();

                Invoke(invalidate);

                var formatter = new BinaryFormatter();

                using (var dest = new FileStream(@"Res-Dev\csitem", FileMode.Create, FileAccess.Write)) formatter.Serialize(dest, ti_item);

                Action done1 = () => WriteLine("Peasy!!");
                Action done2 = () => WriteLine("Hmmmm.....");
                Action done3 = () => WriteLine("@@");

                if (watch.ElapsedMilliseconds < 1100) Invoke(done1);
                else if (watch.ElapsedMilliseconds < 5100) Invoke(done2);
                else Invoke(done3);
            };

            new Task(save).Start();
        }






        public void UpdateSelected(string name, object value)
        {
            foreach (var row in SelectedRows)
            {
                if (row.Cells[name].Value.Equals(value)) continue;


                var itemnum = (uint)row.Cells["ItemNum"].Value;

                var hiddenrow = hidden.Rows.Find(itemnum);

                
                hiddenrow[name] = value;

                modified = true;

                // after changing the value
                // the row could be invisible due to a filter
                if (row.Index != -1) dataGridView.InvalidateRow(row.Index);
            }
        }

        public void UpdateSelectedAttribute(ulong value, bool set)
        {
            foreach (var row in SelectedRows)
            {
                var val = (ulong)row.Cells["ItemAttribute"].Value;

                if (set) val |= value;

                else val &= (~value);

                if (row.Cells["ItemAttribute"].Value.Equals(val)) continue;


                var itemnum = (uint)row.Cells["ItemNum"].Value;

                var hiddenrow = hidden.Rows.Find(itemnum);

                
                hiddenrow["ItemAttribute"] = val;

                modified = true;

                // after changing the value
                // the row could be invisible due to a filter
                if (row.Index >= -1) dataGridView.InvalidateRow(row.Index);
            }
        }




        private void dataGridView_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (hidden == null) return;

            if (e.ColumnIndex >= 0 && e.RowIndex >= 0 && !dataGridView[e.ColumnIndex, e.RowIndex].Selected)

                dataGridView.CurrentCell = dataGridView[e.ColumnIndex, e.RowIndex];

            if (e.Button == MouseButtons.Right) ShowMenu(e.ColumnIndex, e.RowIndex);
        }
      
        private void dataGridView_SelectionChanged(object sender, EventArgs e)
        {
            if (hidden == null) return;

            for (int x = 0; x < forms.Length; x++) if (forms[x] != null && forms[x].IsActive && forms[x].IsLoaded) forms[x].SetDataSource();
        }

        private void dataGridView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (hidden == null) return;

            if (e.Button == MouseButtons.Left) ShowHelpers();
        }

        private void dataGridView_MouseClick(object sender, MouseEventArgs e)
        {
            if (hidden == null) return;

            if (e.Button == MouseButtons.Right) ShowMenu(null);
        }

        private void dataGridView_RowPrePaint(object sender, DataGridViewRowPrePaintEventArgs e)
        {
            if (hidden == null) return;

            var row = dataGridView.Rows[e.RowIndex];

            var itemnum = (uint)row.Cells["ItemNum"].Value;

            var currentrow = hidden.Rows.Find(itemnum) as DataRowEx;
            var savedrow = ti_item[(uint)currentrow.Tag] as object[];
            var savedrow0 = (omi_0[0] as object[][])[(uint)currentrow.Tag] as object[];

            if (!currentrow.ItemArray.SequenceEqual(savedrow))
            {
                row.DefaultCellStyle.BackColor = Color.PaleVioletRed;
                row.DefaultCellStyle.SelectionBackColor = Color.DarkRed;
            }
            else if (!savedrow.SequenceEqual(savedrow0))
            {
                row.DefaultCellStyle.BackColor = Color.LightSteelBlue;
                row.DefaultCellStyle.SelectionBackColor = Color.SlateBlue;
            }
            else
            {
                row.DefaultCellStyle.BackColor = dataGridView.DefaultCellStyle.BackColor;
                row.DefaultCellStyle.SelectionBackColor = dataGridView.DefaultCellStyle.SelectionBackColor;
            }
        }



        private void butSave_Click(object sender, EventArgs e) => SaveOMI();

        private void butDiscard_Click(object sender, EventArgs e) => RefreshOMI();

        private void butReload_Click(object sender, EventArgs e) => ReloadOMI();

        private void butUpload_Click(object sender, EventArgs e) => UploadPatch();

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private void discardAllChangesToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void helpToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }
    }

    public class Info
    {
        public static readonly Dictionary<byte, string> KindMap = new Dictionary<byte, string>
        {
            { 0, "AUTOMATIC" },
            { 1, "VULCAN" },
            { 2, "DUALIST" },
            { 3, "CANNON" },
            { 4, "RIFLE" },
            { 5, "GATLING" },
            { 6, "LAUNCHER" },
            { 7, "MASSDRIVE" },
            { 8, "ROCKET" },
            { 9, "MISSILE" },
            { 10, "BUNDLE" },
            { 11, "MINE" },
            { 12, "SHIELD" },
            { 13, "DUMMY" },
            { 14, "FIXER" },
            { 15, "DECOY" },
            { 16, "DEFENSE" },
            { 17, "SUPPORT" },
            { 18, "ENERGY" },
            { 19, "INGOT" },
            { 20, "CARD" },
            { 21, "ENCHANT" },
            { 22, "TANK" },
            { 23, "BULLET" },
            { 24, "QUEST" },
            { 25, "RADAR" },
            { 26, "COMPUTER" },
            { 27, "GAMBLE" },
            { 28, "PREVENTION_DELETE_ITEM" },
            { 29, "BLASTER" },
            { 30, "RAILGUN" },
            { 31, "ACCESSORY_UNLIMITED" },
            { 32, "ACCESSORY_TIMELIMIT" },
            { 33, "INFLUENCE_BUFF" },
            { 34, "INFLUENCE_GAMEEVENT" },
            { 35, "RANDOMBOX" },
            { 36, "MARK" },
            { 37, "SKILL_SUPPORT_ITEM" },
            { 38, "PET_ITEM" },

            { 50, "SKILL_ATTACK" },
            { 51, "SKILL_DEFENSE" },
            { 52, "SKILL_SUPPORT" },
            { 53, "SKILL_ATTRIBUTE" },

            { 100, "ITEMKIND_FOR_MON_PRIMARY" },
            { 101, "ITEMKIND_FOR_MON_GUN" },
            { 102, "ITEMKIND_FOR_MON_BEAM" },
            { 103, "ITEMKIND_FOR_MON_ALLATTACK" },
            { 104, "ITEMKIND_FOR_MON_SKILL" },
            { 105, "ITEMKIND_FOR_MON_RANGE_ATTACK" },
            { 150, "ITEMKIND_FOR_MON_SECONDARY" },
            { 151, "ITEMKIND_FOR_MON_ROCKET" },
            { 152, "ITEMKIND_FOR_MON_MISSILE" },
            { 153, "ITEMKIND_FOR_MON_BUNDLE" },
            { 154, "ITEMKIND_FOR_MON_MINE" },
            { 155, "ITEMKIND_FOR_MON_SHIELD" },
            { 156, "ITEMKIND_FOR_MON_DUMMY" },
            { 157, "ITEMKIND_FOR_MON_FIXER" },
            { 158, "ITEMKIND_FOR_MON_DECOY" },
            { 159, "ITEMKIND_FOR_MON_FIRE" },
            { 160, "ITEMKIND_FOR_MON_OBJBEAM" },
            { 161, "ITEMKIND_FOR_MON_STRAIGHTBOOM" },
            { 162, "ITEMKIND_FOR_MON_MAIN_ARMAMENT" },
        };

        public static readonly Dictionary<uint, string> ItemAttributeMap = new Dictionary<uint, string>
        {
            { 0x00000001, "AUTO_PICK_UP" },
            { 0x00000002, "NO_TRANSFER" },
            { 0x00000004, "QUEST_ITEM" },
            { 0x00000008, "TIME_LIMITE" },
            { 0x00000010, "KILL_MARK_ITEM" },
            { 0x00000020, "SKILL_SUPPORT_ITEM" },
            { 0x00000040, "DELETED_TIME_LIMITE" },
            { 0x00000080, "ACCOUNT_POSSESSION" },
            { 0x00000100, "ARENA_ITEM" },
            { 0x00000200, "SUPER_GAMBLE_ITEM" },
            { 0x00000400, "PACKAGE_ITEM" },
            { 0x00000800, "FOR_ONLY_GAME_MASTER" },
            { 0x00001000, "UNIQUE_ITEM" },
            { 0x00002000, "BAZAAR_ITEM" },
            { 0x00004000, "LEGEND_ITEM" },
            { 0x00008000, "FORBID_ARENA_COPY_ITEM" },
            { 0x00010000, "CASH_ITEM" },
            { 0x00020000, "CASH_ITEM_PREMIUM_CARD" },
            { 0x00040000, "LUCKY_ITEM" },
            { 0x00080000, "DELETED_TIME_LIMITE_AFTER_USED" },
            { 0x00100000, "MISSION_MASTER" },
            { 0x00200000, "ROBOT_ARMOR" },
            { 0x00400000, "CANNOT_USE_INFINITY" },
            { 0x00800000, "ONLY_USE_INFINITY" },
            { 0x01000000, "CHECK_SKILL_RESISTANCE_STATE" },
            { 0x02000000, "NOT_STORE_SAVE" },
            { 0x04000000, "WAREHOUSE_SHARE_BANNED" },
            { 0x08000000, "HYPER_GAMBLE_ITEM" },
            { 0x10000000, "TRASH_ONLY" },
        };

        public static readonly Dictionary<ushort, string> DesParameterMap = new Dictionary<ushort, string>
        {
            { 0, "DES_NULL" },

            { 1, "DES_ATTACK_PART" },

            { 2, "DES_DEFENSE_PART" },

            { 3, "DES_FUEL_PART" },

            { 4, "DES_SOUL_PART" },

            { 5, "DES_SHIELD_PART" },

            { 6, "DES_DODGE_PART" },

            { 106, "DES_ALL_PART" },

            { 7, "DES_BODYCONDITION" },

            { 8, "DES_ENDURANCE_01" },

            { 9, "DES_ENDURANCE_02" },

            { 10, "DES_CHARGING_01" },

            { 11, "DES_CHARGING_02" },

            { 12, "DES_PROPENSITY" },

            { 13, "DES_HP" },

            { 89, "DES_DP" },

            { 14, "DES_SP" },

            { 15, "DES_EP" },

            { 16, "DES_SPRECOVERY" },

            { 17, "DES_HPRECOVERY" },

            { 18, "DES_MINATTACK_01" },

            { 71, "DES_MAXATTACK_01" },

            { 19, "DES_MINATTACK_02" },

            { 72, "DES_MAXATTACK_02" },

            { 20, "DES_ATTACKPROBABILITY_01" },

            { 21, "DES_ATTACKPROBABILITY_02" },

            { 22, "DES_DEFENSE_01" },

            { 23, "DES_DEFENSE_02" },

            { 24, "DES_DEFENSEPROBABILITY_01" },

            { 25, "DES_DEFENSEPROBABILITY_02" },

            { 26, "DES_SKILLPROBABILITY_01" },

            { 64, "DES_SKILLPROBABILITY_02" },

            { 79, "DES_FACTION_01" },

            { 80, "DES_FACTION_02" },

            { 27, "DES_FACTIONRESISTANCE_01" },

            { 65, "DES_FACTIONRESISTANCE_02" },

            { 28, "DES_SPEED" },

            { 29, "DES_TRANSPORT" },

            { 30, "DES_MATERIAL" },

            { 31, "DES_REATTACKTIME_01" },

            { 32, "DES_REATTACKTIME_02" },

            { 33, "DES_ABRASIONRATE_01" },

            { 34, "DES_ABRASIONRATE_02" },

            { 35, "DES_RANGE_01" },

            { 36, "DES_RANGE_02" },

            { 37, "DES_RANGEANGLE_01" },

            { 38, "DES_RANGEANGLE_02" },

            { 39, "DES_MULTITAGET_01" },

            { 66, "DES_MULTITAGET_02" },

            { 40, "DES_EXPLOSIONRANGE_01" },

            { 67, "DES_EXPLOSIONRANGE_02" },

            { 41, "DES_UNIT" },

            { 42, "DES_REVISION" },

            { 43, "DES_FACTIONPROBABILITY_01" },

            { 68, "DES_FACTIONPROBABILITY_02" },

            { 44, "DES_SHOTNUM_01" },

            { 69, "DES_SHOTNUM_02" },

            { 45, "DES_MULTINUM_01" },

            { 70, "DES_MULTINUM_02" },

            { 46, "DES_ATTACKTIME_01" },

            { 47, "DES_ATTACKTIME_02" },

            { 48, "DES_TIME_01" },

            { 49, "DES_TIME_02" },

            { 73, "DES_OVERHITTIME_01" },

            { 74, "DES_OVERHITTIME_02" },

            { 50, "DES_UNITKIND" },

            { 51, "DES_ITEMKIND" },

            { 52, "DES_SUMMON" },

            { 53, "DES_GRADUAL_HP_UP" },

            { 81, "DES_GRADUAL_DP_UP" },

            { 54, "DES_GRADUAL_SP_UP" },

            { 55, "DES_GRADUAL_EP_UP" },

            { 56, "DES_IN_TIME_HP_UP" },

            { 82, "DES_IN_TIME_DP_UP" },

            { 57, "DES_IN_TIME_SP_UP" },

            { 58, "DES_IN_TIME_EP_UP" },

            { 59, "DES_IMMEDIATE_HP_UP" },

            { 83, "DES_IMMEDIATE_DP_UP" },

            { 60, "DES_IMMEDIATE_SP_UP" },

            { 61, "DES_IMMEDIATE_EP_UP" },

            { 62, "DES_GROUNDMODE" },

            { 63, "DES_SIEGEMODE" },

            { 75, "DES_WEIGHT_01" },

            { 76, "DES_WEIGHT_02" },

            { 77, "DES_BULLET_01" },

            { 78, "DES_BULLET_02" },

            { 84, "DES_PRIMARY_WEAPON" },

            { 85, "DES_SECONDARY_WEAPON" },

            { 86, "DES_ALL_WEAPON" },

            { 87, "DES_CRITICALHITRATE_01" },

            { 88, "DES_CRITICALHITRATE_02" },

            { 90, "DES_WARP" },

            { 91, "DES_REACTION_RANGE" },

            { 92, "DES_RARE_FIX_NONE" },

            { 93, "DES_RARE_FIX_PREFIX" },

            { 94, "DES_RARE_FIX_SUFFIX" },

            { 95, "DES_RARE_FIX_BOTH" },

            { 96, "DES_REQ_MIN_LEVEL" },

            { 97, "DES_REQ_MAX_LEVEL" },

            { 98, "DES_WARP_OUTPOST" },

            { 99, "DES_CHAT_BLOCK" },


            { 100, "DES_CASH_STAT_ALL_INITIALIZE" },


            { 102, "DES_CASH_STAT_PART_INITIALIZE" },

            { 103, "DES_RARE_FIX_PREFIX_INITIALIZE" },

            { 104, "DES_RARE_FIX_SUFFIX_INITIALIZE" },

            { 105, "DES_ENCHANT_INITIALIZE" },


            { 108, "DES_CASH_STEALTH" },

            { 109, "DES_CASH_HP_AND_DP_UP" },

            { 110, "DES_CASH_GUILD_ALL_MEMBERS_SUMMON" },

            { 111, "DES_CASH_GUILD_MEMBER_SUMMON" },

            { 112, "DES_CASH_NORMAL_RESTORE" },

            { 113, "DES_CASH_SUPER_RESTORE" },

            { 114, "DES_CASH_GUILD" },

            { 115, "DES_CASH_MONSTER_SUMMON" },

            { 116, "DES_CASH_CHANGE_CHARACTERNAME" },

            { 117, "DES_CASH_SKILL_INITIALIZE" },

            { 118, "DES_CASH_CHANGE_PILOTFACE" },

            { 119, "DES_CASH_CRACKER_CARD" },





            { 121, "DES_SKILL_REDUCE_SHIELD_DAMAGE" },

            { 122, "DES_SKILL_INVINCIBLE" },

            { 123, "DES_SKILL_BIG_BOOM" },

            { 124, "DES_SKILL_HALLUCINATION" },

            { 125, "DES_SKILL_RANGEDOWN_01" },

            { 126, "DES_SKILL_RANGEDOWN_02" },

            { 127, "DES_SKILL_PROHIBITION_01" },

            { 128, "DES_SKILL_PROHIBITION_02" },

            { 129, "DES_ATTACK_RANGE_01" },

            { 130, "DES_ATTACK_RANGE_02" },

            { 131, "DES_INVISIBLE" },


            { 133, "DES_SKILL_DEFENSE_01" },

            { 134, "DES_SKILL_DEFENSE_02" },

            { 135, "DES_SKILL_FREEZINGBODY" },

            { 136, "DES_SKILL_REVERSECONTROL" },

            { 137, "DES_SKILL_GRADUAL_HPSHIELD_DOWN" },

            { 138, "DES_SKILL_SLOWMOVING" },

            { 139, "DES_SKILL_BOOSTEROFF" },

            { 140, "DES_SKILL_COLLISIONDAMAGE_DOWN" },


            { 142, "DES_SKILL_RANDOMTELEPORT" },

            { 143, "DES_SKILL_ATTACK_ALL" },

            { 144, "DES_SKILL_SCANNING" },

            { 145, "DES_SKILL_REVERSEENGINE" },

            { 146, "DES_SKILL_LOCKDOWN" },

            { 147, "DES_SKILL_STEALSP" },

            { 148, "DES_SKILL_SMARTSP" },

            { 149, "DES_SKILL_SUMMON_FORMATION_MEMBER" },

            { 150, "DES_SKILL_CANCEL_MAGIC" },

            { 151, "DES_SKILL_RANDOM_CANCEL" },

            { 152, "DES_SKILL_STOPMAGIC" },

            { 153, "DES_SKILL_CANCELALL" },

            { 154, "DES_SKILL_REACTIONSPEED" },

            { 155, "DES_SKILL_ENGINEANGLE" },

            { 156, "DES_SKILL_ENGINEBOOSTERANGLE" },



            { 157, "DES_DROP_EXP" },

            { 158, "DES_DROP_SPI" },

            { 159, "DES_DROP_ITEM" },

            { 160, "DES_HP_REPAIR_RATE_FLIGHTING" },

            { 161, "DES_DP_REPAIR_RATE" },

            { 162, "DES_SP_REPAIR_RATE" },



            { 163, "DES_BAZAAR_SELL" },

            { 164, "DES_BAZAAR_BUY" },



            { 165, "DES_KILLMARK_EXP" },



            { 166, "DES_HYPER_BOOSTER" },



            { 167, "DES_SKILL_CHAFF_HP" },

            { 168, "DES_SKILL_AIR_BOMBING" },

            { 169, "DES_SKILL_NO_WARNING" },

            { 170, "DES_SKILL_ROLLING_TIME" },

            { 171, "DES_SKILL_FULL_RECOVERY" },

            { 172, "DES_SKILL_CAMOUFLAGE" },

            { 173, "DES_SKILL_BARRIER" },

            { 174, "DES_SKILL_HYPERSHOT" },

            { 175, "DES_SKILL_SHIELD_PARALYZE" },


            { 176, "DES_WARHEAD_SPEED" },

            { 177, "DES_CHAT_ALL_INFLUENCE" },



            { 178, "DES_ENGINE_BOOSTER_TIME_UP" },

            { 179, "DES_ENGINE_MAX_SPEED_UP" },

            { 180, "DES_ENGINE_MIN_SPEED_UP" },

            { 181, "DES_ENGINE_BOOSTER_SPEED_UP" },

            { 182, "DES_ENGINE_GROUND_SPEED_UP" },

            { 183, "DES_RADAR_OBJECT_DETECT_RANGE" },

            { 184, "DES_PIERCE_UP_01" },

            { 185, "DES_PIERCE_UP_02" },

            { 186, "DES_ENGINE_ANGLE_UP" },

            { 187, "DES_ENGINE_BOOSTERANGLE_UP" },



            { 188, "DES_ITEM_BUFF_INFLUENCE" },

            { 189, "DES_ITEM_BUFF_PARTY" },



            { 190, "DES_ENCHANT_PREVENTION_DELETE_USE_ENCHANT" },

            { 191, "DES_ENCHANT_PREVENTION_DELETE_SAVE_ENCHANT" },

            { 192, "DES_ENCHANT_INCREASE_PROBABILITY" },




            { 193, "DES_SKILL_DAMAGE_DISTRIBUTION" },




            { 194, "DES_SHAPE_ITEM" },

            { 195, "DES_EFFECT_ITEM" },




            { 196, "DES_FIELD_STORE" },




            { 197, "DES_INIT_SHAPE_ITEM" },

            { 198, "DES_INIT_EFFECT_ITEM" },




            { 199, "DES_INCREASE_INVENTORY_SPACE" },

            { 200, "DES_INCREASE_STORE_SPACE" },




            { 201, "DES_ITEM_RESISTANCE" },

            { 202, "DES_ITEM_ADDATTACK" },

            { 203, "DES_ITEM_IGNOREDEFENCE" },

            { 204, "DES_ITEM_IGNOREAVOID" },

            { 205, "DES_ITEM_REDUCEDAMAGE" },

            { 206, "DES_ITEM_ADDATTACK_SEC" },

            { 207, "DES_ITEM_ONCE_RESISTANCE" },

            { 210, "DES_SKILL_MON_SILENCE_PRIMARY" },

            { 211, "DES_SKILL_MON_SILENCE_SECOND" },

            { 212, "DES_SKILL_MON_FREEZE_HP" },

            { 213, "DES_SKILL_MON_FREEZE_DP" },

            { 214, "DES_SKILL_MON_FREEZE_SP" },

            { 215, "DES_SKILL_MON_HOLD" },

            { 216, "DES_SKILL_MON_STEALING" },

            { 217, "DES_SKILL_MON_DRAIN" },

            { 218, "DES_SKILL_RELEASE" },

            { 219, "DES_SKILL_MON_SILENCE_SKILL" },




            { 220, "DES_PAIR_DRAIN_1_RATE" },

            { 221, "DES_PAIR_DRAIN_2_HP_DP_UP_RATE" },

            { 222, "DES_ANTI_DRAIN_RATE" },

            { 223, "DES_PAIR_REFLECTION_1_RATE" },

            { 224, "DES_PAIR_REFLECTION_2_DAMAGE_RATE" },

            { 225, "DES_ANTI_REFLECTION_RATE" },




            { 226, "DES_BLIND" },

            { 227, "DES_SUPERINTEND" },

            { 228, "DES_IMMEDIATE_HP_OR_DP_UP" },

            { 229, "DES_HIT_INVOKE_SKILL" },

            { 230, "DES_TIME_BOMB" },




            { 231, "DES_TRANSFORM_TO_MONSTER" },




            { 232, "DES_INFINITY_REENTRY_TICKET" },




            { 233, "DES_TRANSFORM_TO_GEAR" },




            { 234, "DES_PLUS_WARPOINT_RATE" },




            { 235, "DES_PCROOM_USE_CARD" },




            { 236, "DES_MAX_SP_UP" },




            { 237, "DES_WAR_POINT_UP" },




            { 389, "DES_DONATE_POINT_UP" },




            { 238, "DES_RARE_ITEM_DROP_RATE" },




            { 239, "DES_RARE_ITEM_PARTNER_SPEED" },




            { 240, "DES_RARE_ITEM_PARTNER_DAMAGE" },




            { 241, "DES_RARE_ITEM_HPDP" },



            { 242, "DES_SUMMON_POSITION_X" },

            { 243, "DES_SUMMON_POSITION_Y" },

            { 244, "DES_SUMMON_POSITION_Z" },




            { 245, "DES_MAPBUFF_RANDOM_ADD_REATTACKTIME" },

            { 246, "DES_MAPBUFF_RANDOM_ADD_TIME" },

            { 247, "DES_ENCHANT_ONCE_APPLY" },

            { 248, "DES_GAMBLE_RARE_ITEM_FIX" },



            { 249, "DES_GAMBLE_RARE_ITEM_FIX_STD" },

            { 250, "DES_GAMBLE_RARE_ITEM_FIX_ADV" },



            { 251, "DES_BUFF_PENALTY_RATIO" },

            { 252, "DES_BUFF_PENALTY_APPLIED_UNIT_KIND" },




            { 253, "DES_SKILLTYPE_CONSECUTIVE_VICTORIES" },

            { 254, "DES_SKILLTYPE_TRUN_AROUND" },



            { 255, "DES_SKILLTYPE_OUTPOST_BUFF" },


            { 300, "DES_SKILL_BUFF_MON_ATTACK_POWER" },

            { 301, "DES_SKILL_BUFF_MON_ATTACK_PROBABILITY" },

            { 302, "DES_SKILL_BUFF_MON_ATTACK_PIERCE" },

            { 303, "DES_SKILL_BUFF_MON_DEFENCE" },

            { 304, "DES_SKILL_BUFF_MON_DEFENCE_AVOID" },

            { 305, "DES_SKILL_BUFF_PVP_ATTACK_POWER" },

            { 306, "DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY" },

            { 307, "DES_SKILL_BUFF_PVP_ATTACK_PIERCE" },

            { 308, "DES_SKILL_BUFF_PVP_DEFENCE" },

            { 309, "DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY" },



            { 311, "DES_SKILL_SKILL_CANCEL_IMMUNE" },



            { 390, "DES_COLLECTION_ARMOR_INDEX" },




            { 499, "DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER" },

            { 500, "DES_BUFF_TO_RETURN_USER" },




            { 501, "DES_FIXED_TERM_SHAPE_TIME" },

            { 502, "DES_FIXED_TERM_ITEM_TIME" },



            { 503, "DES_WEB_DELETE_ITEM" },


            { 504, "DES_INGAME_SHOW" },



            { 505, "DES_CASH_CHANGE_NATION" },

            { 506, "DES_CASH_CHANGE_NATION_REQ_FAME" },






            { 600, "DES_OPTION_ITEM_DEFAULT_DESPARAM" },



            { 23000, "DES_PET_SOCKET_ITEM_AUTOKIT" },

            { 23001, "DES_PET_SOCKET_ITEM_AUTOSKILL" },

            { 23002, "DES_PET_SOCKET_ITEM_SPEED" },



            { 23900, "DES_PET_SLOT_ITEM_AUTOKIT_HP" },

            { 23901, "DES_PET_SLOT_ITEM_AUTOKIT_SHIELD" },

            { 23902, "DES_PET_SLOT_ITEM_AUTOKIT_SP" },



            { 23910, "DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR" },

            { 23911, "DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR" },

            { 23912, "DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR" },

            { 23913, "DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR" }, 
        };

        public static readonly Dictionary<string, Tuple<string, string, Func<byte, bool>>[]> itemInfo = new Dictionary<string, Tuple<string, string, Func<byte, bool>>[]>()
        {
	        {
		        "ItemNum",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ItemNum", "", b => true),
		        }
	        },
	        {
		        "Kind",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Kind", "", b => true),
		        }
	        },
	        {
		        "ItemName",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ItemName", "", b => true),
		        }
	        },
	        {
		        "AbilityMin",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "WeaponMinDamage", "", b => b > 0 && b < 15),
			        new Tuple<string, string, Func<byte, bool>>( "EngineMinSpeed", "", b => b == 17),
			        new Tuple<string, string, Func<byte, bool>>( "RadarStdWeaponRange", "", b => b == 25),
			        new Tuple<string, string, Func<byte, bool>>( "MinAmount", "Facts about Kind 35. DestParameter1 is chance % to drop a random Item (from the list specified by LinkItem). DestParameter2 is chance % to get SPI and DestParameter3 is chance % to get EXP. AbilityMin/Max affect the last two values.", b => b == 35),
			        new Tuple<string, string, Func<byte, bool>>( "HologramScale", "Scaling performed on a monster's object to render it as hologram.", b => b == 20),
			        new Tuple<string, string, Func<byte, bool>>( "AbilityMin", "", b => true),
		        }
	        },
	        {
		        "AbilityMax",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "WeaponMaxDamage", "", b => b > 0 && b < 15),
			        new Tuple<string, string, Func<byte, bool>>( "EngineMaxSpeed", "", b => b == 17),
			        new Tuple<string, string, Func<byte, bool>>( "RadarAdvWeaponRange", "", b => b == 25),
			        new Tuple<string, string, Func<byte, bool>>( "MaxAmount", "Facts about Kind 35. DestParameter1 is chance % to drop a random Item (from the list specified by LinkItem). DestParameter2 is chance % to get SPI and DestParameter3 is chance % to get EXP. AbilityMin/Max affect the last two values.", b => b == 35),
			        new Tuple<string, string, Func<byte, bool>>( "AbilityMax", "", b => true),
		        }
	        },
	        {
		        "ReqRace",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqRace", "", b => true),
		        }
	        },
	        {
		        "ReqAttackPart",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqAttackStat", "Required Attack Stat to equip item", b => true),
		        }
	        },
	        {
		        "ReqDefensePart",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqDefenseStat", "Required Defense Stat to equip item", b => true),
		        }
	        },
	        {
		        "ReqFuelPart",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqFuelStat", "Required Fuel Stat to equip item", b => true),
		        }
	        },
	        {
		        "ReqSoulPart",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqSpiritStat", "Required Spirit Stat to equip item", b => true),
		        }
	        },
	        {
		        "ReqShieldPart",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqShieldStat", "Required Shield Stat to equip item", b => true),
		        }
	        },
	        {
		        "ReqDodgePart",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqEvasionStat", "Required Evasion Stat to equip item", b => true),
		        }
	        },
	        {
		        "ReqUnitKind",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqGearKind", "Gear Type / Monster restriction", b => true),
		        }
	        },
	        {
		        "ReqMinLevel",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqMinLevel", "Required minimum level", b => true),
		        }
	        },
	        {
		        "ReqMaxLevel",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqMaxLevel", "Required maximum level", b => true),
		        }
	        },
	        {
		        "ReqItemKind",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqItemKind", "Required ItemKind", b => true),
		        }
	        },
	        {
		        "Weight",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Weight", "Weight of equipment etc.", b => true),
		        }
	        },
	        {
		        "HitRate",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "HitProbability", "Weapon Hit Probability", b => true),
		        }
	        },
	        {
		        "Defense",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Defense", "Not a single entry makes use of this variable", b => true),
		        }
	        },
	        {
		        "FractionResistance",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "PiercePercent", "Pierce percentage from weapons and mostly Monsters", b => true),
		        }
	        },
	        {
		        "NaturalFaction",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "NaturalFaction", "", b => true),
		        }
	        },
	        {
		        "SpeedPenalty",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "LateralSpeed", "Lateral Speed value on engines", b => true),
		        }
	        },
	        {
		        "Range",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "StdWeaponRange", "", b => b > 0 && b < 7),
			        new Tuple<string, string, Func<byte, bool>>( "AdvWeaponRange", "", b => b > 8 && b < 15),
			        new Tuple<string, string, Func<byte, bool>>( "BoosterSpeed", "", b => b == 17),
			        new Tuple<string, string, Func<byte, bool>>( "RadarObjectSensingRadius", "", b => b == 25),
			        new Tuple<string, string, Func<byte, bool>>( "SkillEffectRange", "Maximum range that a skill will have an effect. e.g. Big Boom has 1200 range (hits everyone within 1200m because it's AoE). Might need confirmation if it works the same for ABM, Ragings and things like Purify, Snare Shot etc.", b => b > 50 && b < 53),
			        new Tuple<string, string, Func<byte, bool>>( "Range", "", b => true),
		        }
	        },
	        {
		        "Position",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Position", "", b => true),
		        }
	        },
	        {
		        "Scarcity",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Scarcity", "Possibly to tell whether an item is acquireable by monster drops, not sure", b => true),
		        }
	        },
	        {
		        "Endurance",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ExpiresIn", "Shows time left (in hours) until an item will \"expire\" and disappear from your inventory. Apparently ALL armors have an \"Endurance\" of 30000 (1250 days) for some reason", b => true),
		        }
	        },
	        {
		        "AbrasionRate",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "AbrasionRate", "Something related to Monster actions ranging from 1-5", b => true),
		        }
	        },
	        {
		        "Charging",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "GroundSpeed", "", b => b == 17),
			        new Tuple<string, string, Func<byte, bool>>( "Charging", "Weapon ammunition (even for monsters), shield/energy adhesive value but also some unrelated things such as Infinity Field modules (7027200) and these weird PvP rank marks DA uses (9800000). Weird thing about the marks is that the variable value doesn't match the shield amount the actual items gives", b => true),
		        }
	        },
	        {
		        "Luck",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Luck", "", b => true),
		        }
	        },
	        {
		        "MinTradeQuantity",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "MinSaleQuantity", "Minimum quantity of an item you have to sell at once to get SPI for it. 7031700 (A-Type Shield Kit) for example has a sell value of 3902 SPI, but only if you sell 100 at once. Anything lower than that will not give SPI. Also probably used for item \"packs\" in Cash Shop (or any shop), for example if we put a pack of 500 A-Type SP Kits into Cash Shop this variable might be used to determine that you only pay per 500 units, rather than having to pay the full price for all 500", b => true),
		        }
	        },
	        {
		        "Price",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Price", "Price of an item when purchased from a shop. \"Selling Price\" on items is Price / 5", b => true),
		        }
	        },
	        {
		        "CashPrice",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "CashPrice", "This should be price for credit shop", b => true),
		        }
	        },
	        {
		        "ReAttacktime",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "KitCooldown", "", b => b == 18),
			        new Tuple<string, string, Func<byte, bool>>( "SkillCooldown", "Skill cooldown", b => b > 50 && b < 53),
			        new Tuple<string, string, Func<byte, bool>>( "ReAttacktime", "Weapon Reattack Time. Also used for Item/Skill cooldown in milliseconds", b => true),
		        }
	        },
	        {
		        "Time",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "OverheatTime", "Standard weapon Overheat Time in milliseconds", b => b > 0 && b < 7),
			        new Tuple<string, string, Func<byte, bool>>( "BoosterTime", "engine booster time in seconds", b => b == 17),
			        new Tuple<string, string, Func<byte, bool>>( "Time", "i don't know, nothing that has been suggested tho", b => b == 18),
			        new Tuple<string, string, Func<byte, bool>>( "Time", "Membership account status duration in days (\"Kind=20\", but there's a ton of \"Kind=20\" stuff that doesn't apply to value in days)", b => b == 20),
			        new Tuple<string, string, Func<byte, bool>>( "Duration", "duration of things like Charms (Kind=32) in milliseconds", b => b == 32),
			        new Tuple<string, string, Func<byte, bool>>( "RemainingTime", "\"Remaining Time\" on items in milliseconds, and probably a ton more", b => true),
		        }
	        },
	        {
		        "RepeatTime",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "RepeatTime", "Has either value 0 or 1 for anything unrelated to monster actions and various ranging values for anything related to monsters. Doesn't look like it's meaningful at all and better left alone", b => true),
		        }
	        },
	        {
		        "Material",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Material", "Used for all sorts of armors when Material>1, otherwise Material=1 are a bunch of monster related things. Everything related to neither of these have Material=0.... I think this should be a valid ResourceIndex (that is should be defined in effectinfo)", b => true),
		        }
	        },
	        {
		        "ReqMaterial",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReqMaterial", "", b => true),
		        }
	        },
	        {
		        "RangeAngle",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "TurnAngle / ???", "(When Kind=17) Turn Rate in radians for engines, maybe turn rate for monsters, (when Kind=25) something with radars that I can't figure out.", b => true),
		        }
	        },
	        {
		        "UpgradeNum",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "UpgradeNum", "Not a single entry makes use of this variable", b => true),
		        }
	        },
	        {
		        "LinkItem",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "LinkItem", "Links Contour Cards to their Lv. 60 armor, links various items that have some sort of relation (10x weapons are linked to \"Missile for Level 100\" for example, while Capsule type items are linked to others with similar effects and Blessings of Nation are linked to an identic version with no effect whatsoever", b => true),
		        }
	        },
	        {
		        "MultiTarget",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "MultiTarget", "Enables weapons to shoot at multiple targets as once, e.g. Snipe weapons. Value indicates numbers of silmutaniously fired targets", b => true),
		        }
	        },
	        {
		        "ExplosionRange",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ExplosionRange", "Enables weapons to have an explosion range, e.g. Terol weapons", b => true),
		        }
	        },
	        {
		        "ReactionRange",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ReactionRange", "", b => true),
		        }
	        },
	        {
		        "ShotNum",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "MissileNum", "Missiles shot per volley", b => true),
		        }
	        },
	        {
		        "MultiNum",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "VolleyNum", "Volleys per right click", b => true),
		        }
	        },
	        {
		        "AttackTime",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SkillChargeUpDuration", "Duration in milliseconds of the animation a skill plays until it takes effect", b => true),
		        }
	        },
	        {
		        "ReqSP",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SPCost", "SP cost of skills", b => true),
		        }
	        },
	        {
		        "SummonMonster",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SummonMonster", "Summons monster. Value = monster number", b => true),
		        }
	        },
	        {
		        "NextSkill",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "NextSkill", "", b => true),
		        }
	        },
	        {
		        "SkillLevel",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SkillLevel", "SkillLevel=100 are Special/Elite skills", b => true),
		        }
	        },
	        {
		        "SkillHitRate",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SkillHitRate", "", b => true),
		        }
	        },
	        {
		        "SkillType",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SkillType", "1 = Target Skills and Skills you just click once for their effect (Field Heals), 2 = Buffs and Debuffs, 3 = Toggle skills, 4 = Charge Shot and Hyper Shot", b => true),
		        }
	        },
	        {
		        "SkillTargetType",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SkillTargetType", "1 = Click to target any player, 2 = Target formation members, 9 = Infinity Field related, 23 = Target self only, 24 = Target enemy player only, 25 = Target invisible units", b => true),
		        }
	        },
	        {
		        "Caliber",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Caliber", "", b => true),
		        }
	        },
	        {
		        "OrbitType",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "OrbitType", "", b => true),
		        }
	        },
	        {
		        "ItemAttribute",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "ItemAttribute", "", b => true),
		        }
	        },
	        {
		        "BoosterAngle",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "InductionAngle", "", b => b > 8 && b < 15),
			        new Tuple<string, string, Func<byte, bool>>( "TurnRateBooster", "", b => b == 17),
			        new Tuple<string, string, Func<byte, bool>>( "BoosterAngle", "InductionAngle for Adv Weaps, TurnRateBooster for Engines.", b => true),
		        }
	        },
	        {
		        "CameraPattern",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "CameraPattern", "", b => true),
		        }
	        },
	        {
		        "SourceIndex",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "SourceIndex", "This is the number for the corresponding objectinfo.inf entry.", b => true),
		        }
	        },
	        {
		        "Description",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "Description", "", b => true),
		        }
	        },
	        {
		        "EnchantCheckDestParam",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "EnchantCheckDestParam", "", b => true),
		        }
	        },
	        {
		        "InvokingDestParamID",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "InvokingDestParamID", "", b => true),
		        }
	        },
	        {
		        "InvokingDestParamIDByUse",
		        new []
		        {
			        new Tuple<string, string, Func<byte, bool>>( "InvokingDestParamIDByUse", "", b => true),
		        }
	        },
        };
    }


    // State object for receiving data from remote device.
    public class StateObject
    {
        // Client socket.
        public Socket workSocket = null;
        // Size of receive buffer.
        public const int BufferSize = 4;
        // Receive buffer.
        public byte[] buffer = new byte[BufferSize];
    }

    public class AsynchronousClient
    {
        static MainForm owner;

        // The port number for the remote device.
        private const int port = 11000;

        // ManualResetEvent instances signal completion.
        private static ManualResetEvent connectDone = new ManualResetEvent(false);
        private static ManualResetEvent sendDone = new ManualResetEvent(false);
        private static ManualResetEvent receiveDone = new ManualResetEvent(false);

        // The response from the remote device.
        private static uint response = 0;

        public static uint StartClient(MainForm owner, byte[] data)
        {
            AsynchronousClient.owner = owner;

            // Connect to a remote device.
            try
            {
                // Establish the remote endpoint for the socket.
                IPEndPoint remoteEP = new IPEndPoint(IPAddress.Parse("150.140.211.213"), port);

                // Create a TCP/IP socket.
                Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                // Connect to the remote endpoint.
                client.BeginConnect(remoteEP, new AsyncCallback(ConnectCallback), client);
                connectDone.WaitOne();

                // Send test data to the remote device.
                Send(client, data);
                sendDone.WaitOne();

                // Receive the response from the remote device.
                Receive(client);
                receiveDone.WaitOne();

                // Write the response to the console.
                // Console.WriteLine("Response received : {0}", response);

                // Release the socket.
                client.Shutdown(SocketShutdown.Both);
                client.Close();
            }
            catch (Exception e)
            {
                owner.Invoke(new Action(() => MessageBox.Show(e.ToString(), "Network Error")));
            }

            return response;
        }

        private static void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.
                Socket client = (Socket)ar.AsyncState;

                // Complete the connection.
                client.EndConnect(ar);

                // Console.WriteLine("Socket connected to {0}", client.RemoteEndPoint.ToString());

                // Signal that the connection has been made.
                connectDone.Set();
            }
            catch (Exception e)
            {
                owner.Invoke(new Action(() => MessageBox.Show(e.ToString(), "Network Error")));
            }
        }

        private static void Receive(Socket client)
        {
            try
            {
                // Create the state object.
                StateObject state = new StateObject();
                state.workSocket = client;

                // Begin receiving the data from the remote device.
                client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
            }
            catch (Exception e)
            {
                owner.Invoke(new Action(() => MessageBox.Show(e.ToString(), "Network Error")));
            }
        }

        private static void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the state object and the client socket 
                // from the asynchronous state object.
                StateObject state = (StateObject)ar.AsyncState;
                Socket client = state.workSocket;

                // Read data from the remote device.
                int bytesRead = client.EndReceive(ar);

                if (bytesRead == 4) response = BitConverter.ToUInt32(state.buffer, 0);

                receiveDone.Set();
            }
            catch (Exception e)
            {
                owner.Invoke(new Action(() => MessageBox.Show(e.ToString(), "Network Error")));
            }
        }

        private static void Send(Socket client, byte[] data)
        {
            // Begin sending the data to the remote device.
            client.BeginSend(data, 0, data.Length, 0, new AsyncCallback(SendCallback), client);
        }

        private static void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.
                Socket client = (Socket)ar.AsyncState;

                // Complete sending the data to the remote device.
                int bytesSent = client.EndSend(ar);
                // Console.WriteLine("Sent {0} bytes to server.", bytesSent);

                // Signal that all bytes have been sent.
                sendDone.Set();
            }
            catch (Exception e)
            {
                owner.Invoke(new Action(() => MessageBox.Show(e.ToString(), "Network Error")));
            }
        }
    }
}
