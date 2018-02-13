using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;

using Library;
using Formatter.Omi;
using Formatter.Inf;
using Formatter.Dat;

namespace AresTool
{
    public partial class ItemViewer : Form
    {
        #region Resource Access

        private ResManager resmanager;

        public object[] omi => resmanager.omiinfo.Data as object[];

        public IEnumerable<CachedInfo> objectinfo => resmanager.objectinfo;
        public IEnumerable<CachedInfo> effectinfo => resmanager.effectinfo;

        public IEnumerable<CachedInfo> mapinfo => resmanager.mapinfo;

        #endregion

        public ItemViewer()
        {
            InitializeComponent();
            
            dataGridView.DoubleBuffered(true);

            // Initialize resource files and load them
            resmanager = new ResManager();

            treeView_Init();
        }
        

        #region internal helper functions

        /// <summary>
        /// Was in ItemViewer() moved here to clean up.
        /// Some kind of dependency checker which checks all new resource ideces.
        /// </summary>
        internal void helperfunc0()
        {
            //Func<uint, bool> checksourceindex = SourceIndex =>
            //{
            //    if (SourceIndex / 1000000 == 8) SourceIndex += 4000000;

            //    var filename = SourceIndex.ToString("D8");
            //    var filepath = string.Format(@"res-obj\" + filename + ".obj");

            //    var fileExists = File.Exists(filepath);
            //    var infExists = objectinfo.Any(x => x.Name == filename);

            //    if (!fileExists && File.Exists(newpath + filepath))

            //        File.Copy(newpath + filepath, filepath);

            //    var objinfpath = @"res-dev\objectinfo\" + filename;

            //    if (!infExists && File.Exists(newpath + objinfpath))
            //    {
            //        object[][] objinf;

            //        using (var source = File.OpenRead(newpath + objinfpath))

            //            objinf = ObjectInfoFormatter.Read(source)[1] as object[][];

            //        var soundidx = ObjectInfoFormatter.GetFormatter("data")["SoundFileName"];
            //        var effnameidx = ObjectInfoFormatter.GetFormatter("effects")["EffectName"];

            //        var newsounds = objinf.Select(x => (x[0] as object[])[soundidx] as string).Distinct()
            //        .Where(x => !File.Exists(@"res-dev\sound\" + x));

            //        File.AppendAllLines("newsounds.txt", newsounds.ToArray());

            //        var neweffs = objinf.SelectMany(x => (x[1] as object[][]).Select(y => y[effnameidx] as string)).Distinct()
            //        .Where(x => !File.Exists(@"res-dev\effectinfo\" + x));

            //        File.AppendAllLines("neweffs.txt", neweffs.ToArray());

            //        return true;
            //    }

            //    return false;
            //};

            //for (int i = 0; i < 2; ++i)
            //{
            //    //DBHelper.DBstuff.InsertDB(FormatterOmi.Formatters[i], (newomi[i] as object[][]).Except(omi[i] as object[][], FormatterOmi.RowComparers[i]));

            //    var idx = FormatterOmi.Formatters[i]["SourceIndex"];

            //    var newsourceindeces = (newomi[i] as object[][]).Select(x => (uint)x[idx]).Distinct().Except((omi[i] as object[][]).Select(x => (uint)x[idx]).Distinct());

            //    File.WriteAllLines("newobjinfs" + i + ".txt", newsourceindeces.Where(checksourceindex).Select(x => x.ToString()).ToArray());
            //}
        }

        /// <summary>
        /// Was in ItemViewer() moved here to clean up.
        /// Finds all TextureRenderState and ZbufferEnable used by effects.
        /// </summary>
        internal void helperfunc1()
        {
            // Group all effectinfo's by their effect type
            var xxx = from data0 in effectinfo
                      let data = data0.Data as object[]
                      let inf = new { Header = data[0] as object[], Data = data[1] as object[] }
                      group inf by (uint)inf.Header[2];

            var xxxlist1 = new Tuple<int, int>[4][];
            var xxxlist2 = new object[0];

            for (uint i = 0; i < 4; ++i)
            {
                var formatter = EffectInfoFormatter.GetFormatter(i);

                var idx1 = formatter["TextureRenderState"];
                var idx2 = formatter["ZbufferEnable"];

                var effs = xxx.First(x => x.Key == i);

                if (idx1 != -1) xxxlist1[i] =
                        effs.Select(x => Convert.ToInt32(x.Data[idx1])).GroupBy(x => x).Select(x => new Tuple<int, int>(x.Key, x.Count())).ToArray();

                if (idx2 != -1) xxxlist2 = xxxlist2.Concat(effs.Where(x => !(bool)x.Data[idx2])).ToArray();
            }
        }

