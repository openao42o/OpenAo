using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Data;
using System.Globalization;
using System.ComponentModel;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;
using System.Reflection;

namespace Library
{
    public class DataRowEqualityComaparer : IEqualityComparer<DataRow>
    {
        private readonly int[] impCols;

        public int[] ImportantFields { get { return impCols; } }

        public DataRowEqualityComaparer(params int[] SignificantColumns)
        {
            impCols = SignificantColumns;
        }

        public bool Equals(DataRow x, DataRow y)
        {
            foreach (int col in impCols) if (!x[col].Equals(y[col])) return false;
            return true;
        }

        public int GetHashCode(DataRow dr)
        {
            int hash = 0;
            foreach (int col in impCols) hash ^= dr[col].GetHashCode();
            return hash;
        }
    }

    public static class BitConverter<T>
    {
        private static readonly Type _Type;
        private static readonly int? _Size;

        private static readonly Encoding _Encoding;
        private static readonly CultureInfo _Culture;

        private static Action<byte[], object, int, int> _GetBytes;
        private static Func<byte[], int, int, object> _GetValue;
        private static Func<string, object> _Parse;

        unsafe static BitConverter()
        {
            _Type = typeof(T);

            _Encoding = Encoding.GetEncoding("EUC-KR"); // Encoding.GetEncoding("EUC-KR");
            _Culture = new CultureInfo("en-US");

            if (_Type == typeof(bool)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(bool*)(ptr + idx) = (bool)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToBoolean(arr, idx); _Parse = (s) => bool.Parse(s); _Size = sizeof(bool); }
            else if (_Type == typeof(char)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(char*)(ptr + idx) = (char)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToChar(arr, idx); _Parse = (s) => char.Parse(s); _Size = sizeof(char); }
            else if (_Type == typeof(byte)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(ptr + idx) = (byte)val; } }; _GetValue = (arr, idx, len) => arr[idx]; _Parse = (s) => byte.Parse(s, _Culture); _Size = sizeof(byte); }
            else if (_Type == typeof(sbyte)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(sbyte*)(ptr + idx) = (sbyte)val; } }; _GetValue = (arr, idx, len) => (sbyte)arr[idx]; _Parse = (s) => sbyte.Parse(s, _Culture); _Size = sizeof(sbyte); }
            else if (_Type == typeof(short)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(short*)(ptr + idx) = (short)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToInt16(arr, idx); _Parse = (s) => short.Parse(s, _Culture); _Size = sizeof(short); }
            else if (_Type == typeof(ushort)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(ushort*)(ptr + idx) = (ushort)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToUInt16(arr, idx); _Parse = (s) => ushort.Parse(s, _Culture); _Size = sizeof(ushort); }
            else if (_Type == typeof(int)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(int*)(ptr + idx) = (int)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToInt32(arr, idx); _Parse = (s) => int.Parse(s, _Culture); _Size = sizeof(int); }
            else if (_Type == typeof(uint)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(uint*)(ptr + idx) = (uint)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToUInt32(arr, idx); _Parse = (s) => uint.Parse(s, _Culture); _Size = sizeof(uint); }
            else if (_Type == typeof(long)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(long*)(ptr + idx) = (long)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToInt64(arr, idx); _Parse = (s) => long.Parse(s, _Culture); _Size = sizeof(long); }
            else if (_Type == typeof(ulong)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(ulong*)(ptr + idx) = (ulong)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToUInt64(arr, idx); _Parse = (s) => ulong.Parse(s, _Culture); _Size = sizeof(ulong); }
            else if (_Type == typeof(float)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(float*)(ptr + idx) = (float)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToSingle(arr, idx); _Parse = (s) => float.Parse(s, _Culture); _Size = sizeof(float); }
            else if (_Type == typeof(double)) { _GetBytes = (arr, val, idx, len) => { fixed (byte* ptr = arr) { *(double*)(ptr + idx) = (double)val; } }; _GetValue = (arr, idx, len) => BitConverter.ToDouble(arr, idx); _Parse = (s) => double.Parse(s, _Culture); _Size = sizeof(double); }
            else if (_Type == typeof(string)) { _GetBytes = (arr, val, idx, len) => TranslateToBytes(out arr, (string)val, idx, len); _GetValue = (arr, idx, len) => TranslateToString(arr, idx,  len); _Parse = (s) => s; _Size = null; }
            else throw new ArgumentException("Converter<" + _Type.ToString() + "> cannot be instatiated.", "T");
        }

        public static void TranslateToBytes(out byte[] output, string input, int idx, int len)
        {
            byte[] buffer = input.SelectMany((c) => BitConverter.GetBytes(c)).ToArray();
            output = Encoding.Convert(Encoding.Default, _Encoding, buffer, idx, len);
        }
        public static string TranslateToString(byte[] input, int idx, int len)
        {
            StringBuilder r = new StringBuilder();
            byte[] temp = Encoding.Convert(_Encoding, Encoding.Unicode, input, idx, len);
            for (int i = 0; i < temp.Length / 2; i++)
            {
                char c = BitConverter.ToChar(temp, 2 * i);
                if (c == '\0') break;
                r.Append(c);
            }
            return r.ToString();
        }

        public static void GetBytes(byte[] Buffer, object Value, int Index, int Size)
        {
            AttemptThrowA(Size);
            _GetBytes(Buffer, Value, Index, Size);
        }
        public static void GetBytes(byte[] Buffer, object Value, int Index)
        {
            _GetBytes(Buffer, Value, Index, AttemptThrowC());
        }
        public static byte[] GetBytes(object Value)
        {
            byte[] temp = new byte[AttemptThrowC()];
            _GetBytes(temp, Value, 0, temp.Length);
            return temp;
        }

        public static T GetValue(byte[] Buffer, int Index, int Size)
        {
            AttemptThrowB(Size);
            return (T)_GetValue(Buffer, Index, Size);
        }
        public static T GetValue(byte[] Buffer, int Index)
        {
            return (T)_GetValue(Buffer, Index, AttemptThrowC());
        }
        public static T GetValue(byte[] Buffer)
        {
            return (T)_GetValue(Buffer, 0, AttemptThrowC());
        }

        public static object Parse(string Value)
        {
            return _Parse(Value);
        }

        public static Type Type { get { return _Type; } }
        public static int? Size { get { return _Size; } }

        internal static Action<byte[], object, int, int> GetBytesDelegate { get { return _GetBytes; } set { _GetBytes = value; } }
        internal static Func<byte[], int, int, object> GetValueDelegate { get { return _GetValue; } set { _GetValue = value; } }
        internal static Func<string, object> ParseDelegate { get { return _Parse; } set { _Parse = value; } }

        internal static int AttemptThrowA(int Size)
        {
            if ((_Size ?? 0) > Size) throw new ArgumentException("Cannot convert " + _Type.ToString() + " to " + Size + " bytes only.", "Count");
            return Size;
        }
        internal static int AttemptThrowB(int Size)
        {
            if ((_Size ?? 0) > Size) throw new ArgumentException("Cannot convert " + Size + " bytes only" + " to " + _Type.ToString() + ".", "Count");
            return Size;
        }
        internal static int AttemptThrowC()
        {
            if (!_Size.HasValue) throw new InvalidOperationException("Could not retrieve default size for " + _Type.ToString() + ".");
            return _Size.Value;
        }
    }

    public static class Extensions
    {
        public static void Write<T>(this Stream Stream, object Value, int? Size)
        {
            int size = Size != null ? BitConverter<T>.AttemptThrowA(Size.Value) : BitConverter<T>.AttemptThrowC();

            byte[] temp = new byte[size];
            BitConverter<T>.GetBytesDelegate(temp, Value, 0, size);
            Stream.Write(temp, 0, size);
        }
        public static void Write<T>(this Stream Stream, T Value, int? Size)
        {
            Write<T>(Stream, (object)Value, Size);
        }
        public static void Write<T>(this Stream Stream, object Value)
        {
            Write<T>(Stream, Value, null);
        }
        public static void Write<T>(this Stream Stream, T Value)
        {
            Write<T>(Stream, (object)Value);
        }

        public static T Read<T>(this Stream Stream, int? Size)
        {
            int size = Size != null ? BitConverter<T>.AttemptThrowB(Size.Value) : BitConverter<T>.AttemptThrowC();

            byte[] temp = new byte[size];
            Stream.Read(temp, 0, size);
            return (T)BitConverter<T>.GetValueDelegate(temp, 0, size);
        }
        public static T Read<T>(this Stream Stream)
        {
            return Read<T>(Stream, null);
        }

        public static byte[] ReadBytes(this Stream Stream, int Count)
        {
            byte[] temp = new byte[Count];
            Stream.Read(temp, 0, Count);
            return temp;
        }
        public static void WriteBytes(this Stream Stream, byte[] Bytes)
        {
            Stream.Write(Bytes, 0, Bytes.Length);
        }

        /// <summary>
        /// Same as foreach; side-effect: evaluates the collection.
        /// </summary>
        public static IEnumerable<T> Foreach<T>(this IEnumerable<T> collection, Action<T> action)
        {
            foreach (T item in collection) action(item);
            return collection;
        }
        public static int? FirstIndexOrNull<T>(this IEnumerable<T> collection, Func<T, bool> predicate)
        {
            int i = 0;
            foreach (T item in collection) if (predicate(item)) return i; else i++;
            return null;
        }
        public static IEnumerable<int?> MapIndeces<TOuter, TInner, TKey>(this IEnumerable<TOuter> outer, IEnumerable<TInner> inner, Func<TOuter, TKey> outerKeySelector, Func<TInner, TKey> innerKeySelector)
        {
            foreach (TOuter outerObj in outer) yield return inner.FirstIndexOrNull(innerObj => innerKeySelector(innerObj).Equals(outerKeySelector(outerObj)));
        }

        public static IEnumerable<IGrouping<TKey, TSource>> GroupAndOrderBy<TKey, TSource>(this IEnumerable<TSource> collection, Func<TSource, TKey> keySelector)
        {
            return collection.GroupBy(keySelector).OrderBy(x => x.Key);
        }

        public static IEnumerable<TResult> FullOuterGroupJoin<TA, TB, TKey, TResult>(
        this IEnumerable<TA> a,
        IEnumerable<TB> b,
        Func<TA, TKey> selectKeyA,
        Func<TB, TKey> selectKeyB,
        Func<IEnumerable<TA>, IEnumerable<TB>, TKey, TResult> projection,
        IEqualityComparer<TKey> cmp = null)
        {
            cmp = cmp ?? EqualityComparer<TKey>.Default;
            var alookup = a.ToLookup(selectKeyA, cmp);
            var blookup = b.ToLookup(selectKeyB, cmp);

            var keys = new HashSet<TKey>(alookup.Select(p => p.Key), cmp);
            keys.UnionWith(blookup.Select(p => p.Key));

            var join = from key in keys
                       let xa = alookup[key]
                       let xb = blookup[key]
                       select projection(xa, xb, key);

            return join;
        }


        public static IEnumerable<TResult> FullOuterJoin<TA, TB, TKey, TResult>(
            this IEnumerable<TA> a,
            IEnumerable<TB> b,
            Func<TA, TKey> selectKeyA,
            Func<TB, TKey> selectKeyB,
            Func<TA, TB, TKey, TResult> projection,
            TA defaultA = default(TA),
            TB defaultB = default(TB),
            IEqualityComparer<TKey> cmp = null)
        {
            cmp = cmp ?? EqualityComparer<TKey>.Default;

            var alookup = a.ToLookup(selectKeyA, cmp);
            var blookup = b.ToLookup(selectKeyB, cmp);

            var keys = new HashSet<TKey>(alookup.Select(p => p.Key), cmp);
            keys.UnionWith(blookup.Select(p => p.Key));

            var join = from key in keys
                       from xa in alookup[key].DefaultIfEmpty(defaultA)
                       from xb in blookup[key].DefaultIfEmpty(defaultB)
                       select projection(xa, xb, key);

            return join;
        }


        /// <summary>
        /// Turns on double buffering to the specified DataGridView control.
        /// </summary>
        /// <param name="dgv">The specified DataGridView control.</param>
        /// <param name="setting">true to enable, false to disable.</param>
        public static void DoubleBuffered(this DataGridView dgv, bool setting)
        {
            Type dgvType = dgv.GetType();
            PropertyInfo pi = dgvType.GetProperty("DoubleBuffered",
                  BindingFlags.Instance | BindingFlags.NonPublic);
            pi.SetValue(dgv, setting, null);
        }
        public static void DoubleBuffered(this Panel dgv, bool setting)
        {
            Type dgvType = dgv.GetType();
            PropertyInfo pi = dgvType.GetProperty("DoubleBuffered",
                  BindingFlags.Instance | BindingFlags.NonPublic);
            pi.SetValue(dgv, setting, null);
        }


    }

    /// <summary>
    /// Old helper class.
    /// </summary>
    public static class Tools
    {
        #region "converters"
        public static bool UseKoreanLocalization = false;

        private static Encoding encoding = Encoding.GetEncoding("EUC-KR");

        public static byte[] TranslateToBytes(string input)
        {
            return encoding.GetBytes(input);
        }
        public static string TranslateToString(byte[] input)
        {
            StringBuilder r = new StringBuilder();
            byte[] temp = Encoding.Convert(encoding, Encoding.Unicode, input);
            for (int i = 0; i < temp.Length / 2; i++)
            {
                char c = BitConverter.ToChar(temp, 2 * i);
                if (c == '\0') break;
                r.Append(c);
            }
            return r.ToString();
        }

        public static void PutBytes(byte[] destination, int index, short value)
        {
            Array.ConstrainedCopy(BitConverter.GetBytes(value), 0, destination, index, 2);
        }
        public static void PutBytes(byte[] destination, int index, int value)
        {
            Array.ConstrainedCopy(BitConverter.GetBytes(value), 0, destination, index, 4);
        }
        public static void PutBytes(byte[] destination, int index, long value)
        {
            Array.ConstrainedCopy(BitConverter.GetBytes(value), 0, destination, index, 8);
        }
        public static void PutBytes(byte[] destination, int index, float value)
        {
            Array.ConstrainedCopy(BitConverter.GetBytes(value), 0, destination, index, 4);
        }
        public static void PutBytes(byte[] destination, int index, double value)
        {
            Array.ConstrainedCopy(BitConverter.GetBytes(value), 0, destination, index, 8);
        }
        public static void PutBytes(byte[] destination, int index, string value, int length)
        {
            byte[] temp;
            if (UseKoreanLocalization) temp = TranslateToBytes(value);
            else temp = value.ToArray().Select(c => (byte)c).ToArray();
            Array.Resize<byte>(ref temp, length);
            Array.ConstrainedCopy(temp, 0, destination, index, length);
        }
        public static short TakeInt16(byte[] source, int index)
        {
            byte[] temp = new byte[2];
            Array.ConstrainedCopy(source, index, temp, 0, 2);
            return BitConverter.ToInt16(temp, 0);
        }
        public static int TakeInt32(byte[] source, int index)
        {
            byte[] temp = new byte[4];
            Array.ConstrainedCopy(source, index, temp, 0, 4);
            return BitConverter.ToInt32(temp, 0);
        }
        public static long TakeInt64(byte[] source, int index)
        {
            byte[] temp = new byte[8];
            Array.ConstrainedCopy(source, index, temp, 0, 8);
            return BitConverter.ToInt64(temp, 0);
        }
        public static float TakeSingle(byte[] source, int index)
        {
            byte[] temp = new byte[4];
            Array.ConstrainedCopy(source, index, temp, 0, 4);
            return BitConverter.ToSingle(temp, 0);
        }
        public static double TakeDouble(byte[] source, int index)
        {
            byte[] temp = new byte[8];
            Array.ConstrainedCopy(source, index, temp, 0, 8);
            return BitConverter.ToDouble(temp, 0);
        }
        public static string TakeString(byte[] source, int index, int length)
        {
            if (UseKoreanLocalization)
            {
                byte[] temp = new byte[length];
                Array.ConstrainedCopy(source, index, temp, 0, length);
                return TranslateToString(temp);
            }

            return new string(source.Skip(index).TakeWhile((b, i) => (b != 0) && (i < length)).Select(b => (char)b).ToArray());
        }
        #endregion
    }

    /// <summary>
    /// Represents a row of data in a DataTableEx.
    /// </summary>
    public class DataRowEx : DataRow
    {
        /// <summary>
        /// Only for internal usage.
        /// </summary>
        internal DataRowEx(DataRowBuilder builder) : base(builder) { }
        
        /// <summary>
        /// Gets or sets the object that contains data about this row.
        /// </summary>
        public object Tag { get; set; }
    }

    
    /// <summary>
    /// Represents one table of in-memory data.
    /// The rows in this table may carry a tagged object with them (see DataRowEx.Tag)
    /// </summary>
    public class DataTableEx : DataTable
    {
        /// <summary>
        /// Initializes a new instance of the DataTableEx class with no arguments.
        /// </summary>
        public DataTableEx() : base() { }

        /// <summary>
        /// Initializes a new instance of the DataTableEx class with the specified table name.
        /// </summary>
        /// <param name="tableName">
        /// The name to give the table. If tableName is null or an empty string,
        /// a default name is given when added to the System.Data.DataTableCollection.
        /// </param>
        public DataTableEx(string tableName) : base(tableName) { }

        /// <summary>
        /// Gets the row type.
        /// </summary>
        protected override Type GetRowType()
        {
            return typeof(DataRowEx);
        }

        /// <summary>
        /// Creates a new row from an existing row.
        /// </summary>
        protected override DataRow NewRowFromBuilder(DataRowBuilder builder)
        {
            return new DataRowEx(builder);
        }

        /// <summary>
        /// Gets the row at the specified index
        /// </summary>
        /// <param name="index">The zero-based index of the row to return.</param>
        public DataRowEx this[int index]
        {
            get { return (DataRowEx)Rows[index]; }
        }

        /// <summary>
        /// Adds the specified DataRowEx to the DataRowCollection object.
        /// </summary>
        /// <param name="row">The DataRowEx to add.</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="ArgumentException"></exception>
        /// <exception cref="ConstraintException"></exception>
        /// <exception cref="NoNullAllowedException"></exception>
        public void Add(DataRowEx row)
        {
            Rows.Add(row);
        }

        /// <summary>
        /// Creates a row using specified values and adds it to the DataRowCollection object.
        /// </summary>
        /// <param name="values">The array of values that are used to create the new row.</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="InvalidCastException"></exception>
        /// <exception cref="ConstraintException"></exception>
        /// <exception cref="NoNullAllowedException"></exception>
        public void Add(object[] values)
        {
            Rows.Add(values);
        }

        /// <summary>
        /// Creates a row using specified values,
        /// associates it with the tag object
        /// and adds it to the DataRowCollection object.
        /// </summary>
        /// <param name="values">The array of values that are used to create the new row.</param>
        /// <param name="tag">An object that contains information about this row.</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="InvalidCastException"></exception>
        /// <exception cref="ConstraintException"></exception>
        /// <exception cref="NoNullAllowedException"></exception>
        public void Add(object[] values, object tag)
        {
            var row = NewRow();
            row.ItemArray = values;
            row.Tag = tag;
            Add(row);
        }

        /// <summary>
        /// Removes the specified DataRowEx from the collection.
        /// </summary>
        /// <param name="row">The DataRowEx to remove.</param>
        public void Remove(DataRowEx row)
        {
            Rows.Remove(row);
        }

        /// <summary>
        /// Creates a new DataRowEx with the same schema as the table.
        /// </summary>
        public new DataRowEx NewRow()
        {
            return base.NewRow() as DataRowEx;
        }

        /// <summary>
        /// Copies both the structure and data for this DataTableEx.
        /// </summary>
        public new DataTableEx Copy()
        {
            return base.Copy() as DataTableEx;
        }
    }
}

