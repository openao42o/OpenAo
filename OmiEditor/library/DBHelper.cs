using System;
using System.IO;
using System.Linq;
using System.Data;
using System.Data.SqlClient;
using System.Collections.Generic;

using Formatter;
using Formatter.Omi;
using Library;

namespace DBHelper
{
    public class DBstuff
    {
        public static void UpdateDB(IEnumerable<object[]> data, object[][] items)
        {
            using (var conn = new SqlConnection("Server=127.0.0.1,1433;User ID=atum; Database=atum2_db_account; Password=DB:atum"))
            {
                conn.Open();

                var formatter = FormatterOmi.Formatters[0];

                var idx = formatter["Disabled"];
                var idxnum = formatter["ItemNum"];

                var updatecmdtext = @"UPDATE dbo.ti_ItemInfo SET ";

                for (int i = 0; i < formatter.Fields.Length; ++i)

                    if (i != idxnum && i != idx) updatecmdtext += formatter.Fields[i].Name  + " = @" + i.ToString() + ",";

                updatecmdtext = updatecmdtext.Substring(0, updatecmdtext.Length - 1) + " WHERE ItemNum = @" + idxnum.ToString() + ";";

                using (var cmd = new SqlCommand(updatecmdtext, conn))
                {
                    for (int i = 0; i < formatter.Fields.Length; ++i)

                        if (i != idx) cmd.Parameters.Add("@" + i.ToString(), typeMap[formatter.Fields[i].DataType]);

                    foreach (var row in data)
                    {
                        for (int i = 0; i < formatter.Fields.Length; ++i)

                            if (i != idx)
                            {
                                if (row[i] is bool) cmd.Parameters["@" + i.ToString()].Value = ((bool)row[i] ? 1 : 0);
                                else if (row[i] is float) cmd.Parameters["@" + i.ToString()].Value = Math.Round((float)row[i], 3);
                                else if (row[i] is double) cmd.Parameters["@" + i.ToString()].Value = Math.Round((double)row[i], 8);
                                else cmd.Parameters["@" + i.ToString()].Value = row[i];
                            }

                        cmd.ExecuteNonQuery();

                        var xxx = items.FirstIndexOrNull(x => (uint)x[idxnum] == (uint)row[idxnum]);

                        if (xxx.HasValue) items[xxx.Value] = row;
                    }
                }

                //foreach (var pair in data)
                //{
                //    if (pair.Value.Count == 0) continue;

                //    var fields = OmiFormatter.ItemFormat.Where(x => x.Name == pair.Key);

                //    if (fields.Count() != 1) continue;

                //    var field = fields.First();

                //    var colname = field.Name == "Disabled" ? "Enabled" : field.Name;
                //    var colidx = OmiFormatter.IndexOf(field.Name);

                //    using (var cmd = new SqlCommand("update ti_ItemInfo set " + colname + " = @val where ItemNum = @num", conn))
                //    {
                //        cmd.Parameters.Add("@val", typeMap[field.DataType]);
                //        cmd.Parameters.Add("@num", SqlDbType.Int);

                //        if (field is StringField) cmd.Parameters[0].Size = (int)(field as StringField).Length;

                //        cmd.Prepare();

                //        if (field.Name == "Disabled") foreach (var pair2 in pair.Value)

                //            {
                //                cmd.Parameters[0].Value = (bool)pair2.Value ? 0 : 1;
                //                cmd.Parameters[1].Value = pair2.Key;

                //                cmd.ExecuteNonQuery();
                //            }

                //        else foreach (var pair2 in pair.Value)

                //            {
                //                cmd.Parameters[0].Value = pair2.Value;
                //                cmd.Parameters[1].Value = pair2.Key;

                //                cmd.ExecuteNonQuery();

                //                foreach (var row in items.Where(x => (uint)x[itemnumidx] == pair2.Key)) row[colidx] = pair2.Value;
                //            }
                //    }
                //}
            }

            //byte[] newdata;

            //using (var dest = new MemoryStream())
            //{
            //    OmiFormatter.Write(dest, currentomi);
            //
            //    newdata = dest.ToArray();
            //}

            //byte[] olddata = File.ReadAllBytes(@"versioning\current\Res-Tex\omi.tex");

            //Array.ConstrainedCopy(newdata, 0, olddata, 0, newdata.Length);

            //PATCHER.AddSingleFile("Res-Tex", "omi.tex", olddata);

            //PATCHER.RestartALL();
        }