        /// <summary>
        /// Was in ItemViewer() moved here to clean up.
        /// Finds some bodyconditions and displays them apparently.
        /// </summary>
        internal void helperfunc2()
        {
            var idx = ObjectInfoFormatter.GetFormatter("effects")["EffectName"];
            var idx2 = ObjectInfoFormatter.GetFormatter("data")["BodyConditionName"];

            var x = from pair in objectinfo
                    let inf = (pair.Data as object[])?[1] as object[][]
                    where inf != null
                    from row in inf
                    let bodycond = row[0] as object[]
                    let effs = row[1] as object[][]
                    from eff in effs
                    let effname = eff[idx] as string
                    where effname.StartsWith("06400446") || effname.StartsWith("06400457") || effname.StartsWith("06400465")
                    select new { ObjectInfoName = pair.Name, BodyCondName = bodycond[idx2] as string, EffectName = effname };
            
            var form = new DataGridViewForm(x.ToArray());

            form.Show();

            //// 06400446
            //// 06400457
            //// 06400465
        }

        #endregion

        /// <summary>
        /// Instances of this class carry information about the tree view node.
        /// </summary>
        class treeViewTag
        {
            public byte Operation { get; set; }
            public Formatter.Struct Formatter { get; set; }
            public object Data { get; set; }
            public Action<TreeNode> ShowMenu { get; set; }

            public treeViewTag(byte op, Formatter.Struct formatter, object data, Action<TreeNode> onrightclick)
            {
                Operation = op;
                Formatter = formatter;
                Data = data;
                ShowMenu = onrightclick;
            }
        }

        void treeView_AddOmi()
        {
            var node = new TreeNode("Omi");

            for (int i = 0; i < FormatterOmi.Formatters.Length; i++)
            {
                var child = new TreeNode(FormatterOmi.Formatters[i].Name);

                child.Tag = new treeViewTag(0, FormatterOmi.Formatters[i], omi[i], null);

                node.Nodes.Add(child);
            }

            treeView.Nodes.Add(node);
        }