namespace AtumZip
{
    using Library;

    public static class FileTypes
    {
        /// <summary>
        /// Maps magic words to file types
        /// </summary>
        private static Dictionary<uint, string> extensions = new Dictionary<uint, string>()
        {
            {0x20666f78, "x"},
            {0x000a0000, "tga"},
            {0x00020000, "tga"},
            {0x00030000, "tga"},
            {0x000b0000, "tga"},
            {0xd8ff0000, "jpg"},
            {0xe0ffd8ff, "jpg"},
            {0x474e5089, "png"},
            {0x46464952, "wav"}
        };

        public static string FindType(uint Code) =>
            extensions.ContainsKey(Code) ? extensions[Code] : null;

        public static string FindType(byte[] Item) =>
            (Item.Length < 4) ? null : FindType(BitConverter.ToUInt32(Item, 0));

        public static string FindType(byte[] Item, int Index) =>
            (Item.Length < 4) ? null : FindType(BitConverter.ToUInt32(Item, Index));

        public static string FindType(Stream Stream) =>
            (Stream.Length - Stream.Position < 4) ? null : FindType(Stream.Read<uint>());
    }

    public class Item
    {
        public Item(string Name, byte[] Data)
        {
            this.Name = Name;
            this.Data = Data;
        }

        public string Name { get; set; }
        public byte[] Data { get; set; }