        public static void InsertDB(Struct formatter, IEnumerable<object[]> data)
        {
            using (var conn = new SqlConnection("Server=127.0.0.1,1433;User ID=atum; Database=test; Password=DB:atum"))
            {
                conn.Open();

                var tablename = "dbo.new_" + formatter.Name;

                var dropcmdtext = @"IF OBJECT_ID('" + tablename + "', 'U') IS NOT NULL DROP TABLE " + tablename + ";";

                using (var cmd = new SqlCommand(dropcmdtext, conn)) cmd.ExecuteNonQuery();

                var createcmdtext = @"CREATE TABLE " + tablename + " (";

                foreach (var col in formatter.Fields)
                {
                    if (col.DataType != typeof(string))

                        createcmdtext += col.Name + " " + typeMap[col.DataType].ToString() + ",";

                    else createcmdtext += col.Name + " " + typeMap[col.DataType].ToString() + "(" + col.Length + "),";
                }

                createcmdtext += ");";

                using (var cmd = new SqlCommand(createcmdtext, conn)) cmd.ExecuteNonQuery();

                var insertcmdtext = @"INSERT INTO " + tablename + " VALUES (";

                for (int i = 0; i < formatter.Fields.Length; ++i)

                    insertcmdtext += "@" + i.ToString() + ",";

                insertcmdtext = insertcmdtext.Substring(0, insertcmdtext.Length - 1) + ")";

                using (var cmd = new SqlCommand(insertcmdtext, conn))
                {
                    for (int i = 0; i < formatter.Fields.Length; ++i)

                        if (formatter.Fields[i].DataType != typeof(string))

                            cmd.Parameters.Add("@" + i.ToString(), typeMap[formatter.Fields[i].DataType]);

                        else cmd.Parameters.Add("@" + i.ToString(), typeMap[formatter.Fields[i].DataType], (int)formatter.Fields[i].Length);

                    cmd.Prepare();

                    foreach (var row in data)
                    {
                        for (int i = 0; i < formatter.Fields.Length; ++i)
                        {
                            if (row[i] is bool) cmd.Parameters[i].Value = (bool)row[i] ? 1 : 0;
                            else if (row[i] is float) cmd.Parameters[i].Value = Math.Round((float)row[i], 3);
                            else if (row[i] is double) cmd.Parameters[i].Value = Math.Round((double)row[i], 6);
                            else cmd.Parameters[i].Value = row[i];
                        }

                        cmd.ExecuteNonQuery();
                    }
                }
            }
        }

        /// <summary>
        /// Maps C# types to SqlDbType
        /// </summary>
        static Dictionary<Type, SqlDbType> typeMap { get; }

        static DBstuff()
        {
            typeMap = new Dictionary<Type, SqlDbType>();

            typeMap[typeof(string)] = SqlDbType.VarChar;

            typeMap[typeof(ulong)] = SqlDbType.BigInt;
            typeMap[typeof(long)] = SqlDbType.BigInt;

            typeMap[typeof(uint)] = SqlDbType.Int;
            typeMap[typeof(int)] = SqlDbType.Int;

            typeMap[typeof(ushort)] = SqlDbType.Int; //sql doesnt have unsigned short
            typeMap[typeof(short)] = SqlDbType.SmallInt;

            typeMap[typeof(byte)] = SqlDbType.TinyInt;

            typeMap[typeof(bool)] = SqlDbType.Bit;

            typeMap[typeof(float)] = SqlDbType.Float;
            typeMap[typeof(double)] = SqlDbType.Float;
        }
    }
}