        void treeView_AddObjectInfo()
        {
            var node = new TreeNode("ObjectInfo [" + objectinfo.Count() + "]");

            // next line does not show useful information and is slow
            // panoskj disabled it
            // node.Tag = new object[] { 0, ObjectInfoFormatter.GetFormatter("header"), objectinfo.Select(x => x.Data as object[]).Where(x => x != null) };

            foreach (var inf in objectinfo)
            {
                var exists = File.Exists(@"res-obj\" + inf.Name + ".obj");

                var child = exists ? new TreeNode(inf.Name) : new TreeNode(inf.Name + " (without file)");


                child.Tag = new treeViewTag(4, ObjectInfoFormatter.GetFormatter("data"), inf, null);

                node.Nodes.Add(child);
            }

            treeView.Nodes.Add(node);
        }

        void treeView_AddEffectInfo()
        {
            var node = new TreeNode("EffectInfo [" + effectinfo.Count() + "]");

            Action<TreeNode> onrightclick = selectednode =>
            {
                var menu = new ContextMenuStrip();

                var menuedit = new ToolStripMenuItem("Save");

                menuedit.Click += (sender, e) =>
                {
                    dataGridView.EndEdit();

                    var inf = (CachedInfo)(selectednode.Tag as treeViewTag).Data;

                    var data = inf.Data as object[];

                    var newdata = (dataGridView.DataSource as DataTable).Rows.OfType<DataRow>().SingleOrDefault();

                    if (newdata != null)
                    {
                        data[1] = newdata.ItemArray;
                        inf.Update(data);
                    }
                };

                menu.Items.Add(menuedit);

                menu.Show(MousePosition);
            };

            foreach (var inf in effectinfo)
            {
                var child = new TreeNode(inf.Name);

                child.Tag = new treeViewTag(5, null, inf, onrightclick);

                node.Nodes.Add(child);
            }

            treeView.Nodes.Add(node);
        }

        void treeView_AddMapInfo()
        {
            var node = new TreeNode("Maps");

            foreach (var inf in mapinfo)
            {
                var child = new TreeNode(inf.Name);

                child.Tag = new treeViewTag(3, null, inf, null);

                node.Nodes.Add(child);
            }

            treeView.Nodes.Add(node);
        }
        

        private void treeView_Init()
        {
            treeView_AddOmi();
            treeView_AddObjectInfo();
            treeView_AddEffectInfo();
            treeView_AddMapInfo();
        }
        


        private void treeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            var node = e.Node;
            var tag = node?.Tag as treeViewTag;

            if (tag == null) return;


            var type = (int)tag.Operation;
            var format = tag.Formatter;
            var data = tag.Data;

            dataGridView.ReadOnly = true;

            if (type == 0)

                dataGridView.DataSource = format.GetSchema(data as IEnumerable<object[]>);

            if (type == 1 && node.Nodes.Count == 0)
            {
                foreach (var group in data as IEnumerable<IGrouping<object, object[]>>)
                {
                    var child = new TreeNode(group.Key.ToString() + " [" + group.Count() + "]");

                    child.Tag = new object[] { 2, null, group };

                    node.Nodes.Add(child);
                }
            }

            if (type == 2)

                dataGridView.DataSource = ((node.Parent.Parent.Tag as object[])[1] as Formatter.Struct).GetSchema(data as IEnumerable<object[]>);


            if (type == 3 && node.Nodes.Count == 0)
            {
                var mapinf = ((CachedInfo)data).Data as object[];

                if (mapinf == null) node.Name = data as string + " [Invalid]";

                else
                    for (int i = 1; i < 4; ++i)
                    {
                        var formatter = MapFormatter.GetFormatter(i);

                        var child = new TreeNode(formatter.Name  + " [" + (mapinf[i] as object[]).Length + "]");

                        child.Tag = new treeViewTag(0, formatter, mapinf[i], null);

                        node.Nodes.Add(child);

                    }
            }

            if (type == 4 && data != null)
            {
                var objinfname = ((CachedInfo)data).Name;
                var objinf0 = ((CachedInfo)data).Data as object[];

                if (objinf0 == null) return;

                var objinf = (objinf0[1] as IEnumerable<object>).Select(x => x as object[]).Select(x => new { data = x[0] as object[], effects = x[1] as object[][] });

                if (node.Nodes.Count == 0)
                {
                    node.Nodes.Add("Name: " + (objinf0[0] as object[])[0] as string);

                    var idx1 = FormatterOmi.GetFormatter("ti_Item")["ItemNum"];
                    var idx2 = FormatterOmi.GetFormatter("ti_Item")["Kind"];
                    var idx3 = FormatterOmi.GetFormatter("ti_Item")["ItemName"];
                    var idx4 = FormatterOmi.GetFormatter("ti_Item")["SourceIndex"];
                    
                    int sourceindex;

                    if (int.TryParse(objinfname, out sourceindex))
                    {
                        var usages = from item in omi[0] as object[][]
                                     where (uint)item[idx4] == sourceindex
                                     || (((byte)item[idx2] == 16 || (byte)item[idx2] == 60) && (uint)item[idx4] == (sourceindex - 4000000))
                                     || (((byte)item[idx2] < 16) && (uint)item[idx4] == (sourceindex - 100000))
                                     || (((byte)item[idx2] == 25) && (uint)item[idx4] == (sourceindex - 500000))
                                     select new { ItemNum = (uint)item[idx1], ItemName = item[idx3] as string };

                        var usagesnode = new TreeNode("Used by items [" + usages.Count() + "]");

                        foreach (var usage in usages)
                        {
                            var child = new TreeNode("[" + usage.ItemNum + "] " + usage.ItemName);

                            child.Tag = new treeViewTag(0, FormatterOmi.GetFormatter("ti_Item"), (omi[0] as object[][]).Where(x => (uint)x[idx1] == usage.ItemNum), null);

                            usagesnode.Nodes.Add(child);
                        }
                        node.Nodes.Add(usagesnode);
                    }

                    var effectsnode = new TreeNode("Body Condition Effects");

                    var idx = format["BodyConditionName"];

                    foreach (var row in objinf)
                    {
                        var child = new TreeNode(row.data[idx] as string + " [" + row.effects.Length + "]");

                        child.Tag = new treeViewTag(0, ObjectInfoFormatter.GetFormatter("effects"), row.effects, null);

                        effectsnode.Nodes.Add(child);
                    }

                    node.Nodes.Add(effectsnode);
                }

                dataGridView.DataSource = format.GetSchema(objinf.Select(x => x.data));
            }

            if (type == 5 && data != null)
            {
                dataGridView.ReadOnly = false;

                var temp = ((CachedInfo)data).Data as object[];

                var idx = EffectInfoFormatter.HeaderFormatter["EffectType"];

                var efftype = (uint)(temp[0] as object[])[idx];

                var formatter = EffectInfoFormatter.GetFormatter(efftype);

                dataGridView.DataSource = formatter.GetSchema(
                    new object[][] { temp[1] as object[] });
            }
        }

        //EffectViewEx effectview = null;

        private void dataGridView_CellMouseDoubleClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            //if (effectview == null || effectview.IsDisposed) effectview = new EffectViewEx(dataGridView);

            //effectview.UpdateData();

            //if (!effectview.Visible) effectview.Show(this);
        }


        /// <summary>
        /// When user right-clicks a node it gets selected
        /// and the tagged menu handler gets invoked.
        /// </summary>
        private void treeView_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                treeView.SelectedNode = e.Node;
                (treeView.SelectedNode?.Tag as treeViewTag)?.ShowMenu?.Invoke(treeView.SelectedNode);
            }
        }
    }
}