        public int Size => Data.Length;

        public string Extension => FileTypes.FindType(Data);
    }

    public class ItemInfo
    {
        public ItemInfo(string Name, int Offset, int Size)
        {
            this.Name = Name;
            this.Offset = Offset;
            this.Size = Size;
        }

        public string Name { get; private set; }
        public int Offset { get; private set; }
        public int Size { get; private set; }
    }

    public class FileHeader
    {
        public const int Magic = 1000;

        private int magicA;
        private int magicB;
        private int itemslen;
        private int itemscount;
        private int parity;

        public bool Valid => magicA == Magic && magicB == Magic;
        public int ItemsCount { get { return itemscount; } set { itemscount = value; } }
        public int ItemsLength { get { return itemslen; } set { itemslen = value; } }

        public FileHeader(int ItemsLength, int ItemsCount)
        {
            magicA = Magic;
            magicB = Magic;

            itemslen = ItemsLength;
            itemscount = ItemsCount;

            parity = 0;
        }
        public FileHeader(Stream Stream)
        {
            magicA = Stream.Read<int>();
            magicB = Stream.Read<int>();
            itemslen = Stream.Read<int>();
            itemscount = Stream.Read<int>();
            parity = Stream.Read<int>();
        }

        public void Write(Stream Stream)
        {
            Stream.Position = 0;
            Stream.Write(magicA);
            Stream.Write(magicB);
            Stream.Write(itemslen);
            Stream.Write(itemscount);
            Stream.Write(parity);
        }
    }

