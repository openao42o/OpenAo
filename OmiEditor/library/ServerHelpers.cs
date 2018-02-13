using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;

using Formatter.Sma;
using Formatter.Dat;

namespace Library
{
    public class ServerHelpers
    {
        public static IEnumerable<Tuple<int, int, int>> GetMapConfig(string filename)
        {
            // first read all lines
            // then TAKE each line and SELECT from it line.Split('#')[0] so we keep the part before the first #
            // keep only lines where line.Length > 0
            // now, on the thing we SELECTED before (line.Split('#')[0]), do a Split(' ', '\t')
            // this might return something like "3015", "", "", "FALSE" ...
            // so from this list of tokens we keep only those where token.Length > 0
            // and we convert that list of tokens (without the empty ones) into an array
            // and this array is what we select as element of our "list" alltokens
            // see alltokens is IEnumerable<string[]>
            var alltokens = from line in File.ReadAllLines(filename).Select(x => x.Split('#')[0])
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


        /// <summary>
        /// Generates an array of map ideces and monster unique numbers
        /// from dat and sma files. 2017-01-05 panoskj
        /// </summary>
        public static object[] SomeQuery()
        {

            var dats = Directory.GetFiles(@"..\..\..\map", "*.dat");
            var smas = Directory.GetFiles(@"..\..\..\map", "*.sma");

            // data from map config
            var mapcfg = GetMapConfig(@"..\..\..\config\map.cfg").
                ToDictionary(x => x.Item1, x => new Tuple<int, int>(x.Item2, x.Item3));

            // reads a dat file
            Func<string, Tuple<int, object>> readDat = (s) =>
            {
                int idx = int.Parse(Path.GetFileNameWithoutExtension(s));

                using (AtumZip.ZippedFile zpf = File.OpenRead(s))

                    return new Tuple<int, object>(idx, MapFormatter.Read(zpf.At(0)));
            };

            // reads a sma file
            Func<string, Tuple<int, object>> readSma = (s) =>
            {
                int idx = int.Parse(Path.GetFileNameWithoutExtension(s));

                using (var src = File.OpenRead(s))

                    if (mapcfg.ContainsKey(idx) && mapcfg[idx].Item1 == mapcfg[idx].Item2)

                        return new Tuple<int, object>(idx,
                            SmaFormatter.Read(src, (uint)mapcfg[idx].Item1));

                    else return null;
            };

            // collects all tables containing spawn information
            var datmondata = dats.Select(x => readDat(x)).Where(x => x != null).
                Select(x => new { Index = x.Item1, Data = (x.Item2 as object[])[3] as object[][] });

            // collects all tables containing spawn information
            var smamondata = smas.Select(x => readSma(x)).Where(x => x != null).
                Select(x => new { Index = x.Item1, Data = (x.Item2 as object[])[2] as object[][] });

            // indeces for later use 
            var idx1 = MapFormatter.GetIndex("objectinfo", "ObjectMonsterUniqueNumber");
            var idx2 = SmaFormatter.GetIndex("monsterinfo", "Monster Num");

            // from each object spawn row of each collected table
            // select the map index and ObjectMonsterUniqueNumber
            // if ObjectMonsterUniqueNumber is in range [2000000, 3000000)
            var res1 = from data in datmondata
                       from spawn in data.Data
                       where (uint)spawn[idx1] >= 2000000 && (uint)spawn[idx1] < 3000000 // or check for EventType == 6
                       select new
                       {
                           Index = data.Index,
                           Monster = (uint)spawn[idx1]
                       };

            // from each monster spawn of each collected table
            // select the map index and Monster Num
            // if Monster Num is not 0
            var res2 = from data in smamondata
                       from spawn in data.Data
                       where (uint)spawn[idx2] != 0
                       select new
                       {
                           Index = data.Index,
                           Monster = (uint)spawn[idx2]
                       };

            // return the distinct results
            return res1.Concat(res2).Distinct().ToArray();
        }
    }
}
