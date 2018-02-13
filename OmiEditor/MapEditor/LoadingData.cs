using System.IO;
using System.Windows.Forms;
using System.Collections.Generic;
using System;
using System.Data;
using System.Linq;
using Library;
using System.Text.RegularExpressions;

class LoadingData
{
    public static string directory = @"..\..\..\map\";
    private static readonly string ConfigFolder = @"..\..\..\Config\";
    public static IEnumerable<Tuple<int, int, int>> MapCfg_data;
    public static List<FileInfo> Dat_List = new List<FileInfo>();

    public static void GetListSma(TreeView tv)
    {
        tv.Nodes.Clear();
        LoadConfigSma();     
        if (Directory.Exists(directory))
        {
            foreach (Tuple<int, int, int> ab in MapCfg_data)
            {
                TreeNode Header = new TreeNode("header");
                TreeNode TileInfo = new TreeNode("tileinfo");
                TreeNode MonsterInfo = new TreeNode("monsterinfo");

                TreeNode node = new TreeNode(ab.Item1 + ".sma", new TreeNode[] { Header, TileInfo, MonsterInfo });
                //selection mapindex xy 
                Header.Tag = new Tuple<string, int, int>("header", ab.Item1, ab.Item2);
                TileInfo.Tag = new Tuple<string, int, int>("tileinfo", ab.Item1, ab.Item2);
                MonsterInfo.Tag = new Tuple<string, int, int>("monsterinfo", ab.Item1, ab.Item2);

                node.Tag = new Tuple<string, int, int>(ab.Item1.ToString(), -1, ab.Item2);

                tv.Nodes.Add(node);

            }
           

        }
        else
        {
            MessageBox.Show("cannot find path to Map folder! \n " + directory);
        }
    }



    // first read all lines
    // then TAKE each line and SELECT from it line.Split('#')[0] so we keep the part before the first #
    // keep only lines where line.Length > 0
    // now, on the thing we SELECTED before (line.Split('#')[0]), do a Split(' ', '\t')
    // this might return something like "3015", "", "", "FALSE" ...
    // so from this list of tokens we keep only those where token.Length > 0
    // and we convert that list of tokens (without the empty ones) into an array
    // and this array is what we select as element of our "list" alltokens
    // see alltokens is IEnumerable<string[]>
    public static IEnumerable<Tuple<int, int, int>> GetMapConfig()
    {
       
        if (!File.Exists(ConfigFolder + @"map.cfg"))
        {
            MessageBox.Show("cannot find path to MapConfig folder! \n " + ConfigFolder + @"map.cfg");
        }
        var alltokens = from line in File.ReadAllLines(ConfigFolder + @"map.cfg").Select(x => x.Split('#')[0])
                        where line.Length > 0
                        select line.Split(' ', '\t').Where(token => token.Length > 0).ToArray();
        foreach (var tokens in alltokens)
        {
            if (tokens.Length == 0) continue;
            if (tokens[0] == "ServerGroupDefaultMap") continue;
            if (tokens.Length != 12) throw new ArgumentException("Bad file format");

            yield return new Tuple<int, int, int>(int.Parse(tokens[0]), int.Parse(tokens[9]), int.Parse(tokens[10]));

        }
    }

    private static void LoadConfigSma()
    {
        MapCfg_data = GetMapConfig();
        MapCfg_data.ToDictionary(i => i.Item1, i => i.Item2);
    }


    private static void GetDatFileNames()
    {
        DirectoryInfo dinfo = new DirectoryInfo(directory);
      
        FileInfo[] fiFiles = dinfo.GetFiles("*.dat");

        foreach (FileInfo fi in fiFiles)
        {
            Dat_List.Add(fi);
        }

    }

    public static void GetListDat(TreeView tv)
    {
        GetDatFileNames();
        tv.Nodes.Clear();
     
        foreach(FileInfo name in Dat_List)
        {
            TreeNode Mapdata = new TreeNode("mapdata");
            TreeNode Vertexinfo = new TreeNode("vertexinfo");
            TreeNode TileInfo = new TreeNode("tileinfo");
            TreeNode Objectinfo = new TreeNode("objectinfo");
            TreeNode node = new TreeNode(name.Name, new TreeNode[] { Mapdata, Vertexinfo, TileInfo, Objectinfo });

            //try catch is there just because if we want to search for one sma's xy and its not in MapCfg_data 
            try
            {
                Mapdata.Tag = new Tuple<string, string, int>(name.Name, "mapdata", MapCfg_data.Where(x => x.Item1 == int.Parse(Path.GetFileNameWithoutExtension(name.Name))).Select(y => y.Item2).ElementAt(0));
                Vertexinfo.Tag = new Tuple<string, string, int>(name.Name, "vertexinfo", MapCfg_data.Where(x => x.Item1 == int.Parse(Path.GetFileNameWithoutExtension(name.Name))).Select(y => y.Item2).ElementAt(0));
                TileInfo.Tag = new Tuple<string, string, int>(name.Name, "tileinfo", MapCfg_data.Where(x => x.Item1 == int.Parse(Path.GetFileNameWithoutExtension(name.Name))).Select(y => y.Item2).ElementAt(0));
                Objectinfo.Tag = new Tuple<string, string, int>(name.Name, "objectinfo", MapCfg_data.Where(x => x.Item1 == int.Parse(Path.GetFileNameWithoutExtension(name.Name))).Select(y => y.Item2).ElementAt(0));
                node.Tag = new Tuple<string, string, int>(name.Name,"base", MapCfg_data.Where(x => x.Item1 == int.Parse(Path.GetFileNameWithoutExtension(name.Name))).Select(y => y.Item2).ElementAt(0));
            }

            catch (ArgumentOutOfRangeException)
            {
                Mapdata.Tag = new Tuple<string, string, int>(name.Name, "mapdata", 0);
                Vertexinfo.Tag = new Tuple<string, string, int>(name.Name, "vertexinfo", 0);
                TileInfo.Tag = new Tuple<string, string, int>(name.Name, "tileinfo", 0);
                Objectinfo.Tag = new Tuple<string, string, int>(name.Name, "objectinfo", 0);
                node.Tag = new Tuple<string, string, int>(name.Name,"base", 0);
            }
            tv.Nodes.Add(node);
        }         
    }
}