    public class ItemHeader
    {
        private int namelen;
        private int itemlen;
        private int parity;
        private string name;

        public string Name { get { return name; } set { name = value; } }
        public int ItemLength { get { return itemlen; } set { itemlen = value; } }

        public ItemHeader(string Name, int ItemLength)
        {
            namelen = Name.Count((c) => c != 0);
            itemlen = ItemLength;
            parity = 0;
            name = Name;
        }
        public ItemHeader(Stream Stream)
        {
            namelen = Stream.Read<int>();
            itemlen = Stream.Read<int>();
            parity = Stream.Read<int>();
            name = new string(Stream.Read<string>(12).TakeWhile((c) => c != 0).ToArray());
        }

        public void Write(Stream Stream)
        {
            Stream.Write(namelen);
            Stream.Write(itemlen);
            Stream.Write(parity);
            Stream.Write(name, 12);
        }
    }

    public class ZippedFile : IDisposable
    {
        protected Stream stream;

        protected ItemInfo[] info;
        protected int itemscount;

        public ZippedFile(Stream Stream)
        {
            if (!Stream.CanRead || !Stream.CanSeek) throw new ArgumentException("Stream must be readable and seekable.");

            stream = Stream;

            FileHeader header = new FileHeader(Stream);

            if (!header.Valid) throw new ArgumentException("Stream does not contain a valid archive.");

            itemscount = header.ItemsCount;
            info = new ItemInfo[itemscount];
            for (int i = 0; i < itemscount; i++)
            {
                ItemHeader h = new ItemHeader(Stream);
                info[i] = new ItemInfo(h.Name, (int)Stream.Position, h.ItemLength);
                Stream.Position += h.ItemLength;
            }
        }
        
        public ZippedFile(string Path) : this(File.Open(Path, FileMode.Open, FileAccess.Read, FileShare.Read)) { }

        // returns null if it doesn't find an entry with this name
        public Stream Find(string name)
        {
            foreach (ItemInfo i in info) if (i.Name == name)
                {
                    stream.Position = i.Offset;
                    return stream;
                }

            return null;
        }
        
        /// <summary>
        /// Returns a stream for reading positioned where the specified item begins.
        /// </summary>
        /// <param name="index">The zero-based index of the specified item.</param>
        /// <exception cref="IndexOutOfRangeException"></exception>
        public Stream At(int index)
        {
            stream.Position = info[index].Offset;
            return stream;
        }

        public byte[] GetItem(string name)
        {
            foreach (ItemInfo i in info) if (i.Name == name)
                {
                    byte[] temp = new byte[i.Size];
                    stream.Position = i.Offset;
                    stream.Read(temp, 0, i.Size);
                    return temp;
                }
            return null;
        }
        
        public Item[] GetAllItems()
        {
            Item[] temp = new Item[itemscount];
            for (int i = 0; i < itemscount; i++)
            {
                temp[i] = new Item(info[i].Name, new byte[info[i].Size]);
                stream.Position = info[i].Offset;
                stream.Read(temp[i].Data, 0, info[i].Size);
            }
            return temp;
        }

        public Stream Stream { get { return stream; } }
        public int ItemsCount { get { return itemscount; } }

        public byte[] this[string Name] { get { return GetItem(Name); } }
        public Stream this[int Index] {  get { stream.Position = info[Index].Offset; return stream; } }

        public static implicit operator ZippedFile(Stream stream)
        {
            return new ZippedFile(stream);
        }

        public static void ZipItems(Item[] Items, string Target)
        {
            using (FileStream fs = File.OpenWrite(Target))
            {
                ZipItems(Items, fs);
            }
        }
        public static void ZipItems(Item[] Items, Stream Target)
        {
            FileHeader header = new FileHeader(Items.Sum((i) => i.Size), Items.Length);
            header.Write(Target);
            foreach (Item item in Items)
            {
                ItemHeader iheader = new ItemHeader(item.Name, item.Size);
                iheader.Write(Target);
                Target.Write(item.Data, 0, item.Size);
            }
        }

        public static void ZipFolder(string Folder, string Target)
        {
            using (FileStream fs = File.OpenWrite(Target))
            {
                ZipFolder(Folder, fs);
            }
        }
        public static void ZipFolder(string Folder, Stream Target)
        {
            string[] files = Directory.GetFiles(Folder, "*.*", SearchOption.TopDirectoryOnly);
            FileHeader header = new FileHeader(files.Sum((s) => (int)new FileInfo(s).Length), files.Length);
            header.Write(Target);
            foreach (string file in files)
            {
                string name = Path.GetFileNameWithoutExtension(file);
                byte[] temp = File.ReadAllBytes(file);
                ItemHeader iheader = new ItemHeader(name, temp.Length);
                iheader.Write(Target);
                Target.Write(temp, 0, temp.Length);
            }
        }

        public static void UnzipFolder(string Folder, string Source)
        {
            using (ZippedFile zpf = File.Open(Source, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                var items = zpf.GetAllItems();

                var destf = Folder + @"\" + Path.GetFileNameWithoutExtension(Source);
                if (!Directory.Exists(destf)) Directory.CreateDirectory(destf);

                foreach (var item in items) File.WriteAllBytes(destf + @"\" + item.Name, item.Data);// + "." + item.Extension);
                
            }
        }

        // will be slow on lots of data, different aproach is to use Find when the data is needed (returns a Stream but you need to keep the zipped file open)
        public static Item[] UnzipItems(string Source)
        {
            using (ZippedFile zpf = File.Open(Source, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                return zpf.GetAllItems();
            }
        }
        public void Close()
        {
            stream.Close();
        }
        public void Dispose()
        {
            stream.Dispose();
        }

        public static bool IsPackedFile(Stream stream)
        {
            return stream.Read<int>() == FileHeader.Magic && stream.Read<int>() == FileHeader.Magic;
        }
    }
}
